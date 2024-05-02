#ifndef EI_BUTTON_H
#define EI_BUTTON_H

#include "../api/ei_types.h"
#include "../api/hw_interface.h"
#include "../implem/headers/ei_implementation.h"

typedef struct
{
    ei_impl_widget_t widget;
    ei_color_t color;
    int border_width;
    int corner_radius;
    ei_relief_t relief;
} ei_button_t;

ei_widget_t button_allocfunc();

void button_releasefunc(ei_widget_t widget);

void button_drawfunc(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper);

void button_setdefaultsfunc(ei_widget_t widget);

#endif