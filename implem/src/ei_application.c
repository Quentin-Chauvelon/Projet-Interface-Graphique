#include <stdio.h>

#include "../api/hw_interface.h"
#include "../api/ei_application.h"
#include "../api/ei_widgetclass.h"
#include "../api/ei_geometrymanager.h"
#include "../api/ei_event.h"
#include "../api/ei_types.h"
#include "../implem/headers/ei_button.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_widgetclass_ext.h"
#include "../implem/headers/ei_geometrymanager_ext.h"
#include "../implem/headers/ei_frame.h"

ei_widget_t root = NULL;
ei_surface_t window_surface = NULL;
ei_surface_t offscreen_picking = NULL;

void ei_app_create(ei_size_t main_window_size, bool fullscreen)
{
    hw_init();

    ei_widgetclass_register_all();

    ei_geometrymanager_register_all();

    window_surface = hw_create_window(main_window_size, fullscreen);

    offscreen_picking = hw_surface_create(window_surface, main_window_size, true);

    root = ei_widget_create("frame", NULL, NULL, NULL);
    root->screen_location = hw_surface_get_rect(window_surface);
}

void ei_app_run(void)
{
    ei_event_t event;

    while (true)
    {
        ei_widget_t current = root;

        hw_surface_lock(window_surface);
        hw_surface_lock(offscreen_picking);

        while (true)
        {
            current->wclass->drawfunc(current, window_surface, offscreen_picking, NULL);

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

        hw_surface_unlock(window_surface);
        hw_surface_unlock(offscreen_picking);

        hw_surface_update_rects(window_surface, NULL);

        hw_event_wait_next(&event);

        // User pressed a key
        if (event.type == ei_ev_keydown)
        {
            // If the user pressed the escape key, exit the application
            if (event.param.key_code == SDLK_ESCAPE)
            {
                break;
            }
        }
        // User pressed a mouse button
        else if (event.type == ei_ev_mouse_buttondown)
        {
            ei_widget_t widget = ei_widget_pick(&event.param.mouse.where);

            // If the user clicked on a button, call its callback function
            if (widget != NULL && strcmp(widget->wclass->name, "button") == 0)
            {
                ei_button_t *button = (ei_button_t *)widget;

                if (button->callback != NULL)
                {
                    button->callback(widget, &event, button->user_param);
                }
            }
        }
    }
}

void ei_app_free(void)
{
    free(root);
    hw_surface_free(offscreen_picking);
    hw_quit();
}

ei_widget_t ei_app_root_widget(void)
{
    return root;
}

ei_surface_t ei_app_offscreen_picking_surface(void)
{
    return offscreen_picking;
}