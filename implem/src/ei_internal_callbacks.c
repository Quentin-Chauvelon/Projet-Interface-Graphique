#include "../api/ei_types.h"
#include "../api/ei_event.h"
#include "../implem/headers/ei_button.h"
#include "../implem/headers/ei_internal_callbacks.h"

static bool ei_button_release(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    ei_button_t *button = (ei_button_t *)widget;
    button->relief = ei_relief_raised;

    // Unbind the button release event
    ei_unbind(ei_ev_mouse_buttonup, widget, NULL, ei_button_release, NULL);

    return false;
}

static bool ei_button_press(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    ei_button_t *button = (ei_button_t *)widget;
    button->relief = ei_relief_sunken;

    // Bind the button release event
    ei_bind(ei_ev_mouse_buttonup, widget, NULL, ei_button_release, NULL);

    return false;
}

void ei_bind_all_internal_callbacks()
{
    ei_bind(ei_ev_mouse_buttondown, NULL, "button", ei_button_press, NULL);
}