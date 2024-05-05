#include <stdlib.h>
#include <string.h>

#include "../api/ei_application.h"
#include "../api/ei_draw.h"
#include "../api/ei_utils.h"
#include "../api/ei_widget_configure.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_button.h"
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

    if (button->frame_appearance.image.rect != NULL)
    {
        free(button->frame_appearance.image.rect);
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
    if (clipper != NULL && !rect_intersects_rect(widget->screen_location, *clipper))
    {
        return;
    }

    DEBUG ? printf("Drawing widget %d\n", widget->pick_id) : 0;

    ei_button_t *button = (ei_button_t *)widget;

    // Use 2 separate functions to draw the button, since we have to force the border width and color
    // for the offscreen picking surface.
    // We could only use one function and always pass the border width and color as parameters,
    // but it would be less intuitive having to pass them while the button already has them.
    ei_draw_visible_button(button, surface, clipper);
    ei_draw_offscreen_button(button, pick_surface, clipper, 0, *widget->pick_color);

    ei_impl_widget_draw_children(widget, surface, pick_surface, clipper);
}

void button_setdefaultsfunc(ei_widget_t widget)
{
    ei_button_t *button = (ei_button_t *)widget;

    button->widget_appearance.color = (ei_color_t){0, 0, 0, 255};
    button->widget_appearance.border_width = k_default_button_border_width;
    button->corner_radius = k_default_button_corner_radius;
    button->frame_appearance.relief = ei_relief_raised;
    button->callback = NULL;
    button->user_param = NULL;
}

void button_geomnotifyfunc(ei_widget_t widget)
{
    // widget->wclass->drawfunc(widget, ei_app_root_surface(), ei_app_offscreen_picking_surface(), NULL);
}