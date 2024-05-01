#include <stdlib.h>
#include <string.h>

#include "../implem/ei_implementation.h"
#include "../api/ei_application.h"
#include "../api/ei_draw.h"
#include "../implem/ei_draw_ext.h"
#include "../implem/ei_button.h"
#include "../api/ei_utils.h"
#include "../api/ei_widget_configure.h"

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
    ei_draw_button((ei_button_t *)widget, surface, clipper);
}

void button_setdefaultsfunc(ei_widget_t widget)
{
    ei_button_t *button = (ei_button_t *)widget;

    button->color = (ei_color_t){0, 0, 0, 255};
    button->border_width = k_default_button_border_width;
    button->corner_radius = k_default_button_corner_radius;
    button->relief = ei_relief_raised;
}