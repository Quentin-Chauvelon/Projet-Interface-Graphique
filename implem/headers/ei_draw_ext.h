/**
 * @file    ei_draw_ext.h
 *
 * @brief   Defines functions to draw a button with rounded corners and a relief.
 *          Extends the \ref ei_draw.h file.
 *
 * @see     \ref ei_draw.h
 *
 * @authors Seydina BADIANE
 * @authors Christian BOSSE
 * @authors Quentin CHAUVELON
 */

#ifndef EI_DRAW_EXT_H
#define EI_DRAW_EXT_H

#include "../api/ei_types.h"
#include "../implem/headers/ei_types_ext.h"
#include "../implem/headers/ei_button.h"

/**
 * @brief   Returns the points forming an arc with the given parameters.
 *
 * @param   center      The center point of the arc.
 * @param   radius      The radius of the arc.
 * @param   start_angle The angle at which to start the arc.
 * @param   end_angle   The angle at which to end the arc.
 *
 * @return  An array of points representing the arc.
 */
ei_point_t *arc(ei_point_t center, int radius, int start_angle, int end_angle);

/**
 * @brief   Creates an array of points forming a rounded frame
 *
 * @param   rect            The outer rectangle that includes the rounded frame.
 * @param   radius          The radius of the rounded corners.
 * @param   part_to_draw    The part of the rounded frame to draw (top, bottom, full).
 *
 * @return  An array of points representing the rounded frame.
 */
ei_point_t *rounded_frame(ei_rect_t rect, int radius, ei_rounded_frame_part_t part_to_draw);

/**
 * @brief   Draws a rounded frame.
 *          The function takes a number of parameters to allow for easy customization of the frame
 *          which allows us to draw all widgets (frames, buttons, toplevels, entries...)
 *
 * @param   surface         Where to draw the rounded frame. The surface must be *locked* by \ref hw_surface_lock.
 * @param   screen_location The location where to draw the rounded frame.
 * @param   border_width    The width of the border of the rounded frame.
 * @param   corner_radius   The radius of the corners of the rounded frame.
 * @param   color           The color of the rounded frame.
 * @param   relief          The relief of the rounded frame.
 * @param   clipper         If not NULL, the drawing is restricted within this rectangle.
 */
void ei_draw_rounded_frame(ei_surface_t surface, ei_rect_t screen_location, int border_width, int corner_radius, ei_color_t color, ei_relief_t relief, const ei_rect_t *clipper);

/**
 * @brief   Calculates the number of points needed to draw an arc based on the given start and end angles.
 *
 * @param   start_angle The start angle of the arc.
 * @param   end_angle   The end angle of the arc.
 *
 * @return  The number of points needed to draw the arc.
 */
int ei_get_nb_points_in_arc(int start_angle, int end_angle);

#endif