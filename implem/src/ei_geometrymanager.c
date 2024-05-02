#include <stdlib.h>

#include "../api/ei_geometrymanager.h"
#include "../api/ei_placer.h"
#include "../implem/headers/ei_geometrymanager_ext.h"

// Keep a pointer to the first geometry manager registered
// Other geometry managers are linked using the next pointer
ei_geometrymanager_t *first_geometrymanager = NULL;

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

void ei_geometrymanager_register_all()
{
    ei_geometrymanager_t *placer = malloc(sizeof(ei_geometrymanager_t));
    strcpy(placer->name, "placer");
    placer->runfunc = NULL;
    placer->releasefunc = NULL;
    placer->next = NULL;

    ei_geometrymanager_register(placer);
}