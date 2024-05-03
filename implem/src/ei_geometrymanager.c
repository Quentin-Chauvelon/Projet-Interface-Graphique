#include <stdlib.h>

#include "../api/ei_geometrymanager.h"
#include "../api/ei_placer.h"
#include "../api/ei_utils.h"
#include "../api/ei_application.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_geometrymanager_ext.h"
#include "../implem/headers/ei_placer_ext.h"

// Keep a pointer to the first geometry manager registered
// Other geometry managers are linked using the next pointer
ei_geometrymanager_t *first_geometrymanager = NULL;

size_t ei_geom_param_size()
{
    return sizeof(ei_impl_geom_param_t);
}

void ei_geometrymanager_register(ei_geometrymanager_t *geometrymanager)
{
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
    if (widget->geom_params == NULL)
    {
        return;
    }

    widget->geom_params->manager->releasefunc(widget);

    free(widget->geom_params);

    ei_app_invalidate_rect(&widget->screen_location);

    widget->screen_location = ei_rect_zero();
}

void ei_geometrymanager_register_all()
{
    ei_geometrymanager_t *placer = malloc(sizeof(ei_geometrymanager_t));
    strcpy(placer->name, "placer");
    placer->runfunc = &ei_placer_runfunc;
    placer->releasefunc = NULL;
    placer->next = NULL;

    ei_geometrymanager_register(placer);
}

ei_geometrymanager_t *ei_widget_get_geom_manager(ei_widget_t widget)
{
    return widget->geom_params->manager;
}

void ei_widget_set_geom_manager(ei_widget_t widget, ei_geometrymanager_t *manager)
{
    widget->geom_params->manager = manager;
}

ei_geom_param_t ei_widget_get_geom_params(ei_widget_t widget)
{
    return widget->geom_params;
}

void ei_widget_set_geom_params(ei_widget_t widget, ei_geom_param_t geom_params)
{
    widget->geom_params = geom_params;
}