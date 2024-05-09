#include <stdlib.h>
#include <string.h>

#include "../api/ei_application.h"
#include "../api/ei_draw.h"
#include "../api/ei_utils.h"
#include "../api/ei_event.h"
#include "../api/ei_placer.h"
#include "../api/ei_widget_configure.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_toplevel.h"
#include "../implem/headers/ei_button.h"
#include "../implem/headers/ei_draw_ext.h"
#include "../implem/headers/ei_application_ext.h"
#include "../implem/headers/ei_utils_ext.h"

ei_widget_t ei_toplevel_allocfunc()
{
    return ei_widget_allocfunc(sizeof(ei_toplevel_t));
}

void ei_toplevel_releasefunc(ei_widget_t widget)
{
    ei_toplevel_t *toplevel = (ei_toplevel_t *)widget;

    if (toplevel->min_size != NULL)
    {
        free(toplevel->min_size);
    }

    free(toplevel);
    toplevel = NULL;
}

void ei_toplevel_drawfunc(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    DEBUG ? printf("Drawing widget %d\n", widget->pick_id) : 0;

    ei_toplevel_t *toplevel = (ei_toplevel_t *)widget;

    ei_rect_t title_bar = ei_toplevel_get_title_bar_rect(toplevel);

    // Draw the background of the title bar.
    // The title should have rounded corners at the top and straight corners at the bottom.
    // Instead of implementing a new function that only rounds some corners, we can increase
    // the size of the title bar and draw rounded corners on all 4 sides. Then, the drawing
    // of the main rectangle of the toplevel will cover the bottom rounded corners which will
    // give the illusion of only the top corners being rounded
    ei_draw_rounded_frame(surface, ei_rect_add(title_bar, 0, 0, 0, k_default_toplevel_title_corner_radius * 2), 0, k_default_toplevel_title_corner_radius, k_default_toplevel_title_bar_background_color, ei_relief_none, clipper);

    // Resize the title bar to add a left padding for the close button and the text
    title_bar = ei_rect_move(title_bar, k_default_toplevel_title_left_padding, 0, &title_bar);

    // Draw the close button
    if (toplevel->closable)
    {
        toplevel->close_button->widget.geom_params->manager->runfunc((ei_widget_t)toplevel->close_button);
        toplevel->close_button->widget.wclass->drawfunc((ei_widget_t)toplevel->close_button, surface, pick_surface, clipper);

        ei_point_t center = ei_point(toplevel->close_button->widget.screen_location.top_left.x + k_default_toplevel_close_button_size / 2, toplevel->close_button->widget.screen_location.top_left.y + k_default_toplevel_close_button_size / 2);
        ei_draw_circle(surface, center, (k_default_toplevel_close_button_size - toplevel->close_button->widget_appearance.border_width) / 2, toplevel->close_button->widget_appearance.color, clipper);

        // Reduce the size of the title bar to not draw the text over the close button
        title_bar = ei_rect_move(title_bar, k_default_toplevel_close_button_size + k_default_toplevel_title_left_padding, 0, &title_bar);
    }

    int width = 0;
    int height = 0;
    hw_text_compute_size(toplevel->title, ei_default_font, &width, &height);
    ei_point_t where = ei_get_position_in_parent_from_anchor(title_bar, ei_size(width, height), ei_anc_west);

    // Draw the text
    ei_draw_text(surface, &where, toplevel->title, ei_default_font, (ei_color_t){255, 255, 255, 255}, clipper);

    // Draw the content rectangle of the toplevel
    ei_draw_frame(surface, ei_rect_move(toplevel->widget.screen_location, 0, title_bar.size.height, &toplevel->widget.screen_location), toplevel->widget_appearance.border_width, 0, toplevel->widget_appearance.color, ei_relief_none, &k_default_toplevel_title_bar_background_color, clipper);

    // Draw the toplevel on the offscreen picking surface
    // Just like for the title bar, the drawing on the picking offscreen surface needs to
    // only have the top corners rounded, we can apply the same technique as earlier.
    // First, draw the rounded frame and then draw a small rectangle at the bottom to cover
    // the bottom rounded corners
    ei_draw_rounded_frame(pick_surface, widget->screen_location, 0, k_default_toplevel_title_corner_radius, *widget->pick_color, ei_relief_none, clipper);
    ei_draw_rectangle(pick_surface, ei_rect_move(widget->screen_location, 0, widget->screen_location.size.height - k_default_toplevel_title_corner_radius, &widget->screen_location), *widget->pick_color, clipper);

    // Reduce the size of the clipper to the widget's content rect so that children
    // can't be drawn outside the widget's content rect
    ei_rect_t *children_clipper = malloc(sizeof(ei_rect_t));
    *children_clipper = ei_get_children_clipper(*widget->content_rect, clipper);

    ei_impl_widget_draw_children(widget, surface, pick_surface, children_clipper);

    free(children_clipper);

    // Draw the resize square
    if (toplevel->resizable != ei_axis_none)
    {
        ei_rect_t resizable_square = ei_toplevel_get_resize_square_rect(toplevel);

        ei_draw_rectangle(surface, resizable_square, k_default_toplevel_title_bar_background_color, clipper);
        ei_draw_rectangle(pick_surface, resizable_square, *toplevel->widget.pick_color, clipper);
    }
}

void ei_toplevel_setdefaultsfunc(ei_widget_t widget)
{
    ei_toplevel_t *toplevel = (ei_toplevel_t *)widget;

    toplevel->widget = *widget;

    toplevel->widget_appearance.color = ei_default_background_color;
    toplevel->widget_appearance.border_width = 4;

    toplevel->title = (ei_string_t){"Toplevel"};
    toplevel->closable = true;
    toplevel->resizable = ei_axis_both;

    if (toplevel->closable)
    {
        toplevel->close_button = ei_toplevel_instantiate_close_button(toplevel);
    }

    if (toplevel->min_size == NULL)
    {
        toplevel->min_size = malloc(sizeof(ei_size_t));
    }

    *toplevel->min_size = ei_toplevel_get_natural_size(toplevel);
}

void ei_toplevel_geomnotifyfunc(ei_widget_t widget)
{
    ei_toplevel_t *toplevel = (ei_toplevel_t *)widget;

    // Compute the content rect of the toplevel (size of the toplevel without its border and the title bar)
    *widget->content_rect = ei_rect_move(toplevel->widget.screen_location, 0, ei_toplevel_get_title_bar_rect(toplevel).size.height, &toplevel->widget.screen_location);
    *widget->content_rect = ei_rect_add(*widget->content_rect, toplevel->widget_appearance.border_width, 0, -toplevel->widget_appearance.border_width * 2, -toplevel->widget_appearance.border_width);
}

ei_size_t ei_toplevel_get_natural_size(ei_toplevel_t *toplevel)
{
    return toplevel->widget.requested_size;
}

ei_button_t *ei_toplevel_instantiate_close_button(ei_toplevel_t *toplevel)
{
    ei_widget_t close_button = ei_widget_create("button", (ei_widget_t)toplevel, NULL, NULL);

    // Remove the close button from the hierarchy since we have a direct
    // reference from the close_button attribute of the top level.
    // We still need to create the close button with a parent though,
    // otherwise it would segfault when placing it
    toplevel->widget.children_head = close_button->next_sibling;

    ei_button_configure(close_button,
                        &((ei_size_t){k_default_toplevel_close_button_size, k_default_toplevel_close_button_size}),
                        &(ei_color_t){255, 0, 0, 255},
                        NULL,
                        &(int){k_default_toplevel_close_button_size / 2},
                        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                        NULL, NULL);

    int width = 0;
    int height = 0;
    hw_text_compute_size(toplevel->title, ei_default_font, &width, &height);

    ei_place(close_button, NULL,
             &(int){k_default_toplevel_title_left_padding},
             &(int){height / 2 - k_default_toplevel_close_button_size / 2 + 2},
             NULL, NULL, NULL, NULL, NULL, NULL);

    return (ei_button_t *)close_button;
}

ei_rect_t ei_toplevel_get_title_bar_rect(ei_toplevel_t *toplevel)
{
    int width = 0;
    int height = 0;
    hw_text_compute_size(toplevel->title, ei_default_font, &width, &height);

    ei_rect_t title_bar = ei_rect(toplevel->widget.screen_location.top_left, ei_size(toplevel->widget.screen_location.size.width, height));
}

ei_rect_t ei_toplevel_get_resize_square_rect(ei_toplevel_t *toplevel)
{
    return ei_rect(
        ei_point(
            toplevel->widget.screen_location.top_left.x + toplevel->widget.screen_location.size.width - k_default_toplevel_resize_square_size,
            toplevel->widget.screen_location.top_left.y + toplevel->widget.screen_location.size.height - k_default_toplevel_resize_square_size),
        ei_size(
            k_default_toplevel_resize_square_size,
            k_default_toplevel_resize_square_size));
}

void ei_toplevel_get_min_size(ei_widget_t widget, int *width, int *height)
{
    *width = 160;
    *height = 120;
}