#include <stdlib.h>

#include "../api/ei_widgetclass.h"
#include "../implem/headers/ei_widgetclass_ext.h"
#include "../implem/headers/ei_frame.h"
#include "../implem/headers/ei_button.h"
#include "../implem/headers/ei_implementation.h"

// Keep a pointer to the first widget class registered
// Other widget classes are linked using the next pointer
ei_widgetclass_t *first_widgetclass = NULL;

size_t ei_widget_struct_size()
{
    return sizeof(ei_impl_widget_t);
}

void ei_widgetclass_register(ei_widgetclass_t *widgetclass)
{
    // If no widget class have been registered yet, set the first one
    if (first_widgetclass == NULL)
    {
        first_widgetclass = widgetclass;
    }
    // Otherwise, link the new widget class to the previous one
    else
    {
        ei_widgetclass_t *current = first_widgetclass;

        while (current->next != NULL)
        {
            current = current->next;
        }

        current->next = widgetclass;
    }
}

ei_widgetclass_t *ei_widgetclass_from_name(ei_const_string_t name)
{
    // If no widget class have been registered yet, return NULL
    if (first_widgetclass == NULL)
    {
        return NULL;
    }

    ei_widgetclass_t *current = first_widgetclass;

    // Traverse the linked list to find the widget class with the given name
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

void ei_widgetclass_register_all()
{
    ei_widgetclass_t *frame = malloc(sizeof(ei_widgetclass_t));

    // If malloc failed, return
    if (frame == NULL)
    {
        printf("\033[0;31mError: Couldn't allocate memory for the frame widget class.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        return;
    }

    strcpy(frame->name, "frame");
    frame->allocfunc = &frame_allocfunc;
    frame->releasefunc = &frame_releasefunc;
    frame->drawfunc = &frame_drawfunc;
    frame->setdefaultsfunc = &frame_setdefaultsfunc;
    frame->geomnotifyfunc = &frame_geomnotifyfunc;
    frame->next = NULL;

    ei_widgetclass_t *button = malloc(sizeof(ei_widgetclass_t));

    // If malloc failed, return
    if (button == NULL)
    {
        printf("\033[0;31mError: Couldn't allocate memory for the button widget class.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        return;
    }

    strcpy(button->name, "button");
    button->allocfunc = &button_allocfunc;
    button->releasefunc = &button_releasefunc;
    button->drawfunc = &button_drawfunc;
    button->setdefaultsfunc = &button_setdefaultsfunc;
    button->geomnotifyfunc = &button_geomnotifyfunc;
    button->next = NULL;

    ei_widgetclass_register(frame);
    ei_widgetclass_register(button);
}

void ei_widgetclass_free_all()
{
    ei_widgetclass_t *current = first_widgetclass;
    ei_widgetclass_t *next;

    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
}