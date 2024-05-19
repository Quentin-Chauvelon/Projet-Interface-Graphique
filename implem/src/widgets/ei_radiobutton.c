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

ei_radio_button_t *ei_radio_button_allocfunc()
{
    ei_radio_button_t *radio_button = (ei_radio_button_t *)malloc(sizeof(ei_radio_button_t));

    // If malloc failed, exit since the program will crash if the user tries to use the widget
    if (radio_button == NULL)
    {
        printf("\033[0;31mError: Couldn't allocate memory for the radio_button.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        exit(1);
    }

    memset(radio_button, 0, sizeof(ei_radio_button_t));

    return radio_button;
}

void ei_radio_button_group_releasefunc(ei_widget_t widget)
{
    ei_radio_button_group_t *radio_button_group = (ei_radio_button_group_t *)widget;

    ei_radio_button_t current = radio_button_group->first_radio_button;
    ei_radio_button_t next = NULL;

    while (current != NULL)
    {
        next = current->next_sibling;

        if (current->text.label != NULL)
        {
            free(current->text.label);
        }

        ei_widget_destroy(&current->button->widget);

        free(current);
        current = next;
    }

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

    radio_button_group->first_radio_button = NULL;

    radio_button_group->buttons_color = ei_default_background_color;
    radio_button_group->button_selected_color = k_default_radio_button_selected_color;
}

void ei_radio_button_group_drawfunc(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    DEBUG ? printf("Drawing widget %d\n", widget->pick_id) : 0;

    ei_radio_button_group_t *radio_button_group = (ei_radio_button_group_t *)widget;

    // Draw the visible frame
    ei_draw_straight_frame(surface, radio_button_group->widget.screen_location, radio_button_group->widget_appearance.border_width, radio_button_group->widget_appearance.color, radio_button_group->relief, clipper);

    ei_draw_straight_frame(pick_surface, radio_button_group->widget.screen_location, 0, *widget->pick_color, ei_relief_none, clipper);

    // Margin between the radio button and the top (increases after each radio button is drawn)
    int top_margin = 0;

    ei_radio_button_t current = radio_button_group->first_radio_button;

    while (current != NULL)
    {
        current->button;
        widget->geom_params->manager->runfunc(&current->button->widget);
        current->button->widget.wclass->drawfunc(&current->button->widget, surface, pick_surface, clipper);

        ei_point_t label_top_left = ei_point(
            radio_button_group->widget.content_rect->top_left.x + k_default_radio_button_size + k_default_radio_button_label_spacing,
            radio_button_group->widget.content_rect->top_left.y + top_margin);

        ei_font_t text_font = current->text.font != NULL
                                  ? current->text.font
                                  : radio_button_group->text.font;

        ei_color_t text_color = current->text.color.alpha != 0
                                    ? current->text.color
                                    : radio_button_group->text.color;

        ei_draw_text(surface, &label_top_left, current->text.label, text_font, text_color, clipper);

        int width = 0;
        int height = 0;
        hw_text_compute_size(current->text.label, text_font, &width, &height);

        top_margin += height + k_default_radio_button_group_spacing;

        ei_draw_straight_frame(pick_surface, (ei_rect_t){label_top_left, ei_size(width, height)}, 0, *widget->pick_color, ei_relief_none, clipper);

        current = current->next_sibling;
    }

    // int number = ei_nb_radio_button_group(group) + 1;

    // ei_size_t rec_size=clipper->size;
    // ei_point_t top = clipper->top_left;
    // ei_point_t end =(ei_point_t) {clipper->top_left.x ,clipper->top_left.y +rec_size.height/ number};

    // //We have to add the border_width
    // top.x+=group->window.border_width;
    // top.y+=group->window.border_width;
    // end.x+=group->window.border_width;
    // end.y+=group->window.border_width;

    // while (radio_button != NULL)
    // {
    //     // printf("ici de draw");
    //     // Draw of a button before the radio_button to know if he is selected ot not
    //     ei_color_t color = radio_button->selected == false ? (ei_color_t){0x55, 0x55, 0x55, 0xff} : (ei_color_t){0xff, 0x0, 0x0, 0xff};
    //     int length_rect = (end.y - top.y) / 2;
    //     ei_point_t center = (ei_point_t){top.x + length_rect / 4, top.y + length_rect / 2};
    //     ei_rect_t *cir_rect = &(ei_rect_t){center, {length_rect, length_rect}}; //

    //     radio_button->button = (ei_button_t *)ei_widget_create("button", group, NULL, NULL);
    //     ;
    //     ei_button_t *button = (ei_button_t *)radio_button->button;
    //     ei_button_setdefaultsfunc(button);

    //     ei_button_configure((ei_widget_t)button, NULL,
    //                         &color,
    //                         &(int){2}, NULL,
    //                         &(ei_relief_t){ei_relief_raised},
    //                         NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    //                         &radio_button->callback, NULL);
    //     button->widget.screen_location = *cir_rect;

    //     ei_button_drawfunc((ei_widget_t)button, surface, pick_surface, cir_rect);

    //     // Draw the frame appearance (text)
    //     ei_size_t size = (ei_size_t){rec_size.width - length_rect, rec_size.height / number};
    //     ei_rect_t *rb_clipper = &(ei_rect_t){{top.x + length_rect, top.y}, size};
    //     widget->content_rect = rb_clipper;
    //     ei_draw_frame_appearance(surface, widget, radio_button->text, (ei_image_properties_t)image, rb_clipper);

    //     // Advance to the next radiobuttton
    //     radio_button = radio_button->next_sibling;
    //     top.y += rec_size.height / number;
    //     end.y += rec_size.height / number;
    // }

    // // Draw the frame on the offscreen picking surface
    // ei_draw_rounded_frame(pick_surface, widget->screen_location, 0, 0, *widget->pick_color, ei_relief_none, clipper);
}

// int ei_nb_radio_button_group(ei_radio_button_group_t *group)
// {
//     int compt= (int) 0 ;
//     //printf("ici de nb\n");
//     ei_radio_button_t *run = (ei_radio_button_t *)group->radio_button;
//     while ( run !=NULL)
//     {
//         //printf("ici de nb\n");
//         compt++;
//         run=run->next_sibling;
//     }
//     return compt;
// }

bool ei_check_change_radio_button_state(ei_radio_button_t radio_button, bool selected)
{
    //     ei_radio_button_t *part1 = radio_button->previous_sibling;
    //     ei_radio_button_t *part2 = radio_button->next_sibling;
    //     bool end=false;//To avoid to advance uselessly in the chaine

    //     //Checking and changing
    //     if (selected)
    //     {
    //         //Change of the current one state
    //         radio_button->selected = selected;

    //         // We consider there are only one activated radio_button

    //         //First part of the chain
    //         while (part1!=NULL)
    //         {
    //             if (part1->selected)
    //             {
    //                 part1->selected = false;
    //                 end=true;
    //                 break;
    //             }
    //             part1=part1->previous_sibling;
    //         }

    //         //Second part of the chain
    //         while (!end && part2!=NULL)
    //         {
    //             if (part2->selected)
    //             {
    //                 part2->selected = false;
    //                 break;
    //             }
    //             part2=part2->next_sibling;
    //         }
    //     }

    //     return true;
}

void ei_radio_button_group_geomnotifyfunc(ei_widget_t widget)
{
    ei_radio_button_group_t *radio_button_group = (ei_radio_button_group_t *)widget;

    int border_width = radio_button_group->widget_appearance.border_width;

    // Compute the content rect of the radio button group (size of the group without its border)
    *widget->content_rect = ei_rect_add(widget->screen_location, border_width, border_width, -border_width * 2, -border_width * 2);
    *widget->content_rect = ei_rect_add(widget->screen_location, k_default_radio_button_group_padding, k_default_radio_button_group_padding, -k_default_radio_button_group_padding * 2, -k_default_radio_button_group_padding * 2);

    ei_radio_button_t current = radio_button_group->first_radio_button;

    while (current != NULL)
    {
        ei_update_radio_button_position(current);

        current = current->next_sibling;
    }
}

ei_size_t ei_radio_button_group_get_natural_size(ei_radio_button_group_t *radio_button_group)
{
    int max_width = 0;
    int total_height = k_default_radio_button_group_padding * 2 + radio_button_group->widget_appearance.border_width * 2;

    ei_radio_button_t current = radio_button_group->first_radio_button;

    // Find the largest width out of all the radio buttons + update the height to include all radio buttons
    while (current != NULL)
    {
        ei_font_t text_font = current->text.font != NULL
                                  ? current->text.font
                                  : radio_button_group->text.font;

        int width = 0;
        int height = 0;
        hw_text_compute_size(current->text.label, text_font, &width, &height);

        width += k_default_radio_button_size + k_default_radio_button_label_spacing;

        if (width > max_width)
        {
            max_width = width;
        }

        total_height += height + k_default_radio_button_group_spacing;

        current = current->next_sibling;
    }

    // Add the padding and border to the maximum width
    max_width += k_default_radio_button_group_padding * 2 + radio_button_group->widget_appearance.border_width * 2;

    // Remove the last spacing from the total height
    total_height -= k_default_radio_button_group_spacing;

    return ei_size(max_width, total_height);
}

ei_radio_button_t ei_radio_button_create(ei_string_t text)
{
    ei_radio_button_t radio_button = malloc(sizeof(ei_impl_radio_button_t));

    radio_button->text.label = malloc(strlen(text) + 1);
    strcpy(radio_button->text.label, text);

    // Set the default values to NULL, so that the group's values are used, except once
    // those values are set for the radio_button by the user
    radio_button->text.font = NULL;
    radio_button->text.color = (ei_color_t){222, 76, 138, 0}; // Random transparent color since it can't be NULL
    radio_button->text.anchor = ei_anc_center;
    radio_button->button_selected_color = (ei_color_t){222, 76, 138, 0}; // Random transparent color since it can't be NULL

    radio_button->previous_sibling = NULL;
    radio_button->next_sibling = NULL;

    ei_widget_t button = ei_widget_create_internal("button", NULL, NULL, NULL);

    ei_button_configure(button,
                        &((ei_size_t){k_default_radio_button_size, k_default_radio_button_size}),
                        &ei_default_background_color,
                        &k_default_radio_button_border_width,
                        &(int){0},
                        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    radio_button->button = (ei_button_t *)button;

    radio_button->group = NULL;
}

void ei_add_radio_button_to_group(ei_widget_t group, ei_radio_button_t radio_button)
{
    ei_radio_button_group_t *radio_button_group = (ei_radio_button_group_t *)group;

    ei_radio_button_t current = radio_button_group->first_radio_button;

    if (current == NULL)
    {
        radio_button_group->first_radio_button = radio_button;
    }
    else
    {
        while (current->next_sibling != NULL)
        {
            current = current->next_sibling;
        }

        current->next_sibling = radio_button;
        radio_button->previous_sibling = current;
    }

    radio_button->group = radio_button_group;

    ei_size_t radio_button_group_size = ei_radio_button_group_get_natural_size(radio_button_group);

    // Update the radio button group size
    radio_button->group->widget.requested_size = radio_button_group_size;

    ei_update_radio_button_position(radio_button);

    if (ei_widget_is_displayed(&radio_button->group->widget))
    {
        radio_button->group->widget.geom_params->manager->runfunc(&radio_button->group->widget);
    }
}

void ei_update_radio_button_position(ei_radio_button_t radio_button)
{

    // If the radio button is not in a group yet, return
    if (radio_button->group == NULL)
    {
        return;
    }

    ei_radio_button_t current = radio_button->group->first_radio_button;

    int top = 0;

    while (true)
    {
        if (current == NULL)
        {
            break;
        }

        ei_font_t text_font = current->text.font != NULL
                                  ? current->text.font
                                  : radio_button->group->text.font;

        int width = 0;
        int height = 0;
        hw_text_compute_size(current->text.label, text_font, &width, &height);

        // If we are at the radio button we are looking for, update the position of the button
        // based on the size of its label
        if (current == radio_button)
        {
            ei_widget_t button = (ei_widget_t)current->button;

            int button_x = radio_button->group->widget.content_rect->top_left.x;
            int button_y = radio_button->group->widget.content_rect->top_left.y + top + height / 2 - k_default_radio_button_size / 2;

            ei_place(button, NULL, &button_x, &button_y, NULL, NULL, NULL, NULL, NULL, NULL);

            break;
        }
        // Otherwise, update the top position to include the height + spacing of the current radio button
        else
        {
            top += height + k_default_radio_button_group_spacing;
        }

        current = current->next_sibling;
    }
}