#include <stdio.h>

#include "../api/hw_interface.h"
#include "../api/ei_application.h"
#include "../api/ei_widgetclass.h"
#include "../api/ei_geometrymanager.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_widgetclass_ext.h"
#include "../implem/headers/ei_geometrymanager_ext.h"
#include "../implem/headers/ei_frame.h"

ei_widget_t root = NULL;
ei_surface_t window_surface = NULL;
ei_surface_t picking_offscreen = NULL;

void ei_app_create(ei_size_t main_window_size, bool fullscreen)
{
    hw_init();

    ei_widgetclass_register_all();

    ei_geometrymanager_register_all();

    window_surface = hw_create_window(main_window_size, fullscreen);

    picking_offscreen = hw_surface_create(window_surface, main_window_size, true);

    root = ei_widget_create("frame", NULL, NULL, NULL);
    root->screen_location = hw_surface_get_rect(window_surface);
}

void ei_app_run(void)
{
    ei_widget_t current = root;

    hw_surface_lock(window_surface);
    hw_surface_lock(picking_offscreen);

    while (true)
    {
        current->wclass->drawfunc(current, window_surface, picking_offscreen, NULL);

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
    hw_surface_unlock(picking_offscreen);

    hw_surface_update_rects(window_surface, NULL);

    getchar();
}

void ei_app_free(void)
{
    free(root);
    hw_quit();
}

ei_widget_t ei_app_root_widget(void)
{
    return root;
}