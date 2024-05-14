/**
 * @file    ei_parser_option_value_conversion.h
 *
 * @brief   Defines functions to convert generic pointers to the specitic types needed for the option
 *
 * @authors Seydina BADIANE
 * @authors Christian BOSSE
 * @authors Quentin CHAUVELON
 */

#ifndef EI_PARSER_OPTION_VALUE_CONVERSION_H
#define EI_PARSER_OPTION_VALUE_CONVERSION_H

char *ei_option_value_to_string();

ei_color_t ei_option_value_to_color();

ei_size_t ei_option_value_to_size();

ei_rect_t ei_option_value_to_rect();

ei_anchor_t ei_option_value_to_anchor(void *option_value);

ei_axis_set_t ei_option_value_to_axis_set(void *option_value);

ei_relief_t ei_option_value_to_relief(void *option_value);

#endif