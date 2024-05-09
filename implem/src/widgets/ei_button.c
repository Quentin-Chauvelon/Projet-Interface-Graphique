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

ei_widget_t ei_button_allocfunc()
{
    return ei_widget_allocfunc(sizeof(ei_button_t));
}

void ei_button_releasefunc(ei_widget_t widget)
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

void ei_button_drawfunc(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    DEBUG ? printf("Drawing widget %d\n", widget->pick_id) : 0;

    ei_button_t *button = (ei_button_t *)widget;

    // Draw the visible button
    ei_draw_rounded_frame(surface, widget->screen_location, button->widget_appearance.border_width, button->corner_radius, button->widget_appearance.color, button->frame_appearance.relief, clipper);

    // Draw the frame appearance (text and image)
    ei_draw_frame_appearance(surface, widget, button->frame_appearance.text, button->frame_appearance.image, clipper);

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
    *children_clipper = ei_get_children_clipper(*widget->content_rect, clipper);

    ei_impl_widget_draw_children(widget, surface, pick_surface, children_clipper);

    free(children_clipper);
}

void ei_button_setdefaultsfunc(ei_widget_t widget)
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

void ei_button_geomnotifyfunc(ei_widget_t widget)
{
    ei_button_t *button = (ei_button_t *)widget;

    // Compute the content rect of the button (size of the button without its border)
    *widget->content_rect = ei_rect_add(widget->screen_location, button->widget_appearance.border_width, button->widget_appearance.border_width, -button->widget_appearance.border_width * 2, -button->widget_appearance.border_width * 2);
}

ei_size_t ei_button_get_natural_size(ei_button_t *button)
{
    ei_size_t size = ei_size_zero();

    ei_calculate_frame_appearance_natural_size(button->frame_appearance, button->widget_appearance.border_width, &size);

    return size;
}