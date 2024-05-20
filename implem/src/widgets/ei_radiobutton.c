#include <stdlib.h>
#include <string.h>

#include "../api/ei_application.h"
#include "../api/ei_draw.h"
#include "../api/ei_utils.h"
#include "../api/ei_placer.h"
#include "../api/ei_types.h"
#include "../api/ei_widget_configure.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_button.h"
#include "../implem/headers/ei_radiobutton.h"
#include "../implem/headers/api/ei_radio_button.h"
#include "../implem/headers/ei_draw_ext.h"
#include "../implem/headers/ei_application_ext.h"
#include "../implem/headers/ei_utils_ext.h"

ei_widget_t ei_radio_button_group_allocfunc()
{
    return ei_widget_allocfunc(sizeof(ei_radio_button_group_t));
}

void ei_radio_button_group_releasefunc(ei_widget_t widget)
{
    ei_radio_button_group_t *radio_button_group = (ei_radio_button_group_t *)widget;

    free(radio_button_group);
}

void ei_radio_button_group_setdefaultsfunc(ei_widget_t widget)
{
    ei_radio_button_group_t *radio_button_group = (ei_radio_button_group_t *)widget;

    radio_button_group->widget = *widget;

    radio_button_group->widget_appearance.color = ei_default_background_color;
    radio_button_group->widget_appearance.border_width = 0;

    radio_button_group->relief = ei_relief_none;

    radio_button_group->text.label = NULL;
    radio_button_group->text.font = ei_default_font;
    radio_button_group->text.color = ei_font_default_color;
    radio_button_group->text.anchor = ei_anc_center;

    radio_button_group->buttons_color = k_default_radio_button_color;
    radio_button_group->buttons_selected_color = k_default_radio_button_selected_color;
}

void ei_radio_button_group_drawfunc(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    DEBUG ? printf("Drawing widget %d\n", widget->pick_id) : 0;

    ei_radio_button_group_t *radio_button_group = (ei_radio_button_group_t *)widget;

    // Draw the visible frame
    ei_draw_straight_frame(surface, radio_button_group->widget.screen_location, radio_button_group->widget_appearance.border_width, radio_button_group->widget_appearance.color, radio_button_group->relief, clipper);

    ei_draw_straight_frame(pick_surface, radio_button_group->widget.screen_location, 0, *widget->pick_color, ei_relief_none, clipper);

    ei_widget_drawfunc_finalize(widget, surface, pick_surface, clipper);
}

void ei_radio_button_group_geomnotifyfunc(ei_widget_t widget)
{
    ei_radio_button_group_t *radio_button_group = (ei_radio_button_group_t *)widget;

    int border_width = radio_button_group->widget_appearance.border_width;

    // Compute the content rect of the radio button group (size of the group without its border)
    *widget->content_rect = ei_rect_add(widget->screen_location, border_width, border_width, -border_width * 2, -border_width * 2);
    *widget->content_rect = ei_rect_add(widget->screen_location, k_default_radio_button_group_padding, k_default_radio_button_group_padding, -k_default_radio_button_group_padding * 2, -k_default_radio_button_group_padding * 2);

    ei_radio_buttons_update_position(radio_button_group);
}

ei_size_t ei_radio_button_group_get_natural_size(ei_radio_button_group_t *radio_button_group)
{
    int max_width = 0;
    int total_height = k_default_radio_button_group_padding * 2 + radio_button_group->widget_appearance.border_width * 2;
    int number_of_radio_buttons = 0;

    if (radio_button_group->widget.children_head == NULL)
    {
        return ei_size_zero();
    }

    // Find the largest of its children + calculate the total height of all the children
    for (ei_widget_t current = radio_button_group->widget.children_head; current != NULL; current = current->next_sibling)
    {
        ei_radio_button_t *radio_button = (ei_radio_button_t *)current;

        ei_font_t text_font = radio_button->text.font != NULL
                                  ? radio_button->text.font
                                  : radio_button_group->text.font;

        int width = 0;
        int height = 0;
        hw_text_compute_size(radio_button->text.label, text_font, &width, &height);

        if (width > max_width)
        {
            max_width = width;
        }

        total_height += height;

        number_of_radio_buttons++;
    }

    // Add the button, spacing, padding and border to the maximum width
    max_width += k_default_radio_button_size + k_default_radio_button_label_spacing + k_default_radio_button_group_padding * 2 + radio_button_group->widget_appearance.border_width * 2;

    // Add the spacing between the radio buttons to the total height
    total_height += k_default_radio_button_group_spacing * (number_of_radio_buttons - 1);

    return ei_size(max_width, total_height);
}

ei_widget_t ei_radio_button_allocfunc()
{
    ei_widget_t radio_button = ei_widget_allocfunc(sizeof(ei_radio_button_t));

    return radio_button;
}

void ei_radio_button_releasefunc(ei_widget_t widget)
{
    ei_radio_button_t *radio_button = (ei_radio_button_t *)widget;

    if (radio_button->text.label != NULL)
    {
        free(radio_button->text.label);
    }

    free(radio_button);
}

void ei_radio_button_setdefaultsfunc(ei_widget_t widget)
{
    ei_radio_button_t *radio_button = (ei_radio_button_t *)widget;

    radio_button->widget = *widget;

    // Set the default values to NULL (or a random transparent color since colors can't be
    // as they are not pointers), so that the group's values are used, except once
    // those values are set for that radio_button by the user
    radio_button->text.label = NULL;
    radio_button->text.font = NULL;
    radio_button->text.color = k_default_random_radio_button_color;
    radio_button->text.anchor = ei_anc_center;

    radio_button->button_color = k_default_random_radio_button_color;
    radio_button->button_selected_color = k_default_random_radio_button_color;

    radio_button->selected = false;

    radio_button->widget.parent->requested_size = ei_radio_button_group_get_natural_size((ei_radio_button_group_t *)radio_button->widget.parent);

    ei_radio_buttons_update_position((ei_radio_button_group_t *)radio_button->widget.parent);
}

ei_color_t ei_get_radio_button_color(ei_radio_button_t *radio_button, bool *selected)
{
    if (selected == NULL)
    {
        selected = &radio_button->selected;
    }

    // If the radio button is selected or the selected parameter is true, return the selected button color
    if (*selected)
    {
        // If the radio button color has the same components as the random color, then use
        // the group's color (this is because these colors arent's pointer and thus can't
        // be NULL, so using a random transparent color seemed like the best fix)
        if (radio_button->button_color.red == k_default_random_radio_button_color.red &&
            radio_button->button_color.green == k_default_random_radio_button_color.green &&
            radio_button->button_color.blue == k_default_random_radio_button_color.blue &&
            radio_button->button_color.alpha == k_default_random_radio_button_color.alpha)
        {
            return ((ei_radio_button_group_t *)radio_button->widget.parent)->buttons_selected_color;
        }
        // Otherwise, use the radio button color
        else
        {
            return radio_button->button_selected_color;
        }
    }
    // Otherwise return the unselected button color
    else
    {
        if (radio_button->button_color.red == k_default_random_radio_button_color.red &&
            radio_button->button_color.green == k_default_random_radio_button_color.green &&
            radio_button->button_color.blue == k_default_random_radio_button_color.blue &&
            radio_button->button_color.alpha == k_default_random_radio_button_color.alpha)
        {
            return ((ei_radio_button_group_t *)radio_button->widget.parent)->buttons_color;
        }
        else
        {
            return radio_button->button_color;
        }
    }
}

void ei_radio_button_drawfunc(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    DEBUG ? printf("Drawing widget %d\n", widget->pick_id) : 0;

    ei_radio_button_t *radio_button = (ei_radio_button_t *)widget;

    ei_point_t where = ei_point(
        radio_button->widget.screen_location.top_left.x + k_default_radio_button_size + k_default_radio_button_label_spacing,
        radio_button->widget.screen_location.top_left.y);

    ei_font_t text_font = radio_button->text.font != NULL
                              ? radio_button->text.font
                              : ((ei_radio_button_group_t *)widget->parent)->text.font;

    ei_color_t text_color;

    // If the radio button text color has the same components as the random color, then use
    // the group's color (this is because these colors arent's pointer and thus can't
    // be NULL, so using a random transparent color seemed like the best fix)
    if (radio_button->text.color.red == k_default_random_radio_button_color.red &&
        radio_button->text.color.green == k_default_random_radio_button_color.green &&
        radio_button->text.color.blue == k_default_random_radio_button_color.blue &&
        radio_button->text.color.alpha == k_default_random_radio_button_color.alpha)
    {
        text_color = ((ei_radio_button_group_t *)radio_button->widget.parent)->text.color;
    }
    // Otherwise, use the radio button text color
    else
    {
        text_color = radio_button->text.color;
    }

    // Draw the text
    ei_draw_text(surface, &where, radio_button->text.label, text_font, text_color, clipper);

    int width = 0;
    int height = 0;
    hw_text_compute_size(radio_button->text.label, text_font, &width, &height);

    ei_rect_t text_rect = ei_rect(
        where,
        ei_size(width, height));

    // Draw the rect that includes the text on the offscreen picking surface
    ei_draw_straight_frame(pick_surface, text_rect, 0, *widget->pick_color, ei_relief_none, clipper);

    ei_rect_t button_rect = ei_rect(
        ei_point(
            radio_button->widget.screen_location.top_left.x,
            radio_button->widget.screen_location.top_left.y + height / 2 - k_default_radio_button_size / 2 + 1),
        ei_size(k_default_radio_button_size, k_default_radio_button_size));

    ei_color_t unselected_button_color = ei_get_radio_button_color(radio_button, &(bool){false});

    ei_color_t top_color = radio_button->selected
                               ? ei_lighten_color(unselected_button_color)
                               : ei_darken_color(unselected_button_color);

    ei_color_t bottom_color = radio_button->selected
                                  ? ei_darken_color(unselected_button_color)
                                  : ei_lighten_color(unselected_button_color);

    // Draw the border of the button
    ei_draw_diamond(surface, button_rect, top_color, ei_rounded_frame_top, clipper);
    ei_draw_diamond(surface, button_rect, bottom_color, ei_rounded_frame_bottom, clipper);

    ei_color_t button_color = ei_get_radio_button_color(radio_button, NULL);

    ei_draw_diamond(pick_surface, button_rect, *widget->pick_color, ei_rounded_frame_full, clipper);

    // Draw the center of the button
    button_rect = ei_rect_add(button_rect, k_default_radio_button_border_width, k_default_radio_button_border_width, -k_default_radio_button_border_width * 2, -k_default_radio_button_border_width * 2);
    ei_draw_diamond(surface, button_rect, button_color, ei_rounded_frame_full, clipper);

    ei_widget_drawfunc_finalize(widget, surface, pick_surface, clipper);
}

void ei_radio_button_geomnotifyfunc(ei_widget_t widget)
{
    ei_radio_button_t *radio_button = (ei_radio_button_t *)widget;

    *widget->content_rect = widget->screen_location;
}

void select_radio_button(ei_widget_t widget)
{
    ei_radio_button_t *radio_button = (ei_radio_button_t *)widget;
    ei_radio_button_group_t *radio_button_group = (ei_radio_button_group_t *)widget->parent;

    for (ei_widget_t current = radio_button_group->widget.children_head; current != NULL; current = current->next_sibling)
    {
        ei_radio_button_t *current_radio_button = (ei_radio_button_t *)current;

        // Unselect any selected radio button
        current_radio_button->selected = false;

        // Select the given radio button
        if (current_radio_button == radio_button)
        {
            current_radio_button->selected = true;
        }
    }
}

bool is_radio_button_selected(ei_widget_t widget)
{
    return ((ei_radio_button_t *)widget)->selected;
}

ei_widget_t get_selected_radio_button(ei_widget_t widget)
{
    ei_radio_button_group_t *radio_button_group = (ei_radio_button_group_t *)widget;

    for (ei_widget_t current = radio_button_group->widget.children_head; current != NULL; current = current->next_sibling)
    {
        if (is_radio_button_selected(current))
        {
            return current;
        }
    }
}

ei_string_t get_selected_radio_button_text(ei_widget_t widget)
{
    return ((ei_radio_button_t *)get_selected_radio_button(widget))->text.label;
}

void clear_selection(ei_widget_t widget)
{
    ei_radio_button_group_t *radio_button_group = (ei_radio_button_group_t *)widget;

    for (ei_widget_t current = radio_button_group->widget.children_head; current != NULL; current = current->next_sibling)
    {
        ((ei_radio_button_t *)current)->selected = false;
    }
}

void ei_radio_buttons_update_position(ei_radio_button_group_t *radio_button_group)
{
    if (radio_button_group->widget.children_head == NULL)
    {
        return;
    }

    int top_left_y = 0;

    for (ei_widget_t current = radio_button_group->widget.children_head; current != NULL; current = current->next_sibling)
    {
        ei_radio_button_t *radio_button = (ei_radio_button_t *)current;

        ei_place(current, NULL, NULL, &top_left_y, NULL, NULL, NULL, NULL, NULL, NULL);

        ei_font_t text_font = radio_button->text.font != NULL
                                  ? radio_button->text.font
                                  : radio_button_group->text.font;

        int width = 0;
        int height = 0;

        if (radio_button->text.label != NULL)
        {
            hw_text_compute_size(radio_button->text.label, text_font, &width, &height);
        }

        top_left_y += height + k_default_radio_button_group_spacing;
    }

    if (ei_widget_is_displayed(&radio_button_group->widget))
    {
        radio_button_group->widget.geom_params->manager->runfunc(&radio_button_group->widget);
    }
}