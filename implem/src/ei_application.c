#include <stdio.h>

#include "../api/hw_interface.h"
#include "../api/ei_application.h"
#include "../api/ei_widgetclass.h"
#include "../api/ei_geometrymanager.h"
#include "../api/ei_event.h"
#include "../api/ei_types.h"
#include "../api/ei_utils.h"
#include "../implem/headers/ei_button.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_widgetclass_ext.h"
#include "../implem/headers/ei_geometrymanager_ext.h"
#include "../implem/headers/ei_event_ext.h"
#include "../implem/headers/ei_frame.h"
#include "../implem/headers/ei_internal_callbacks.h"
#include "../implem/headers/ei_widget_ext.h"
#include "../implem/headers/ei_utils_ext.h"
#include "../implem/headers/ei_application_ext.h"

ei_widget_t root = NULL;
ei_surface_t window_surface = NULL;
ei_surface_t offscreen_picking = NULL;
bool main_loop_running = true;
ei_linked_rect_t *invalid_rects = NULL;

void ei_app_create(ei_size_t main_window_size, bool fullscreen)
{
    hw_init();

    ei_widgetclass_register_all();

    ei_geometrymanager_register_all();

    ei_bind_all_internal_callbacks();

    window_surface = hw_create_window(main_window_size, fullscreen);

    offscreen_picking = hw_surface_create(window_surface, main_window_size, true);

    root = ei_widget_create_internal("frame", NULL, NULL, NULL);
    root->screen_location = hw_surface_get_rect(window_surface);
}

void ei_app_run(void)
{
    // Only used for debugging
    int main_loop_count = 0;

    ei_event_t event;

    // Invalidate the root widget once to draw the whole screen
    ei_app_invalidate_rect(&ei_app_root_widget()->screen_location);

    ei_widget_t current = ei_app_root_widget();
    while (true)
    {
        // Recompute the geometry of the widget
        if (current->geom_params != NULL)
        {
            current->geom_params->manager->runfunc(current);
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

    while (main_loop_running)
    {
        DEBUG ? printf("Running main loop %d\n", main_loop_count++) : 0;

        hw_surface_lock(window_surface);
        hw_surface_lock(offscreen_picking);

        // Redraw all the widgets but use each invalid_rect as a clipper so that we don't redraw the whole screen every time
        if (invalid_rects != NULL)
        {
            for (ei_linked_rect_t *invalid_rect = invalid_rects; invalid_rect != NULL; invalid_rect = invalid_rect->next)
            {
                DEBUG ? printf("Drawing invalid rect %d %d %d %d\n", invalid_rect->rect.top_left.x, invalid_rect->rect.top_left.y, invalid_rect->rect.size.width, invalid_rect->rect.size.height) : 0;

                // Redraw the root widget
                ei_app_root_widget()->wclass->drawfunc(ei_app_root_widget(), window_surface, offscreen_picking, &invalid_rect->rect);
            }
        }

        hw_surface_unlock(window_surface);
        hw_surface_unlock(offscreen_picking);

        hw_surface_update_rects(window_surface, invalid_rects);

        // Free the invalid_rects linked list
        ei_linked_rect_t *next_invalid_rect;
        for (ei_linked_rect_t *invalid_rect = invalid_rects; invalid_rect != NULL; invalid_rect = next_invalid_rect)
        {
            next_invalid_rect = invalid_rect->next;
            free(invalid_rect);
        }
        invalid_rects = NULL;

        hw_event_wait_next(&event);

        handle_event(event);
    }
}

void ei_app_free(void)
{
    free(root);
    hw_surface_free(offscreen_picking);
    hw_quit();
}

void ei_app_invalidate_rect(const ei_rect_t *rect)
{
    DEBUG ? printf("Invalidating rect %d %d %d %d\n", rect->top_left.x, rect->top_left.y, rect->size.width, rect->size.height) : 0;

    // If the linked list is empty, add the first element
    if (invalid_rects == NULL)
    {
        invalid_rects = malloc(sizeof(ei_linked_rect_t));
        invalid_rects->rect = *rect;
        invalid_rects->next = NULL;
    }
    // Otherwise, insert the rectangle in the linked list
    else
    {
        ei_linked_rect_t *current_invalid_rect = invalid_rects;
        while (current_invalid_rect->next != NULL)
        {
            printf("current_invalid_rect %p\n", current_invalid_rect);
            // If the rectangle is fully included in another rectangle, we don't need it
            if (rect_included_in_rect(*rect, current_invalid_rect->rect))
            {
                return;
            }

            // If two rectangles have a big enough common area, merge them, overwrite the current rectangle in
            // the linked list and return since we don't need the new rectangle anymore
            if (get_intersection_percentage(*rect, current_invalid_rect->rect) >= RECTANGLES_MERGE_THRESHOLD)
            {
                DEBUG ? printf("Merged %d %d %d %d with %d %d %d %d\n", rect->top_left.x, rect->top_left.y, rect->size.width, rect->size.height, current_invalid_rect->rect.top_left.x, current_invalid_rect->rect.top_left.y, current_invalid_rect->rect.size.width, current_invalid_rect->rect.size.height) : 0;
                current_invalid_rect->rect = merge_rectangles(*rect, current_invalid_rect->rect);
                return;
            }

            current_invalid_rect = current_invalid_rect->next;
        }

        ei_linked_rect_t *invalid_rect = malloc(sizeof(ei_linked_rect_t));
        invalid_rect->rect = *rect;
        invalid_rect->next = NULL;

        // Link the rectangle to the last element of the linked list
        current_invalid_rect->next = invalid_rect;
    }
}

void ei_app_quit_request(void)
{
    main_loop_running = false;
}

ei_widget_t ei_app_root_widget(void)
{
    return root;
}

ei_surface_t ei_app_root_surface(void)
{
    return window_surface;
}

ei_surface_t ei_app_offscreen_picking_surface(void)
{
    return offscreen_picking;
}