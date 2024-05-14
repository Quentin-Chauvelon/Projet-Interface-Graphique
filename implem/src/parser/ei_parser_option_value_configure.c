#include <stdio.h>

#include "../api/hw_interface.h"
#include "../api/ei_types.h"
#include "../api/ei_entry.h"
#include "../api/ei_application.h"
#include "../api/ei_placer.h"
#include "../api/ei_widget_configure.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/parser/ei_parser_types.h"
#include "../implem/headers/parser/ei_parser_option_value_conversion.h"

void ei_set_option_value(enum options_action action, ei_widget_t widget, char *option_name, void *option_value, int *cast_tye)
{
    if (action == OPTION_CONFIGURE)
    {
        if (strcmp(widget->wclass->name, "frame") == 0)
        {
            if (strcmp(option_name, "requested_size") == 0)
            {
                ei_size_t requested_size = ei_option_value_to_size();
                ei_frame_configure(widget, &requested_size, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
            }
            else if (strcmp(option_name, "color") == 0)
            {
                ei_color_t color = ei_option_value_to_color();
                ei_frame_configure(widget, NULL, &color, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
            }
            else if (strcmp(option_name, "border_width") == 0)
            {
                ei_frame_configure(widget, NULL, NULL, &(int){*(double *)option_value}, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
            }
            else if (strcmp(option_name, "relief") == 0)
            {
                ei_frame_configure(widget, NULL, NULL, NULL, &(ei_relief_t){ei_option_value_to_relief(option_value)}, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
            }
            else if (strcmp(option_name, "text") == 0)
            {
                if (*cast_tye == 0)
                {
                    ei_frame_configure(widget, NULL, NULL, NULL, NULL, &(char *){option_value}, NULL, NULL, NULL, NULL, NULL, NULL);
                }
                else if (*cast_tye == 1)
                {
                    char *text = ei_option_value_to_string();
                    ei_frame_configure(widget, NULL, NULL, NULL, NULL, &text, NULL, NULL, NULL, NULL, NULL, NULL);
                    free(text);
                }
            }
            else if (strcmp(option_name, "text_font") == 0)
            {
                ei_font_t font = hw_text_font_create(ei_option_value_to_string(), ei_style_normal, 5);
                ei_frame_configure(widget, NULL, NULL, NULL, NULL, NULL, &font, NULL, NULL, NULL, NULL, NULL);
                hw_text_font_free(font);
            }
            else if (strcmp(option_name, "text_color") == 0)
            {
                ei_color_t color = ei_option_value_to_color();
                ei_frame_configure(widget, NULL, NULL, NULL, NULL, NULL, NULL, &color, NULL, NULL, NULL, NULL);
            }
            else if (strcmp(option_name, "text_anchor") == 0)
            {
                ei_frame_configure(widget, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &(ei_anchor_t){ei_option_value_to_anchor(option_value)}, NULL, NULL, NULL);
            }
            else if (strcmp(option_name, "img") == 0)
            {
                ei_surface_t image = hw_image_load((char *)option_value, ei_app_root_surface());
                ei_frame_configure(widget, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &image, NULL, NULL);
            }
            else if (strcmp(option_name, "img_rect") == 0)
            {
                ei_rect_t *img_rect = malloc(sizeof(ei_rect_t));
                *img_rect = ei_option_value_to_rect();
                ei_frame_configure(widget, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &img_rect, NULL);
                free(img_rect);
            }
            else if (strcmp(option_name, "img_anchor") == 0)
            {
                ei_frame_configure(widget, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &(ei_anchor_t){ei_option_value_to_anchor(option_value)});
            }
            else
            {
                printf("\033[0;31mError: Option %s is not configurable for frame\033[0m\n", option_name);
                error = 0;
                return;
            }
        }
        else if (strcmp(widget->wclass->name, "button") == 0)
        {
            if (strcmp(option_name, "requested_size") == 0)
            {
                ei_size_t requested_size = ei_option_value_to_size();
                ei_button_configure(widget, &requested_size, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
            }
            else if (strcmp(option_name, "color") == 0)
            {
                ei_color_t color = ei_option_value_to_color();
                ei_button_configure(widget, NULL, &color, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
            }
            else if (strcmp(option_name, "border_width") == 0)
            {
                ei_button_configure(widget, NULL, NULL, &(int){*(double *)option_value}, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
            }
            else if (strcmp(option_name, "corner_radius") == 0)
            {
                ei_button_configure(widget, NULL, NULL, NULL, &(int){*(double *)option_value}, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
            }
            else if (strcmp(option_name, "relief") == 0)
            {
                ei_button_configure(widget, NULL, NULL, NULL, NULL, &(ei_relief_t){ei_option_value_to_relief(option_value)}, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
            }
            else if (strcmp(option_name, "text") == 0)
            {
                if (*cast_tye == 0)
                {
                    ei_button_configure(widget, NULL, NULL, NULL, NULL, NULL, &(char *){option_value}, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
                }
                else if (*cast_tye == 1)
                {
                    char *text = ei_option_value_to_string();
                    ei_button_configure(widget, NULL, NULL, NULL, NULL, NULL, &text, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
                    free(text);
                }
            }
            else if (strcmp(option_name, "text_font") == 0)
            {
                ei_font_t font = hw_text_font_create(ei_option_value_to_string(), ei_style_normal, 5);
                ei_button_configure(widget, NULL, NULL, NULL, NULL, NULL, NULL, &font, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
                hw_text_font_free(font);
            }
            else if (strcmp(option_name, "text_color") == 0)
            {
                ei_color_t color = ei_option_value_to_color();
                ei_button_configure(widget, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &color, NULL, NULL, NULL, NULL, NULL, NULL);
            }
            else if (strcmp(option_name, "text_anchor") == 0)
            {
                ei_button_configure(widget, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &(ei_anchor_t){ei_option_value_to_anchor(option_value)}, NULL, NULL, NULL, NULL, NULL);
            }
            else if (strcmp(option_name, "img") == 0)
            {
                ei_surface_t image = hw_image_load((char *)option_value, ei_app_root_surface());
                ei_button_configure(widget, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &image, NULL, NULL, NULL, NULL);
            }
            else if (strcmp(option_name, "img_rect") == 0)
            {
                ei_rect_t *img_rect = malloc(sizeof(ei_rect_t));
                *img_rect = ei_option_value_to_rect();
                ei_button_configure(widget, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &img_rect, NULL, NULL, NULL);
                free(img_rect);
            }
            else if (strcmp(option_name, "img_anchor") == 0)
            {
                ei_button_configure(widget, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &(ei_anchor_t){ei_option_value_to_anchor(option_value)}, NULL, NULL);
            }
            else
            {
                printf("\033[0;31mError: Option %s is not configurable for button\033[0m\n", option_name);
                error = 0;
                return;
            }
        }
        else if (strcmp(widget->wclass->name, "toplevel") == 0)
        {
            if (strcmp(option_name, "requested_size") == 0)
            {
                ei_size_t requested_size = ei_option_value_to_size();
                ei_toplevel_configure(widget, &requested_size, NULL, NULL, NULL, NULL, NULL, NULL);
            }
            else if (strcmp(option_name, "color") == 0)
            {
                ei_color_t color = ei_option_value_to_color();
                ei_toplevel_configure(widget, NULL, &color, NULL, NULL, NULL, NULL, NULL);
            }
            else if (strcmp(option_name, "border_width") == 0)
            {
                ei_toplevel_configure(widget, NULL, NULL, &(int){*(double *)option_value}, NULL, NULL, NULL, NULL);
            }
            else if (strcmp(option_name, "title") == 0)
            {
                if (*cast_tye == 0)
                {
                    ei_toplevel_configure(widget, NULL, NULL, NULL, &(char *){option_value}, NULL, NULL, NULL);
                }
                else if (*cast_tye == 1)
                {
                    char *title = ei_option_value_to_string();
                    ei_toplevel_configure(widget, NULL, NULL, NULL, &title, NULL, NULL, NULL);
                    free(title);
                }
            }
            else if (strcmp(option_name, "closable") == 0)
            {
                if (*cast_tye == 1)
                {
                    ei_toplevel_configure(widget, NULL, NULL, NULL, NULL, &(bool){strcmp((char *)option_value, "true") == 0}, NULL, NULL);
                }
                else if (*cast_tye == 2)
                {
                    ei_toplevel_configure(widget, NULL, NULL, NULL, NULL, &(bool){(int){*(double *)option_value}}, NULL, NULL);
                }
            }
            else if (strcmp(option_name, "resizable") == 0)
            {
                ei_toplevel_configure(widget, NULL, NULL, NULL, NULL, NULL, &(ei_axis_set_t){ei_option_value_to_axis_set(option_value)}, NULL);
            }
            else if (strcmp(option_name, "min_size") == 0)
            {
                ei_size_t *min_size = malloc(sizeof(ei_size_t));
                *min_size = ei_option_value_to_size();

                ei_toplevel_configure(widget, NULL, NULL, NULL, NULL, NULL, NULL, &min_size);

                ei_empty_list_number();
                free(min_size);
            }
            else
            {
                printf("\033[0;31mError: Option %s is not configurable for toplevel\033[0m\n", option_name);
                error = 0;
                return;
            }
        }
        else if (strcmp(widget->wclass->name, "entry") == 0)
        {
            if (strcmp(option_name, "requested_char_size") == 0)
            {
                ei_entry_configure(widget, &(int){*(double *)option_value}, NULL, NULL, NULL, NULL);
            }
            else if (strcmp(option_name, "color") == 0)
            {
                ei_color_t color = ei_option_value_to_color();
                ei_entry_configure(widget, NULL, &color, NULL, NULL, NULL);
            }
            else if (strcmp(option_name, "border_width") == 0)
            {
                ei_entry_configure(widget, NULL, NULL, &(int){*(double *)option_value}, NULL, NULL);
            }
            else if (strcmp(option_name, "text_font") == 0)
            {
                ei_font_t font = hw_text_font_create(ei_option_value_to_string(), ei_style_normal, 5);
                ei_entry_configure(widget, NULL, NULL, NULL, &font, NULL);
                hw_text_font_free(font);
            }
            else if (strcmp(option_name, "text_color") == 0)
            {
                ei_color_t color = ei_option_value_to_color();
                ei_entry_configure(widget, NULL, NULL, NULL, NULL, &color);
            }
            else
            {
                printf("\033[0;31mError: Option %s is not configurable for entry\033[0m\n", option_name);
                error = 0;
                return;
            }
        }
    }
    else if (action == OPTION_PLACE)
    {
        if (strcmp(option_name, "anchor") == 0)
        {
            ei_place(widget, &(ei_anchor_t){ei_option_value_to_anchor(option_value)}, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        }
        else if (strcmp(option_name, "x") == 0)
        {
            ei_place(widget, NULL, &(int){*(double *)option_value}, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        }
        else if (strcmp(option_name, "y") == 0)
        {
            ei_place(widget, NULL, NULL, &(int){*(double *)option_value}, NULL, NULL, NULL, NULL, NULL, NULL);
        }
        else if (strcmp(option_name, "width") == 0)
        {
            ei_place(widget, NULL, NULL, NULL, &(int){*(double *)option_value}, NULL, NULL, NULL, NULL, NULL);
        }
        else if (strcmp(option_name, "height") == 0)
        {
            ei_place(widget, NULL, NULL, NULL, NULL, &(int){*(double *)option_value}, NULL, NULL, NULL, NULL);
        }
        else if (strcmp(option_name, "rel_x") == 0)
        {
            ei_place(widget, NULL, NULL, NULL, NULL, NULL, &(float){*(double *)option_value}, NULL, NULL, NULL);
        }
        else if (strcmp(option_name, "rel_y") == 0)
        {
            ei_place(widget, NULL, NULL, NULL, NULL, NULL, NULL, &(float){*(double *)option_value}, NULL, NULL);
        }
        else if (strcmp(option_name, "rel_width") == 0)
        {
            ei_place(widget, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &(float){*(double *)option_value}, NULL);
        }
        else if (strcmp(option_name, "rel_height") == 0)
        {
            ei_place(widget, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &(float){*(double *)option_value});
        }
        else
        {
            printf("\033[0;31mError: Option %s is not a valid placer option\033[0m\n", option_name);
            error = 0;
            return;
        }
    }
}