#include <stdlib.h>

#include "../api/ei_widgetclass.h"
#include "../implem/ei_frame.h"
#include "../implem/ei_widgetclass_ext.h"

ei_widgetclass_t *first_widgetclass = NULL;

void ei_widgetclass_register(ei_widgetclass_t *widgetclass)
{
    if (first_widgetclass == NULL)
    {
        first_widgetclass = widgetclass;
    }
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
    if (first_widgetclass == NULL)
    {
        return NULL;
    }

    ei_widgetclass_t *current = first_widgetclass;

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
    strcpy(frame->name, "frame");
    frame->allocfunc = &frame_allocfunc;
    frame->releasefunc = &frame_releasefunc;
    frame->drawfunc = &frame_drawfunc;
    frame->setdefaultsfunc = &frame_setdefaultsfunc;
    frame->geomnotifyfunc = NULL;
    frame->next = NULL;

    ei_widgetclass_register(frame);
}
