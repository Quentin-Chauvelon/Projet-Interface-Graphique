#include <stdlib.h>
#include <string.h>

#include "../api/ei_application.h"
#include "../api/ei_draw.h"
#include "../api/ei_utils.h"
#include "../api/ei_widget_configure.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_button.h"
#include "../implem/headers/ei_draw_ext.h"

ei_widget_t button_allocfunc()
{
    ei_button_t *button = malloc(sizeof(ei_button_t));
    memset(button, 0, sizeof(ei_button_t));

    return (ei_widget_t)button;
}

void button_releasefunc(ei_widget_t widget)
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

    ei_button_t *button = (ei_button_t *)widget;
    // free(&button->color);
    // free(&button->border_width);
    // free(&button->corner_radius);
    // free(&button->relief);
}

void button_drawfunc(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    ei_button_t *button = (ei_button_t *)widget;

    // Use 2 separate functions to draw the button, since we have to force the border width and color
    // for the offscreen picking surface.
    // We could only use one function and always pass the border width and color as parameters,
    // but it would be less intuitive having to pass them while the button already has them.
    ei_draw_visible_button(button, surface, clipper);
    ei_draw_offscreen_button(button, pick_surface, clipper, 0, *widget->pick_color);
}

void button_setdefaultsfunc(ei_widget_t widget)
{
    ei_button_t *button = (ei_button_t *)widget;

    button->color = (ei_color_t){0, 0, 0, 255};
    button->border_width = k_default_button_border_width;
    button->corner_radius = k_default_button_corner_radius;
    button->relief = ei_relief_raised;
}