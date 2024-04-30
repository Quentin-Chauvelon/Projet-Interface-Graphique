#include <stdlib.h>

#include "../api/ei_widgetclass.h"
#include "../implem/ei_frame.h"
#include "../implem/ei_widgetclass_ext.h"

ei_widgetclass_t **widgetclasses;
int widgetclasses_index = 0;

void ei_widgetclass_register(ei_widgetclass_t *widgetclass)
{
    widgetclasses[widgetclasses_index++] = widgetclass;
}

ei_widgetclass_t *ei_widgetclass_from_name(ei_const_string_t name)
{
    for (int i = 0; i < widgetclasses_index; i++)
    {
        if (strcmp(widgetclasses[i]->name, name) == 0)
        {
            return widgetclasses[i];
        }
    }

    return NULL;
}

ei_widgetclass_t *ei_widgetclass_register_all()
{
    widgetclasses = calloc(5, sizeof(ei_widgetclass_t));

    ei_widgetclass_t *frame = malloc(sizeof(ei_widgetclass_t));
    strcpy(frame->name, "frame");
    frame->allocfunc = &frame_allocfunc;
    frame->releasefunc = &frame_releasefunc;
    frame->drawfunc = &frame_drawfunc;
    frame->setdefaultsfunc = &frame_setdefaultsfunc;
    frame->geomnotifyfunc = NULL;
    frame->next = NULL;

    ei_widgetclass_register(frame);

    return frame;
}
