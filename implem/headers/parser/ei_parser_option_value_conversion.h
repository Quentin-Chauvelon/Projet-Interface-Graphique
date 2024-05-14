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

/**
 * @brief   Convert a list of strings ( \ref ei_list_name_t ) to a single string.
 *
 * @return  The string converted from the list of strings
 */
char *ei_option_value_to_string();

/**
 * @brief   Convert a list of numbers ( \ref ei_list_number_t ) to a color.
 *          The list must contain exactly 4 numbers or it will return a default color
 *
 * @return  The color converted from the list of numbers
 */
ei_color_t ei_option_value_to_color();

/**
 * @brief   Convert a list of numbers ( \ref ei_list_number_t ) to a size.
 *          The list must contain exactly 2 numbers or it will return a default size
 *
 * @return  The size converted from the list of numbers
 */
ei_size_t ei_option_value_to_size();

/**
 * @brief   Convert a list of numbers ( \ref ei_list_number_t ) to a rect.
 *          The list must contain exactly 4 numbers or it will return a default rect
 *
 * @return  The rect converted from the list of numbers
 */
ei_rect_t ei_option_value_to_rect();

/**
 * @brief   Convert the given generic pointer to an \ref ei_anchor_t
 *
 * @param option_value  The generic pointer to convert to an anchor
 *
 * @return  The anchor converted from the generic pointer
 */
ei_anchor_t ei_option_value_to_anchor(void *option_value);

/**
 * @brief   Convert the given generic pointer to an \ref ei_axis_set_t
 *
 * @param option_value  The generic pointer to convert to an axis
 *
 * @return  The axis converted from the generic pointer
 */
ei_axis_set_t ei_option_value_to_axis_set(void *option_value);

/**
 * @brief   Convert the given generic pointer to an \ref ei_relief_t
 *
 * @param option_value  The generic pointer to convert to a relief
 *
 * @return  The relief converted from the generic pointer
 */
ei_relief_t ei_option_value_to_relief(void *option_value);

#endif