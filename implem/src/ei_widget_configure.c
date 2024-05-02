#include "../api/ei_widget_configure.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_frame.h"
#include "../implem/headers/ei_button.h"

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

void ei_button_configure(ei_widget_t widget,
                         ei_size_t *requested_size,
                         const ei_color_t *color,
                         int *border_width,
                         int *corner_radius,
                         ei_relief_t *relief,
                         ei_string_t *text,
                         ei_font_t *text_font,
                         ei_color_t *text_color,
                         ei_anchor_t *text_anchor,
                         ei_surface_t *img,
                         ei_rect_ptr_t *img_rect,
                         ei_anchor_t *img_anchor,
                         ei_callback_t *callback,
                         ei_user_param_t *user_param)
{
    ei_button_t *button = (ei_button_t *)widget;

    if (requested_size != NULL)
    {
        widget->requested_size = *requested_size;
        widget->screen_location.size = widget->requested_size;
        widget->content_rect = &widget->screen_location;
    }

    if (color != NULL)
    {
        button->color = *color;
    }

    if (border_width != NULL)
    {
        button->border_width = *border_width;
    }

    if (corner_radius != NULL)
    {
        button->corner_radius = *corner_radius;
    }

    if (relief != NULL)
    {
        button->relief = *relief;
    }
}