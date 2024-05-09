#include <stdlib.h>
#include <string.h>

#include "../api/ei_application.h"
#include "../api/ei_draw.h"
#include "../api/ei_utils.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_frame.h"
#include "../implem/headers/ei_draw_ext.h"
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

    if (frame->frame_appearance.image.data != NULL)
    {
        free(frame->frame_appearance.image.data);
    }

    free(frame);
    frame = NULL;
}

void frame_drawfunc(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    DEBUG ? printf("Drawing widget %d\n", widget->pick_id) : 0;

    ei_frame_t *frame = (ei_frame_t *)widget;

    // Draw the visible frame
    ei_draw_straight_frame(surface, widget->screen_location, frame->widget_appearance.border_width, frame->widget_appearance.color, frame->frame_appearance.relief, NULL);

    // Draw the text
    if (frame->frame_appearance.text.label != NULL)
    {
        int width = 0;
        int height = 0;
        hw_text_compute_size(frame->frame_appearance.text.label, frame->frame_appearance.text.font, &width, &height);

        ei_point_t where = get_position_in_parent_from_anchor(*widget->content_rect, ei_size(width, height), frame->frame_appearance.text.anchor);

        ei_draw_text(surface, &where, frame->frame_appearance.text.label, frame->frame_appearance.text.font, frame->frame_appearance.text.color, clipper);
    }

    // Draw the image
    if (frame->frame_appearance.image.data != NULL)
    {
        ei_point_t where = get_position_in_parent_from_anchor(*widget->content_rect, frame->frame_appearance.image.rect == NULL ? hw_surface_get_size(frame->frame_appearance.image.data) : frame->frame_appearance.image.rect->size, frame->frame_appearance.image.anchor);

        ei_draw_image(surface, frame->frame_appearance.image.data, frame->frame_appearance.image.rect, &where, clipper);
    }

    // Draw the frame on the offscreen picking surface
    ei_draw_rounded_frame(pick_surface, widget->screen_location, 0, 0, *widget->pick_color, ei_relief_none, clipper);

    // Reduce the size of the clipper to the widget's content rect so that children
    // can't be drawn outside the widget's content rect
    ei_rect_t *children_clipper = malloc(sizeof(ei_rect_t));
    if (clipper != NULL)
    {
        *children_clipper = get_intersection_rectangle(*widget->content_rect, *clipper);
    }
    else
    {
        *children_clipper = *widget->content_rect;
    }

    ei_impl_widget_draw_children(widget, surface, pick_surface, children_clipper);

    free(children_clipper);
}

void frame_setdefaultsfunc(ei_widget_t widget)
{
    ei_frame_t *frame = (ei_frame_t *)widget;

    frame->widget = *widget;
    ei_frame_set_color(frame, (ei_color_t){0, 0, 0, 255});
}

void frame_geomnotifyfunc(ei_widget_t widget)
{
    ei_frame_t *frame = (ei_frame_t *)widget;

    // Compute the content rect of the frame (size of the frame without its border)
    *widget->content_rect = ei_rect_add(widget->screen_location, frame->widget_appearance.border_width, frame->widget_appearance.border_width, -frame->widget_appearance.border_width * 2, -frame->widget_appearance.border_width * 2);
}

ei_size_t ei_frame_get_natural_size(ei_frame_t *frame)
{
    if (&frame->widget == ei_app_root_widget())
    {
        return frame->widget.screen_location.size;
    }

    ei_size_t size = ei_size_zero();

    if (frame->widget_appearance.border_width > 0)
    {
        size.width += frame->widget_appearance.border_width * 2;
        size.height += frame->widget_appearance.border_width * 2;
    }

    if (frame->frame_appearance.text.label != NULL)
    {
        int width = 0;
        int height = 0;
        hw_text_compute_size(frame->frame_appearance.text.label, frame->frame_appearance.text.font, &width, &height);

        size.width += width;
        size.height += height;
    }

    if (frame->frame_appearance.image.data != NULL)
    {
        ei_size_t image_size = frame->frame_appearance.image.rect == NULL ? hw_surface_get_size(frame->frame_appearance.image.data) : frame->frame_appearance.image.rect->size;

        size.width += image_size.width;
        size.height += image_size.height;
    }

    return size;
}