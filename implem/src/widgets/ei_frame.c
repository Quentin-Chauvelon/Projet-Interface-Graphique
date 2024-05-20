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

ei_widget_t ei_frame_allocfunc()
{
    return ei_widget_allocfunc(sizeof(ei_frame_t));
}

void ei_frame_releasefunc(ei_widget_t widget)
{
    ei_frame_t *frame = (ei_frame_t *)widget;

    if (frame->frame_appearance.text.label != NULL)
    {
        free(frame->frame_appearance.text.label);
    }

    if (frame->frame_appearance.image.data != NULL)
    {
        hw_surface_free(frame->frame_appearance.image.data);
    }

    if (frame->frame_appearance.image.rect != NULL)
    {
        free(frame->frame_appearance.image.rect);
    }

    free(frame);
    frame = NULL;
}

void ei_frame_drawfunc(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    DEBUG ? printf("Drawing widget %d\n", widget->pick_id) : 0;

    ei_frame_t *frame = (ei_frame_t *)widget;

    // Draw the visible frame
    ei_draw_straight_frame(surface, widget->screen_location, frame->widget_appearance.border_width, frame->widget_appearance.color, frame->frame_appearance.relief, clipper);

    // Draw the frame appearance (text and image)
    ei_draw_frame_appearance(surface, widget, frame->frame_appearance.text, frame->frame_appearance.image, clipper);

    // Draw the frame on the offscreen picking surface
    ei_draw_rounded_frame(pick_surface, widget->screen_location, 0, 0, *widget->pick_color, ei_relief_none, clipper);

    ei_widget_drawfunc_finalize(widget, surface, pick_surface, clipper);
}

void ei_frame_setdefaultsfunc(ei_widget_t widget)
{
    ei_frame_t *frame = (ei_frame_t *)widget;

    frame->widget = *widget;

    frame->widget_appearance.color = ei_default_background_color;
    frame->widget_appearance.border_width = 0;

    frame->frame_appearance.relief = ei_relief_none;
    frame->frame_appearance.text.label = NULL;

    frame->frame_appearance.text.font = ei_default_font;
    frame->frame_appearance.text.color = ei_font_default_color;
    frame->frame_appearance.text.anchor = ei_anc_center;

    frame->frame_appearance.image.data = NULL;
    frame->frame_appearance.image.rect = NULL;
    frame->frame_appearance.image.anchor = ei_anc_center;
}

void ei_frame_geomnotifyfunc(ei_widget_t widget)
{
    ei_frame_t *frame = (ei_frame_t *)widget;

    int border_width = frame->widget_appearance.border_width;

    // Compute the content rect of the frame (size of the frame without its border)
    *widget->content_rect = ei_rect_add(widget->screen_location, border_width, border_width, -border_width * 2, -border_width * 2);
}

ei_size_t ei_frame_get_natural_size(ei_frame_t *frame)
{
    if (&frame->widget == ei_app_root_widget())
    {
        return frame->widget.screen_location.size;
    }

    ei_size_t size = ei_size_zero();

    ei_calculate_frame_appearance_natural_size(frame->frame_appearance, frame->widget_appearance.border_width, &size);

    return size;
}