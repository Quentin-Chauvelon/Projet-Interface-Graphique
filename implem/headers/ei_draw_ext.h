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
 * @brief   Draws a button.
 *          The button takes a border width and a color as parameters so that it can be
 *          customized without having to change the button values.
 *
 * @param   button  The button to draw.
 * @param	surface Where to draw the polygon. The surface must be *locked* by \ref hw_surface_lock.
 * @param   clipper If not NULL, the drawing is restricted within this rectangle.
 * @param   border_width The width of the border of the button.
 * @param   color The color of the button.
 */
void ei_draw_button(ei_button_t *button, ei_surface_t surface, const ei_rect_t *clipper, int border_width, ei_color_t color);

/**
 * @brief   Draws a visible button.
 *          Wrapper for \ref ei_draw_button that uses the button's values.
 *
 * @param   button  The button to draw.
 * @param	surface Where to draw the polygon. The surface must be *locked* by \ref hw_surface_lock.
 * @param   clipper If not NULL, the drawing is restricted within this rectangle.
 */
static inline void ei_draw_visible_button(ei_button_t *button, ei_surface_t surface, const ei_rect_t *clipper)
{
    ei_draw_button(button, surface, clipper, button->border_width, button->color);
}

/**
 * @brief   Draws an offscreen button.
 *          Wrapper for \ref ei_draw_button that uses the given border width and color.
 *
 * @param   button  The button to draw.
 * @param	surface Where to draw the polygon. The surface must be *locked* by \ref hw_surface_lock.
 * @param   clipper If not NULL, the drawing is restricted within this rectangle.
 * @param   border_width The width of the border of the button.
 * @param   color The color of the button.
 */
static inline void ei_draw_offscreen_button(ei_button_t *button, ei_surface_t surface, const ei_rect_t *clipper, int border_width, ei_color_t color)
{
    ei_draw_button(button, surface, clipper, border_width, color);
}

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