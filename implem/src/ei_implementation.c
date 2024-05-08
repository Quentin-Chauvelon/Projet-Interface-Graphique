#include "../api/ei_types.h"
#include "../api/hw_interface.h"
#include "../implem/headers/ei_implementation.h"

void ei_impl_widget_draw_children(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    ei_widget_t current = widget->children_head;

    while (current != NULL)
    {
        current->wclass->drawfunc(current, surface, pick_surface, current->content_rect);
        current = current->next_sibling;
    }
}