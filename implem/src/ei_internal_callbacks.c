#include "../api/ei_types.h"
#include "../api/ei_event.h"
#include "../implem/headers/ei_button.h"
#include "../implem/headers/ei_event_ext.h"
#include "../implem/headers/ei_internal_callbacks.h"

static bool ei_button_press(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    ei_button_t *button = (ei_button_t *)widget;
    button->relief = ei_relief_sunken;

    // Bind the button release and move event
    // The button should be released if the user releases the mouse button
    // or if they move the cursor out of the button
    ei_bind(ei_ev_mouse_buttonup, widget, NULL, ei_button_release, NULL);
    ei_bind(ei_ev_mouse_move, widget, NULL, ei_cursor_left_button, NULL);

    return false;
}

static bool ei_button_release(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    ei_button_t *button = (ei_button_t *)widget;
    button->relief = ei_relief_raised;

    // Unbind the button release and move event
    ei_unbind(ei_ev_mouse_buttonup, widget, NULL, ei_button_release, NULL);
    ei_unbind(ei_ev_mouse_move, widget, NULL, ei_cursor_left_button, NULL);

    return false;
}

static bool ei_cursor_left_button(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    // If the widget beneath the cursor is not the button, it means the user moved
    // the cursor out of the button, and so, we want to release the button
    if (ei_get_picking_widget() != widget)
    {
        return ei_button_release(widget, event, user_param);
    }

    return false;
}

void ei_bind_all_internal_callbacks()
{
    ei_bind(ei_ev_mouse_buttondown, NULL, "button", ei_button_press, NULL);
}