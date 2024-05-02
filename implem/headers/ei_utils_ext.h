/**
 * @file    ei_utils_ext.h
 *
 * @brief   Defines more utility functions
 *          Extends the \ref ei_utils.h file.
 *
 * @see     \ref ei_utils.h
 *
 * @authors Seydina BADIANE
 * @authors Christian BOSSE
 * @authors Quentin CHAUVELON
 */

#ifndef EI_UTILS_EXT_H
#define EI_UTILS_EXT_H

#include "../api/ei_types.h"

/**
 * @brief   Returns the color corresponding to the id
 *          Eg: id = 0 -> {0, 0, 0, 255}, id = 257 -> {0, 1, 1, 255}
 *
 * @param   id  The id to convert to a color
 *
 * @return  The color corresponding to the id
 */
ei_color_t get_color_from_id(int id);

/**
 * @brief   Returns the id corresponding to the color
 *          Eg: {0, 0, 0, 255} -> 0, {0, 1, 1, 255} -> 257
 *
 * @param   color   The color to convert to an id
 *
 * @return  The id corresponding to the color
 */
int get_id_from_color(ei_color_t color);

#endif