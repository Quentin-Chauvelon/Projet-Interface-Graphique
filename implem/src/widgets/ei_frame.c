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

    if (ei_frame_get_image_data(frame) != NULL)
    {
        free(ei_frame_get_image_data(frame));
    }

    if (ei_frame_get_image_rect(frame) != NULL)
    {
        free(ei_frame_get_image_rect(frame));
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

    ei_draw_polygon(surface, point_array, 4, ei_frame_get_color(frame), clipper);
    ei_draw_polygon(pick_surface, point_array, 4, *widget->pick_color, clipper);

    free(point_array);

    ei_impl_widget_draw_children(widget, surface, pick_surface, clipper);
}

void frame_setdefaultsfunc(ei_widget_t widget)
{
    ei_frame_t *frame = (ei_frame_t *)widget;

    frame->widget = *widget;
    ei_frame_set_color(frame, (ei_color_t){0, 0, 0, 255});
}

void frame_geomnotifyfunc(ei_widget_t widget)
{
    // widget->wclass->drawfunc(widget, ei_app_root_surface(), ei_app_offscreen_picking_surface(), NULL);
}

ei_color_t ei_frame_get_color(ei_frame_t *frame)
{
    return frame->widget_appearance.color;
}

int ei_frame_get_border_width(ei_frame_t *frame)
{
    return frame->widget_appearance.border_width;
}

ei_relief_t ei_frame_get_relief(ei_frame_t *frame)
{
    return frame->frame_appearance.relief;
}

ei_string_t ei_frame_get_text_label(ei_frame_t *frame)
{
    return frame->frame_appearance.text.label;
}

ei_font_t ei_frame_get_text_font(ei_frame_t *frame)
{
    return frame->frame_appearance.text.font;
}

ei_color_t ei_frame_get_text_color(ei_frame_t *frame)
{
    return frame->frame_appearance.text.color;
}

ei_anchor_t ei_frame_get_text_anchor(ei_frame_t *frame)
{
    return frame->frame_appearance.text.anchor;
}

ei_surface_t ei_frame_get_image_data(ei_frame_t *frame)
{
    return frame->frame_appearance.image.rect;
}

ei_rect_t *ei_frame_get_image_rect(ei_frame_t *frame)
{
    return frame->frame_appearance.image.rect;
}

ei_anchor_t ei_frame_get_image_anchor(ei_frame_t *frame)
{
    return frame->frame_appearance.image.anchor;
}

ei_size_t *ei_frame_get_natural_size(ei_frame_t *frame)
{
    if (&frame->widget == ei_app_root_widget())
    {
        return &frame->widget.screen_location.size;
    }
    else
    {
        return &frame->widget.requested_size;
    }
}

void ei_frame_set_color(ei_frame_t *frame, ei_color_t color)
{
    frame->widget_appearance.color = color;
}

void ei_frame_set_border_width(ei_frame_t *frame, int border_width)
{
    frame->widget_appearance.border_width = border_width;
}

void ei_frame_set_relief(ei_frame_t *frame, ei_relief_t relief)
{
    frame->frame_appearance.relief = relief;
}

void ei_frame_set_text_label(ei_frame_t *frame, ei_string_t text)
{
    frame->frame_appearance.text.label = text;
}

void ei_frame_set_text_font(ei_frame_t *frame, ei_font_t text_font)
{
    frame->frame_appearance.text.font = text_font;
}

void ei_frame_set_text_color(ei_frame_t *frame, ei_color_t text_color)
{
    frame->frame_appearance.text.color = text_color;
}

void ei_frame_set_text_anchor(ei_frame_t *frame, ei_anchor_t text_anchor)
{
    frame->frame_appearance.text.anchor = text_anchor;
}

void ei_frame_set_image_data(ei_frame_t *frame, ei_surface_t image)
{
    frame->frame_appearance.image.data = image;
}

void ei_frame_set_image_rect(ei_frame_t *frame, ei_rect_t *image_rect)
{
    frame->frame_appearance.image.rect = image_rect;
}

void ei_frame_set_image_anchor(ei_frame_t *frame, ei_anchor_t image_anchor)
{
    frame->frame_appearance.image.anchor = image_anchor;
}