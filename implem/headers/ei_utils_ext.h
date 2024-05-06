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

// If the area of the intersection of two rectangles over the area of both rectangles
// is greater than this value, merge the two rectangles (20% seems low but if you draw
// the rectangles, you will see that the merged rectangle seems to be the right choice)
static const int RECTANGLES_MERGE_THRESHOLD = 20;

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

/**
 * @brief   Returns if the two given rectangles intersect
 *
 * @param   rect1   The first rectangle
 * @param   rect2   The second rectangle
 *
 * @return  True if the rectangles intersect, false otherwise
 */
bool rect_intersects_rect(ei_rect_t rect1, ei_rect_t rect2);

/**
 * @brief   Returns if the first rectangle is included in the second rectangle
 *
 * @param   rect1   The first rectangle
 * @param   rect2   The second rectangle
 *
 * @return  True if the first rectangle is included in the second rectangle, false otherwise
 */
bool rect_included_in_rect(ei_rect_t rect1, ei_rect_t rect2);

/**
 * @brief   Returns the rectangle representing the intersection of the two given rectangles
 *
 * @param   rect1   The first rectangle
 * @param   rect2   The second rectangle
 *
 * @return  The rectangle representing the intersection of the two given rectangles
 */
ei_rect_t get_intersection_rectangle(ei_rect_t rect1, ei_rect_t rect2);

/**
 * @brief   Returns the area of the intersection over the area of the two rectangles as a percentage
 *
 * @param   rect1   The first rectangle
 * @param   rect2   The second rectangle
 *
 * @return  The area of the intersection over the area of the two rectangles as a percentage
 */
int get_intersection_percentage(ei_rect_t rect1, ei_rect_t rect2);

/**
 * @brief   Returns the rectangle representing the merging ot the two given rectangles
 *
 * @param   rect1   The first rectangle
 * @param   rect2   The second rectangle
 *
 * @return  The rectangle representing the merging ot the two given rectangles
 */
ei_rect_t merge_rectangles(ei_rect_t rect1, ei_rect_t rect2);

/**
 * @brief   Compares two sizes for equality
 *
 * @param   size1   The first size
 * @param   size2   The second size
 *
 * @return  Returns true if the sizes are equal, false otherwise
 */
bool equal_sizes(ei_size_t size1, ei_size_t size2);

/**
 * @brief   Returns the position of a child in its parent based on the given anchor
 *
 * @param   parent  The parent rectangle
 * @param   child   The child size
 * @param   anchor  The anchor
 *
 * @return  The position of the child in its parent based on the given anchor
 */
ei_point_t get_position_in_parent_from_anchor(ei_rect_t parent, ei_size_t child, ei_anchor_t anchor);

#endif