#include "../api/ei_types.h"
#include "../api/ei_event.h"
#include "../api/ei_utils.h"
#include "../api/ei_application.h"
#include "../implem/headers/ei_button.h"
#include "../implem/headers/ei_event_ext.h"
#include "../implem/headers/ei_internal_callbacks.h"
#include "../implem/headers/ei_application_ext.h"
#include "../implem/headers/ei_utils_ext.h"

static bool ei_button_press(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    ei_button_t *button = (ei_button_t *)widget;
    button->frame_appearance.relief = ei_relief_sunken;

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
    button->frame_appearance.relief = ei_relief_raised;

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

static bool toggle_offscreen_picking_surface_display(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    if (event->param.key_code == SDLK_g)
    {
        // Custom parameter to act as a backup of the root surface so that
        // it can be redrawn when turning off the offscreen picking surface.
        // Checking if the surface is NULL also allows to know if we should
        // toggle on or off the offscreen picking surface.
        ei_surface_t *root_surface_copy = (ei_surface_t *)user_param;

        ei_widget_t current = ei_app_root_widget();
        while (true)
        {
            // Recompute the geometry of the widget
            if (current->geom_params != NULL)
            {
                // // If the offscreen picking surface is already displayed, reset the pick color
                if (*root_surface_copy != NULL)
                {
                    *current->pick_color = get_color_from_id(current->pick_id);
                }
                // Otherwise multiply by a big number to differentiate the pick colors from one another
                else
                {
                    *current->pick_color = get_color_from_id(current->pick_id * 1000000);
                }

                current->wclass->drawfunc(current, ei_app_root_surface(), ei_app_offscreen_picking_surface(), &widget->screen_location);
            }

            if (current->next_sibling != NULL)
            {
                current = current->next_sibling;
            }
            else if (current->children_head != NULL)
            {
                current = current->children_head;
            }
            else
            {
                break;
            }
        }

        hw_surface_lock(ei_app_root_surface());
        hw_surface_lock(ei_app_offscreen_picking_surface());

        // If the offscreen picking surface is not displayed, make a copy of the root surface
        // and display the offscreen picking surface
        if (*root_surface_copy == NULL)
        {
            printf("Displaying offscreen picking surface\n");
            *root_surface_copy = hw_surface_create(ei_app_root_surface(), hw_surface_get_size(ei_app_root_surface()), true);
            ei_copy_surface(*root_surface_copy, NULL, ei_app_root_surface(), NULL, false);

            ei_copy_surface(ei_app_root_surface(), NULL, ei_app_offscreen_picking_surface(), NULL, false);
        }
        // Otherwise, restore the root surface and delete the copy
        else
        {
            printf("Hiding offscreen picking surface\n");
            ei_copy_surface(ei_app_root_surface(), NULL, *root_surface_copy, NULL, false);

            hw_surface_free(*root_surface_copy);
            *root_surface_copy = NULL;
        }

        hw_surface_unlock(ei_app_root_surface());
        hw_surface_unlock(ei_app_offscreen_picking_surface());

        hw_surface_update_rects(ei_app_root_surface(), NULL);

        return true;
    }

    return false;
}

void ei_bind_all_internal_callbacks()
{
    ei_bind(ei_ev_mouse_buttondown, NULL, "button", ei_button_press, NULL);

    ei_surface_t root_surface_copy = NULL;
    ei_bind(ei_ev_keydown, NULL, "all", toggle_offscreen_picking_surface_display, &root_surface_copy);
}

void test()
{
    toggle_offscreen_picking_surface_display(ei_app_root_widget(), NULL, NULL);
}