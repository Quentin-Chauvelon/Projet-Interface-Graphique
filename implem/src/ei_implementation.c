#include "../api/ei_types.h"
#include "../api/hw_interface.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_utils_ext.h"

void ei_impl_widget_draw_children(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    ei_widget_t current = widget->children_head;

    while (current != NULL)
    {
        // If there is no intersection between the clipper (representing the widget that has had an event)
        // and the current widget, don't redraw it
        if (clipper == NULL || ei_rect_intersects_rect(current->screen_location, *clipper))
        {
            ei_rect_t children_clipper = ei_get_intersection_rectangle(current->screen_location, *clipper);
            current->wclass->drawfunc(current, surface, pick_surface, &children_clipper);
        }

        current = current->next_sibling;
    }
}