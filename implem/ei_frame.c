#include <stdlib.h>
#include <string.h>

#include "../implem/ei_implementation.h"
#include "../api/ei_application.h"
#include "../api/ei_draw.h"
#include "../implem/ei_frame.h"
#include "../api/ei_utils.h"

ei_widget_t frame_allocfunc()
{
    frame_t *frame = malloc(sizeof(frame_t));
    memset(frame, 0, sizeof(frame_t));

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
    // free(widget->requested_size);
    // free(widget->screen_location);
    free(widget->content_rect);
}

void frame_drawfunc(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    frame_t *frame = (frame_t *)widget;

    ei_point_t *point_array = malloc(4 * sizeof(ei_point_t));

    point_array[0] = widget->screen_location.top_left;
    point_array[1] = ei_point(widget->screen_location.top_left.x + widget->screen_location.size.width, widget->screen_location.top_left.y);
    point_array[1] = ei_point(widget->screen_location.top_left.x + widget->screen_location.size.width, widget->screen_location.top_left.y);
    point_array[2] = ei_point(widget->screen_location.top_left.x + widget->screen_location.size.width, widget->screen_location.top_left.y + widget->screen_location.size.height);
    point_array[3] = ei_point(widget->screen_location.top_left.x, widget->screen_location.top_left.y + widget->screen_location.size.height);

    ei_draw_polygon(surface, point_array, 4, frame->color, clipper);

    free(point_array);
}

void frame_setdefaultsfunc(ei_widget_t widget)
{
    ei_widget_t frame_widget = frame_allocfunc();
    frame_t *frame = (frame_t *)frame_widget;

    frame->widget = widget;
    frame->color = (ei_color_t){0, 0, 0, 255};
}
