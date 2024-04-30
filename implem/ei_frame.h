#include "../api/ei_types.h"

typedef struct
{
    ei_widget_t widget;
    ei_color_t color;
} frame_t;

ei_widget_t frame_allocfunc();

void frame_releasefunc(ei_widget_t widget);

void frame_drawfunc(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper);

void frame_setdefaultsfunc(ei_widget_t widget);