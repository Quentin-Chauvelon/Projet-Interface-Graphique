#include <stdlib.h>
#include <string.h>

#include "../api/ei_application.h"
#include "../api/ei_draw.h"
#include "../api/ei_utils.h"
#include "../api/ei_widget_configure.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_button.h"
#include "../implem/headers/ei_toplevel.h"
#include "../implem/headers/ei_draw_ext.h"
#include "../implem/headers/ei_application_ext.h"
#include "../implem/headers/ei_utils_ext.h"

ei_widget_t button_allocfunc()
{
    ei_button_t *button = malloc(sizeof(ei_button_t));

    // If malloc failed, exit since the program will crash if the user tries to use the button
    if (button == NULL)
    {
        printf("\033[0;31mError: Couldn't allocate memory for the button widget.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        exit(1);
    }

    memset(button, 0, sizeof(ei_button_t));

    return (ei_widget_t)button;
}

void button_releasefunc(ei_widget_t widget)
{
    ei_button_t *button = (ei_button_t *)widget;

    if (button->frame_appearance.image.data != NULL)
    {
        free(button->frame_appearance.image.data);
    }

    if (button->user_param != NULL)
    {
        free(button->user_param);
    }

    free(button);
    button = NULL;
}

void button_drawfunc(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    DEBUG ? printf("Drawing widget %d\n", widget->pick_id) : 0;

    ei_button_t *button = (ei_button_t *)widget;

    // Draw the visible button
    ei_draw_rounded_frame(surface, widget->screen_location, button->widget_appearance.border_width, button->corner_radius, button->widget_appearance.color, button->frame_appearance.relief, clipper);

    // Draw the text
    if (button->frame_appearance.text.label != NULL)
    {
        int width = 0;
        int height = 0;
        hw_text_compute_size(button->frame_appearance.text.label, button->frame_appearance.text.font, &width, &height);

        ei_point_t where = get_position_in_parent_from_anchor(*widget->content_rect, ei_size(width, height), button->frame_appearance.text.anchor);

        ei_draw_text(surface, &where, button->frame_appearance.text.label, button->frame_appearance.text.font, button->frame_appearance.text.color, clipper);
    }

    // Draw the image
    if (button->frame_appearance.image.data != NULL)
    {
        ei_point_t where = get_position_in_parent_from_anchor(*widget->content_rect, button->frame_appearance.image.rect == NULL ? hw_surface_get_size(button->frame_appearance.image.data) : button->frame_appearance.image.rect->size, button->frame_appearance.image.anchor);

        ei_draw_image(surface, button->frame_appearance.image.data, button->frame_appearance.image.rect, &where, clipper);
    }

    // Only draw the button on the offscreen picking surface if it's not the close button of a toplevel
    if (strcmp(widget->parent->wclass->name, "toplevel") != 0 ||
        ((ei_toplevel_t *)widget->parent)->close_button == NULL ||
        (ei_widget_t *)((ei_toplevel_t *)widget->parent)->close_button != (ei_widget_t *)widget)
    {
        ei_draw_rounded_frame(pick_surface, widget->screen_location, 0, button->corner_radius, *widget->pick_color, ei_relief_none, clipper);
    }

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

void button_setdefaultsfunc(ei_widget_t widget)
{
    ei_button_t *button = (ei_button_t *)widget;

    button->widget = *widget;

    button->widget_appearance.color = ei_default_background_color;
    button->widget_appearance.border_width = k_default_button_border_width;

    button->frame_appearance.relief = ei_relief_raised;
    button->frame_appearance.text.label = NULL;

    button->frame_appearance.text.font = ei_default_font;
    button->frame_appearance.text.color = ei_font_default_color;
    button->frame_appearance.text.anchor = ei_anc_center;

    button->frame_appearance.image.data = NULL;
    button->frame_appearance.image.rect = NULL;
    button->frame_appearance.image.anchor = ei_anc_center;

    button->corner_radius = k_default_button_corner_radius;
    button->callback = NULL;
    button->user_param = NULL;
}

void button_geomnotifyfunc(ei_widget_t widget)
{
    ei_button_t *button = (ei_button_t *)widget;

    // Compute the content rect of the button (size of the button without its border)
    *widget->content_rect = ei_rect_add(widget->screen_location, button->widget_appearance.border_width, button->widget_appearance.border_width, -button->widget_appearance.border_width * 2, -button->widget_appearance.border_width * 2);
}

ei_size_t ei_button_get_natural_size(ei_button_t *button)
{
    ei_size_t size = ei_size_zero();

    if (button->widget_appearance.border_width > 0)
    {
        size.width += button->widget_appearance.border_width * 2;
        size.height += button->widget_appearance.border_width * 2;
    }

    if (button->frame_appearance.text.label != NULL)
    {
        int width = 0;
        int height = 0;
        hw_text_compute_size(button->frame_appearance.text.label, button->frame_appearance.text.font, &width, &height);

        size.width += width;
        size.height += height;
    }

    if (button->frame_appearance.image.data != NULL)
    {
        ei_size_t image_size = button->frame_appearance.image.rect == NULL ? hw_surface_get_size(button->frame_appearance.image.data) : button->frame_appearance.image.rect->size;

        size.width += image_size.width;
        size.height += image_size.height;
    }

    return size;
}