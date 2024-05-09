#include <stdlib.h>

#include "../api/ei_geometrymanager.h"
#include "../api/ei_placer.h"
#include "../api/ei_utils.h"
#include "../api/ei_application.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_application_ext.h"
#include "../implem/headers/ei_geometrymanager_ext.h"
#include "../implem/headers/ei_placer_ext.h"
#include "../implem/headers/ei_utils_ext.h"
#include "../implem/headers/ei_toplevel.h"

// Keep a pointer to the first geometry manager registered
// Other geometry managers are linked using the next pointer
ei_geometrymanager_t *first_geometrymanager = NULL;

size_t ei_geom_param_size()
{
    return sizeof(ei_impl_geom_param_t);
}

void ei_geometry_run_finalize(ei_widget_t widget, ei_rect_t *new_screen_location)
{
    // If the geometry computation did not result in any change of the geometry of the widget, return
    if (widget->screen_location.top_left.x == new_screen_location->top_left.x &&
        widget->screen_location.top_left.y == new_screen_location->top_left.y &&
        widget->screen_location.size.width == new_screen_location->size.width &&
        widget->screen_location.size.height == new_screen_location->size.height)
    {
        // If the widget is the close button of a toplevel, don't invalidate it,
        // otherwise this causes issues when displaying the offscreen picking surface.
        // This is because the toplevel contains a button for which the geometry is
        // recomputed during the toplevel redraw, which then calls the button drawfunc
        // which leads to the whole toplevel being invalidated and redrawn over the
        // offscreen picking surface
        if (strcmp(widget->parent->wclass->name, "toplevel") == 0 &&
            ((ei_toplevel_t *)widget->parent)->close_button != NULL &&
            (ei_widget_t *)((ei_toplevel_t *)widget->parent)->close_button == (ei_widget_t *)widget)
        {
            return;
        }

        // If the widget's geometry has not changed, invalidate the widget's location
        // because it may have to be redrawn anyway (eg: button pressed internal event).
        ei_app_invalidate_rect(&widget->screen_location);

        return;
    }

    // If the area of the intersection is at least 20% of both rectangles, invalidate the intersection.
    // Otherwise, most of rectangles will be drawn twice.
    if (ei_get_intersection_percentage(widget->screen_location, *new_screen_location) >= RECTANGLES_MERGE_THRESHOLD)
    {
        DEBUG ? printf("Merged %d %d %d %d with %d %d %d %d\n", widget->screen_location.top_left.x, widget->screen_location.top_left.y, widget->screen_location.size.width, widget->screen_location.size.height, new_screen_location->top_left.x, new_screen_location->top_left.y, new_screen_location->size.width, new_screen_location->size.height) : 0;

        ei_rect_t merged_rectange = ei_merge_rectangles(widget->screen_location, *new_screen_location);
        ei_app_invalidate_rect(&merged_rectange);
    }
    else
    {
        ei_app_invalidate_rect(&widget->screen_location);
        ei_app_invalidate_rect(new_screen_location);
    }

    widget->screen_location = *new_screen_location;

    if (widget->wclass->geomnotifyfunc != NULL)
    {
        widget->wclass->geomnotifyfunc(widget);
    }

    for (ei_widget_t children = widget->children_head; children != NULL; children = children->next_sibling)
    {
        if (ei_widget_is_displayed(children))
        {
            children->geom_params->manager->runfunc(children);
        }
    }
}

void ei_geometrymanager_register(ei_geometrymanager_t *geometrymanager)
{
    // If a geometry manager already exists with the same name, return
    if (ei_geometrymanager_from_name(geometrymanager->name) != NULL)
    {
        printf("\033[0;31mError: A geometry manager called \"%s\" already exists.\n\t at %s (%s:%d)\033[0m\n", geometrymanager->name, __func__, __FILE__, __LINE__);
        return;
    }

    // If no geometry manager have been registered yet, set the first one
    if (first_geometrymanager == NULL)
    {
        first_geometrymanager = geometrymanager;
    }
    // Otherwise, link the new geometry manager to the previous one
    else
    {
        ei_geometrymanager_t *current = first_geometrymanager;

        while (current->next != NULL)
        {
            current = current->next;
        }

        current->next = geometrymanager;
    }
}

ei_geometrymanager_t *ei_geometrymanager_from_name(ei_geometrymanager_name_t name)
{
    // If no geometry manager have been registered yet, return NULL
    if (first_geometrymanager == NULL)
    {
        return NULL;
    }

    ei_geometrymanager_t *current = first_geometrymanager;

    // Traverse the linked list to find the geometry manager with the given name
    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0)
        {
            return current;
        }

        current = current->next;
    }

    return NULL;
}

void ei_geometrymanager_unmap(ei_widget_t widget)
{
    // If the widget is not currently displayed, return silently
    if (!ei_widget_is_displayed(widget))
    {
        return;
    }

    widget->geom_params->manager->releasefunc(widget);

    free(widget->geom_params);
    widget->geom_params = NULL;

    ei_app_invalidate_rect(&widget->screen_location);

    widget->screen_location = ei_rect_zero();
}

void ei_geometrymanager_register_all()
{
    ei_geometrymanager_t *placer = malloc(sizeof(ei_geometrymanager_t));

    // If malloc failed, return
    if (placer == NULL)
    {
        printf("\033[0;31mError: Couldn't allocate memory to register geometry manager.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        return;
    }

    strcpy(placer->name, "placer");
    placer->runfunc = &ei_placer_runfunc;
    placer->releasefunc = &ei_placer_releasefunc;
    placer->next = NULL;

    ei_geometrymanager_register(placer);
}

void ei_geometrymanager_free_all()
{
    ei_geometrymanager_t *current = first_geometrymanager;
    ei_geometrymanager_t *next = NULL;

    while (current != NULL)
    {
        next = current->next;

        // Only free the geometry managers that were created by the library,
        // it is up to the programmer to free any geometry manager they
        // would have added
        if (strcmp(current->name, "placer") == 0)
        {
            free(current);
        }

        current = next;
    }
}

ei_geometrymanager_t *ei_widget_get_geom_manager(ei_widget_t widget)
{
    return widget->geom_params->manager;
}

void ei_widget_set_geom_manager(ei_widget_t widget, ei_geometrymanager_t *manager)
{
    if (ei_widget_is_displayed(widget))
    {
        widget->geom_params->manager = manager;
    }
}

ei_geom_param_t ei_widget_get_geom_params(ei_widget_t widget)
{
    return widget->geom_params;
}

void ei_widget_set_geom_params(ei_widget_t widget, ei_geom_param_t geom_params)
{
    widget->geom_params = geom_params;
}

void ei_recompute_geometry_of_all_descendants(ei_widget_t *widget)
{
    if (widget == NULL)
    {
        return;
    }

    // Recompute the geometry of the widget
    if (ei_widget_is_displayed(*widget))
    {
        (*widget)->geom_params->manager->runfunc(*widget);
    }

    // Update the geometry of all children
    for (ei_widget_t children = (*widget)->children_head; children != NULL; children = children->next_sibling)
    {
        ei_recompute_geometry_of_all_descendants(&children);
    }
}