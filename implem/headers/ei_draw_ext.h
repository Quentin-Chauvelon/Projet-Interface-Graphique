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
ei_point_t *ei_get_arc_points(ei_point_t center, int radius, int start_angle, int end_angle);

/**
 * @brief   Creates an array of points forming a rounded frame
 *
 * @param   rect            The outer rectangle that includes the rounded frame.
 * @param   radius          The radius of the rounded corners.
 * @param   part_to_draw    The part of the rounded frame to draw (top, bottom, full).
 *
 * @return  An array of points representing the rounded frame.
 */
ei_point_t *ei_get_rounded_frame_points(ei_rect_t rect, int radius, ei_rounded_frame_part_t part_to_draw);

/**
 * @brief   Draws a plain rectanglee.
 *
 * @param   surface             Where to draw the rectangle. The surface must be *locked* by \ref hw_surface_lock.
 * @param   screen_location     The location of the rectangle.
 * @param   background_color    The color of the rectangle.
 * @param   clipper             If not NULL, the drawing is restricted within this rectangle.
 */
void ei_draw_rectangle(ei_surface_t surface, ei_rect_t screen_location, ei_color_t background_color, const ei_rect_t *clipper);

/**
 * @brief   Draws a frame with rounded or straight corners.
 *          The function takes a number of parameters to allow for easy customization of the frame
 *          which allows us to draw all widgets (frames, buttons, toplevels, entries...)
 *
 * @param   surface             Where to draw the rounded frame. The surface must be *locked* by \ref hw_surface_lock.
 * @param   screen_location     The location where to draw the rounded frame.
 * @param   border_width        The width of the border of the rounded frame.
 * @param   corner_radius       The radius of the corners of the rounded frame.
 * @param   background_color    The color of the rounded frame.
 * @param   relief              The relief of the rounded frame.
 * @param   border_color        The color of the border of the rounded frame.
 * @param   clipper             If not NULL, the drawing is restricted within this rectangle.
 */
void ei_draw_frame(ei_surface_t surface, ei_rect_t screen_location, int border_width, int corner_radius, ei_color_t background_color, ei_relief_t relief, const ei_color_t *border_color, const ei_rect_t *clipper);

static inline void ei_draw_rounded_frame(ei_surface_t surface, ei_rect_t screen_location, int border_width, int corner_radius, ei_color_t background_color, ei_relief_t relief, const ei_rect_t *clipper)
{
    ei_draw_frame(surface, screen_location, border_width, corner_radius, background_color, relief, NULL, clipper);
}

static inline void ei_draw_straight_frame(ei_surface_t surface, ei_rect_t screen_location, int border_width, ei_color_t background_color, ei_relief_t relief, const ei_rect_t *clipper)
{
    ei_draw_frame(surface, screen_location, border_width, 0, background_color, relief, NULL, clipper);
}

void ei_draw_circle(ei_surface_t surface, ei_point_t center, int radius, ei_color_t color, const ei_rect_t *clipper);

/**
 * @brief   Calculates the number of points needed to draw an arc based on the given start and end angles.
 *          If the radius is 0, the function returns 0 (since we don't need to draw an arc)
 *
 * @param   start_angle The start angle of the arc.
 * @param   end_angle   The end angle of the arc.
 * @param   radius      The radius of the arc.
 *
 * @return  The number of points needed to draw the arc.
 */
int ei_get_nb_points_in_arc(int start_angle, int end_angle, int radius);

/**
 * @brief   Draws image.
 *
 * @param   surface 	    Where to draw the image. The surface must be *locked* by
 *				            \ref hw_surface_lock.
 * @param   image		    The surface containing the image. Can't be NULL.
 * @param   image_subpart	If not NULL, the rectangle defines a subpart of "img" to use as the
 *				            image displayed
 * @param   where		    Coordinates, in the surface, where to anchor the top-left corner of
 *		            	    the image.
 * @param   clipper		    If not NULL, the drawing is restricted within this rectangle.
 */
void ei_draw_image(ei_surface_t surface, ei_surface_t image, ei_rect_t *image_subpart, const ei_point_t *where, const ei_rect_t *clipper);

/**
 * @brief   Computes the the two colors (one lighter and one darked) used for the border based
 *          on the background color and the relief. Pointers to the two colors should be passed
 *          as arguments to get the result.
 *
 * @param   background_color The color of the background.
 * @param   relief          The relief of the border.
 * @param   lighter_color   A pointer to the lighter color.
 * @param   darker_color    A pointer to the darker color.
 */
void ei_get_border_colors(ei_color_t background_color, ei_relief_t relief, ei_color_t *lighter_color, ei_color_t *darker_color);

/**
 * @brief   Returns a color that is lighter than the given color.
 *
 * @param   color   The color to lighten.
 *
 * @return  A color that is lighter than the given color.
 */
ei_color_t ei_lighten_color(ei_color_t color);

/**
 * @brief   Returns a color that is darker than the given color.
 *
 * @param   color   The color to darken.
 *
 * @return  A color that is darker than the given color.
 */
ei_color_t ei_darken_color(ei_color_t color);

#endif