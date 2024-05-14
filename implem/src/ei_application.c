#include <stdio.h>

#include "../api/hw_interface.h"
#include "../api/ei_application.h"
#include "../api/ei_widgetclass.h"
#include "../api/ei_geometrymanager.h"
#include "../api/ei_parser.h"
#include "../api/ei_event.h"
#include "../api/ei_types.h"
#include "../api/ei_utils.h"
#include "../implem/headers/ei_button.h"
#include "../implem/headers/ei_toplevel.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_widgetclass_ext.h"
#include "../implem/headers/ei_geometrymanager_ext.h"
#include "../implem/headers/ei_event_ext.h"
#include "../implem/headers/ei_frame.h"
#include "../implem/headers/ei_internal_callbacks.h"
#include "../implem/headers/ei_widget_ext.h"
#include "../implem/headers/ei_utils_ext.h"
#include "../implem/headers/ei_application_ext.h"
#include "../implem/headers/ei_entry.h"

static ei_widget_t root = NULL;
static ei_surface_t window_surface = NULL;
static ei_surface_t offscreen_picking = NULL;
static bool main_loop_running = true;
static ei_linked_rect_t *invalid_rects = NULL;

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

    ei_event_t *event = (ei_event_t *)malloc(sizeof(struct ei_event_t));

    // If malloc failed, exit since the program can't run without the event
    if (event == NULL)
    {
        printf("\033[0;31mError: Couldn't allocate memory for event.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        exit(1);
    }

    event->type = ei_ev_none;

    // Invalidate the root widget once to draw the whole screen
    ei_app_invalidate_rect(&ei_app_root_widget()->screen_location);

    // Recompute the geometry of all widgets
    ei_recompute_geometry_of_all_descendants(&root);

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

        hw_event_wait_next(event);

        ei_handle_event(*event);

        ei_unbind_events_registered_for_unbind();
    }

    free(event);
}

void ei_app_free(void)
{
    free_name_to_widget_list();

    ei_widget_destroy(ei_app_root_widget());

    ei_unbind_events_registered_for_unbind();

    ei_unbind_all_events();

    ei_widgetclass_free_all();

    ei_geometrymanager_free_all();

    if (clipboard != NULL)
    {
        free(clipboard);
    }

    ei_linked_rect_t *next_invalid_rect;
    for (ei_linked_rect_t *invalid_rect = invalid_rects; invalid_rect != NULL; invalid_rect = next_invalid_rect)
    {
        next_invalid_rect = invalid_rect->next;
        free(invalid_rect);
    }
    invalid_rects = NULL;

    hw_surface_free(offscreen_picking);
    hw_quit();
}

void ei_app_invalidate_rect(const ei_rect_t *rect)
{
    // If the linked list is empty, add the first element
    if (invalid_rects == NULL)
    {
        invalid_rects = malloc(sizeof(ei_linked_rect_t));

        // If malloc failed, return
        if (invalid_rects == NULL)
        {
            printf("\033[0;31mError: Couldn't allocate memory to invalidate rectangle.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
            return;
        }

        DEBUG ? printf("Invalidating rect %d %d %d %d\n", rect->top_left.x, rect->top_left.y, rect->size.width, rect->size.height) : 0;

        invalid_rects->rect = *rect;
        invalid_rects->next = NULL;
    }
    // Otherwise, insert the rectangle in the linked list
    else
    {
        ei_linked_rect_t *current_invalid_rect = invalid_rects;
        while (true)
        {
            // If the rectangle is fully included in another rectangle, we don't need it
            if (ei_rect_included_in_rect(*rect, current_invalid_rect->rect))
            {
                return;
            }

            // If two rectangles have a big enough common area, merge them, overwrite the current rectangle in
            // the linked list and return since we don't need the new rectangle anymore
            if (ei_get_intersection_percentage(*rect, current_invalid_rect->rect) >= rectangles_merge_threshold)
            {
                DEBUG ? printf("Merged %d %d %d %d with %d %d %d %d\n", rect->top_left.x, rect->top_left.y, rect->size.width, rect->size.height, current_invalid_rect->rect.top_left.x, current_invalid_rect->rect.top_left.y, current_invalid_rect->rect.size.width, current_invalid_rect->rect.size.height) : 0;
                current_invalid_rect->rect = ei_merge_rectangles(*rect, current_invalid_rect->rect);
                return;
            }

            if (current_invalid_rect->next == NULL)
            {
                break;
            }

            current_invalid_rect = current_invalid_rect->next;
        }

        ei_linked_rect_t *invalid_rect = malloc(sizeof(ei_linked_rect_t));

        // If malloc failed, return
        if (invalid_rect == NULL)
        {
            printf("\033[0;31mError: Couldn't allocate memory to invalidate rectangle.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
            return;
        }

        DEBUG ? printf("Invalidating rect %d %d %d %d\n", rect->top_left.x, rect->top_left.y, rect->size.width, rect->size.height) : 0;

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

bool ei_is_app_running(void)
{
    return main_loop_running;
}