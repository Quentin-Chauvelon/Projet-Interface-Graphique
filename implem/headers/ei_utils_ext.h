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
#include "../implem/headers/ei_types_ext.h"

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
ei_color_t ei_get_color_from_id(int id);

/**
 * @brief   Returns the id corresponding to the color
 *          Eg: {0, 0, 0, 255} -> 0, {0, 1, 1, 255} -> 257
 *
 * @param   color   The color to convert to an id
 *
 * @return  The id corresponding to the color
 */
int ei_get_id_from_color(ei_color_t color);

/**
 * @brief   Returns if the two given rectangles intersect
 *
 * @param   rect1   The first rectangle
 * @param   rect2   The second rectangle
 *
 * @return  True if the rectangles intersect, false otherwise
 */
bool ei_rect_intersects_rect(ei_rect_t rect1, ei_rect_t rect2);

/**
 * @brief   Returns if the first rectangle is included in the second rectangle
 *
 * @param   rect1   The first rectangle
 * @param   rect2   The second rectangle
 *
 * @return  True if the first rectangle is included in the second rectangle, false otherwise
 */
bool ei_rect_included_in_rect(ei_rect_t rect1, ei_rect_t rect2);

/**
 * @brief   Returns the rectangle representing the intersection of the two given rectangles
 *
 * @param   rect1   The first rectangle
 * @param   rect2   The second rectangle
 *
 * @return  The rectangle representing the intersection of the two given rectangles
 */
ei_rect_t ei_get_intersection_rectangle(ei_rect_t rect1, ei_rect_t rect2);

/**
 * @brief   Returns the area of the intersection over the area of the two rectangles as a percentage
 *
 * @param   rect1   The first rectangle
 * @param   rect2   The second rectangle
 *
 * @return  The area of the intersection over the area of the two rectangles as a percentage
 */
int ei_get_intersection_percentage(ei_rect_t rect1, ei_rect_t rect2);

/**
 * @brief   Returns the rectangle representing the merging ot the two given rectangles
 *
 * @param   rect1   The first rectangle
 * @param   rect2   The second rectangle
 *
 * @return  The rectangle representing the merging ot the two given rectangles
 */
ei_rect_t ei_merge_rectangles(ei_rect_t rect1, ei_rect_t rect2);

/**
 * @brief   Compares two sizes for equality
 *
 * @param   size1   The first size
 * @param   size2   The second size
 *
 * @return  Returns true if the sizes are equal, false otherwise
 */
bool ei_equal_sizes(ei_size_t size1, ei_size_t size2);

/**
 * @brief   Returns the position of a child in its parent based on the given anchor
 *
 * @param   parent  The parent rectangle
 * @param   child   The child size
 * @param   anchor  The anchor
 *
 * @return  The position of the child in its parent based on the given anchor
 */
ei_point_t ei_get_position_in_parent_from_anchor(ei_rect_t parent, ei_size_t child, ei_anchor_t anchor);

/**
 * @brief   Returns the given rectangle moved by the given x and y
 *
 * @param   rect   The first rectangle
 * @param   x       The amount of pixels to move the rectangle by on the x axis
 * @param   y       The amount of pixels to move the rectangle by on the y axis
 * @param   clipper If not null, the moved rectangle size will be cropped to fit in the clipper
 *
 * @return  The given rectangle moved by the given x and y
 */
ei_rect_t ei_rect_move(ei_rect_t rect, int x, int y, ei_rect_t *clipper);

/**
 * @brief	Returns a \ref ei_rect_t which components are the components of the rect
 *		    parameter plus the given parametrs.
 *
 * @param	rect	The rectangle for which to add the components
 * @param	x		The value to add to the x component of the rectangle
 * @param	y		The value to add to the y component of the rectangle
 * @param   width   The value to add to the width component of the rectangle
 * @param   height  The value to add to the height component of the rectangle
 */
ei_rect_t ei_rect_add(ei_rect_t rect, int x, int y, int width, int height);

/**
 * @brief	Returns a \ref ei_rect_t which components are the components of the rect
 *		    parameter minus the given parametrs.
 *
 * @param	rect	The rectangle for which to sub the components
 * @param	x		The value to sub to the x component of the rectangle
 * @param	y		The value to sub to the y component of the rectangle
 * @param   width   The value to sub to the width component of the rectangle
 * @param   height  The value to sub to the height component of the rectangle
 */
ei_rect_t ei_rect_sub(ei_rect_t rect, int x, int y, int width, int height);

/**
 * @brief	Returns if the point is inside the circle formed by the given parameters
 *
 * @param	point	The point to check if it is inside the circle
 * @param	center	The center of the circle
 * @param	radius	The radius of the circle
 *
 * @return	True if the point is inside the circle, false otherwise
 */
bool ei_is_point_in_circle(ei_point_t point, ei_point_t center, int radius);

/**
 * @brief   Converts a color from RGB to HSL.
 *
 * @param   color   The color to convert.
 *
 * @return  The color converted to HSL.
 *
 * @see     https://en.wikipedia.org/wiki/HSL_and_HSV for the formula and https://axonflux.com/handy-rgb-to-hsl-and-rgb-to-hsv-color-model-c
 */
ei_hsl_color_t ei_convert_rgb_to_hsl(ei_color_t color);

/**
 * @brief   Converts a color from HSL to RGB.
 *
 * @param   hsl The color to convert.
 *
 * @return  The color converted to RGB.
 *
 * @see     https://en.wikipedia.org/wiki/HSL_and_HSV for the formula and https://axonflux.com/handy-rgb-to-hsl-and-rgb-to-hsv-color-model-c

*/
ei_color_t ei_convert_hsl_to_rgb(ei_hsl_color_t hsl);

#endif