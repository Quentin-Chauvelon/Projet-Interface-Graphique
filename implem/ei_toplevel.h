#ifndef EI_TOPLEVEL_H
#define EI_TOPLEVEL_H


#include "../implem/ei_implementation.h"

typedef struct
{
    ei_impl_widget_t widget;
    ei_color_t color;
    int border_width;
    ei_string_t		title;
    bool			closable;
    ei_axis_set_t		resizable;
    ei_size_ptr_t		min_size;
} ei_toplevel_t;

ei_widget_t toplevel_allocfunc();

void toplevel_releasefunc(ei_widget_t widget);

void toplevel_drawfunc(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper);

void toplevel_setdefaultsfunc(ei_widget_t widget);



#endif