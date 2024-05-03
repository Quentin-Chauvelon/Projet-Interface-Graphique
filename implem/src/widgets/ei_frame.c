#include <stdlib.h>
#include <string.h>

#include "../api/ei_application.h"
#include "../api/ei_draw.h"
#include "../api/ei_utils.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_frame.h"

ei_widget_t frame_allocfunc()
{
    ei_frame_t *frame = malloc(sizeof(ei_frame_t));
    memset(frame, 0, sizeof(ei_frame_t));

    return (ei_widget_t)frame;
}

void frame_releasefunc(ei_widget_t widget)
{
    free(widget->wclass);
    free(widget->pick_color);
    free(widget->user_data);
    free(widget->destructor);
    free(widget->parent);
    free(widget->children_head);
    free(widget->children_tail);
    free(widget->next_sibling);
    free(widget->geom_params);
    free(widget->content_rect);

    free((ei_frame_t *)widget);
}

void frame_drawfunc(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    ei_frame_t *frame = (ei_frame_t *)widget;

    ei_point_t *point_array = malloc(4 * sizeof(ei_point_t));

    point_array[0] = widget->screen_location.top_left;
    point_array[1] = ei_point(widget->screen_location.top_left.x + widget->screen_location.size.width, widget->screen_location.top_left.y);
    point_array[2] = ei_point(widget->screen_location.top_left.x + widget->screen_location.size.width, widget->screen_location.top_left.y + widget->screen_location.size.height);
    point_array[3] = ei_point(widget->screen_location.top_left.x, widget->screen_location.top_left.y + widget->screen_location.size.height);

    ei_draw_polygon(surface, point_array, 4, frame->color, clipper);
    ei_draw_polygon(pick_surface, point_array, 4, *widget->pick_color, clipper);

    free(point_array);

    ei_impl_widget_draw_children(widget, surface, pick_surface, clipper);
}

void frame_setdefaultsfunc(ei_widget_t widget)
{
    ei_frame_t *frame = (ei_frame_t *)widget;

    frame->widget = *widget;
    frame->color = (ei_color_t){0, 0, 0, 255};
}
