#include "../api/ei_widget_configure.h"
#include "../implem/ei_implementation.h"
#include "../implem/ei_frame.h"

void ei_frame_configure(ei_widget_t widget,
                        ei_size_t *requested_size,
                        const ei_color_t *color,
                        int *border_width,
                        ei_relief_t *relief,
                        ei_string_t *text,
                        ei_font_t *text_font,
                        ei_color_t *text_color,
                        ei_anchor_t *text_anchor,
                        ei_surface_t *img,
                        ei_rect_ptr_t *img_rect,
                        ei_anchor_t *img_anchor)
{
    ei_frame_t *frame = (ei_frame_t *)widget;

    if (requested_size != NULL)
    {
        widget->requested_size = *requested_size;
        widget->screen_location.size = widget->requested_size;
        widget->content_rect = &widget->screen_location;
    }

    if (color != NULL)
    {
        frame->color = *color;
    }
}