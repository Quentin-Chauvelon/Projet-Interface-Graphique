#include <stdlib.h>
#include <string.h>

#include "../api/ei_application.h"
#include "../api/ei_draw.h"
#include "../api/ei_utils.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_frame.h"
#include "../implem/headers/ei_application_ext.h"
#include "../implem/headers/ei_utils_ext.h"

ei_widget_t frame_allocfunc()
{
    ei_frame_t *frame = malloc(sizeof(ei_frame_t));

    // If malloc failed, exit since the program will crash if the user tries to use the frame
    if (frame == NULL)
    {
        printf("\033[0;31mError: Couldn't allocate memory for the frame widget.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        exit(1);
    }

    memset(frame, 0, sizeof(ei_frame_t));

    return (ei_widget_t)frame;
}

void frame_releasefunc(ei_widget_t widget)
{
    ei_frame_t *frame = (ei_frame_t *)widget;

    if (frame->img != NULL)
    {
        free(frame->img);
    }

    if (frame->img_rect != NULL)
    {
        free(frame->img_rect);
    }

    free(frame);
    frame = NULL;
}

void frame_drawfunc(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    if (clipper != NULL && !rect_intersects_rect(widget->screen_location, *clipper))
    {
        return;
    }

    DEBUG ? printf("Drawing widget %d\n", widget->pick_id) : 0;

    ei_frame_t *frame = (ei_frame_t *)widget;

    ei_point_t *point_array = malloc(4 * sizeof(ei_point_t));

    // If malloc failed, return
    if (point_array == NULL)
    {
        printf("\033[0;31mError: Couldn't allocate memory to draw the frame.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        return;
    }

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

void frame_geomnotifyfunc(ei_widget_t widget)
{
    // widget->wclass->drawfunc(widget, ei_app_root_surface(), ei_app_offscreen_picking_surface(), NULL);
}