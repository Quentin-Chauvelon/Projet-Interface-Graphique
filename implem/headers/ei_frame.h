#ifndef EI_FRAME_H
#define EI_FRAME_H

#include "../api/ei_types.h"
#include "../implem/headers/ei_implementation.h"

typedef struct
{
    ei_impl_widget_t widget;
    ei_color_t color;
} ei_frame_t;

ei_widget_t frame_allocfunc();

void frame_releasefunc(ei_widget_t widget);

void frame_drawfunc(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper);

void frame_setdefaultsfunc(ei_widget_t widget);

#endif