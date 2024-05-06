/**
 * @file	ei_frame.h
 *
 * @brief 	Definition of a frame widget class.
 *          Defines custom functions from \ref ei_widgetclass.h
 *
 * @see    \ref ei_widgetclass.h
 *
 * @authors Seydina BADIANE
 * @authors Christian BOSSE
 * @authors Quentin CHAUVELON
 */

#ifndef EI_FRAME_H
#define EI_FRAME_H

#include "../api/ei_types.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_widget_ext.h"

typedef struct
{
    ei_impl_widget_t widget;                  // Common attributes for all types of widgets
    ei_widget_appearance_t widget_appearance; // Appearance fields common to frames, buttons, toplevels, and entries
    ei_frame_appearance_t frame_appearance;   // Appearance fields common to frames and buttons
} ei_frame_t;

/**
 * \brief	A function that allocates a block of memory that is big enough to store the
 *		attributes of a frame.
 *
 * @return			A block of memory big enough to represent a widget of this class,
 * 				with all bytes set to 0.
 */
ei_widget_t frame_allocfunc();

/**
 * \brief	A function that releases the memory used by a widget before it is destroyed.
 *		The memory used for the \ref ei_widget_t structure itself must *not* be freed by
 *		the function. It frees the *fields* of the structure, if necessary.
 *		Can be set to NULL in \ref ei_widgetclass_t if no memory is used by a class of widget.
 *
 * @param	widget		The widget which resources are to be freed.
 */
void frame_releasefunc(ei_widget_t widget);

/**
 * \brief	A function that draws a frame.
 * 		The function must also draw the children of the widget.
 *
 * @param	widget		A pointer to the widget instance to draw.
 * @param	surface		A locked surface where to draw the widget. The actual location of the widget in the
 *				surface is stored in its "screen_location" field.
 * @param	pick_surface	The picking offscreen.
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle
 *				(expressed in the surface reference frame).
 */
void frame_drawfunc(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper);

/**
 * \brief	A function that sets the default values for a frame.
 *
 * @param	widget		A pointer to the widget instance to initialize.
 */
void frame_setdefaultsfunc(ei_widget_t widget);

/**
 * \brief 	A function that is called to notify the widget that its geometry has been modified
 *		    by its geometry manager. Can set to NULL in \ref ei_widgetclass_t.
 *		    The new location can be retrieved by \ref ei_widget_get_screen_location.
 *
 * @param	widget		The widget instance to notify of a geometry change.
 */
void frame_geomnotifyfunc(ei_widget_t widget);

/**
 * @brief   Returns the color of a frame.
 *
 * @param   frame   The frame to return the color from.
 *
 * @return  The color of the frame.
 */
ei_color_t ei_frame_get_color(ei_frame_t *frame);

/**
 * @brief   Returns the border width of a frame.
 *
 * @param   frame   The frame to return the border width from.
 *
 * @return  The border width of the frame.
 */
int ei_frame_get_border_width(ei_frame_t *frame);

/**
 * @brief   Returns the relief of a frame.
 *
 * @param   frame   The frame to return the relief from.
 *
 * @return  The relief of the frame.
 */
ei_relief_t ei_frame_get_relief(ei_frame_t *frame);

/**
 * @brief   Returns the text of a frame.
 *
 * @param   frame   The frame to return the text from.
 *
 * @return  The text of the frame.
 */
ei_string_t ei_frame_get_text_label(ei_frame_t *frame);

/**
 * @brief   Returns the font of the text of a frame.
 *
 * @param   frame   The frame to return the font from.
 *
 * @return  The font of the text of the frame.
 */
ei_font_t ei_frame_get_text_font(ei_frame_t *frame);

/**
 * @brief   Returns the color of the text of a frame.
 *
 * @param   frame   The frame to return the color from.
 *
 * @return  The color of the text of the frame.
 */
ei_color_t ei_frame_get_text_color(ei_frame_t *frame);

/**
 * @brief   Returns the anchor of the text of a frame.
 *
 * @param   frame   The frame to return the anchor from.
 *
 * @return  The anchor of the text of the frame.
 */
ei_anchor_t ei_frame_get_text_anchor(ei_frame_t *frame);

/**
 * @brief   Returns the image of a frame.
 *
 * @param   frame   The frame to return the image from.
 *
 * @return  The image of the frame.
 */
ei_surface_t ei_frame_get_image_data(ei_frame_t *frame);

/**
 * @brief   Returns the rectangle of the image of a frame.
 *
 * @param   frame   The frame to return the rectangle from.
 *
 * @return  The rectangle of the image of the frame.
 */
ei_rect_t *ei_frame_get_image_rect(ei_frame_t *frame);

/**
 * @brief   Returns the anchor of the image of a frame.
 *
 * @param   frame   The frame to return the anchor from.
 *
 * @return  The anchor of the image of the frame.
 */
ei_anchor_t ei_frame_get_image_anchor(ei_frame_t *frame);

/**
 * @brief   Returns the natural size of a frame
 *
 * @param   frame   The frame to return the natural size for.
 *
 * @return  The natural size of the frame.
 */
ei_size_t ei_frame_get_natural_size(ei_frame_t *frame);

/**
 * @brief   Sets the color of a frame.
 *
 * @param   frame   The frame to set the color to.
 * @param   color   The color to set.
 */
void ei_frame_set_color(ei_frame_t *frame, ei_color_t color);

/**
 * @brief   Sets the border width of a frame.
 *
 * @param   frame   The frame to set the border width to.
 * @param   border_width The border width to set.
 */
void ei_frame_set_border_width(ei_frame_t *frame, int border_width);

/**
 * @brief   Sets the relief of a frame.
 *
 * @param   frame   The frame to set the relief to.
 * @param   relief  The relief to set.
 */
void ei_frame_set_relief(ei_frame_t *frame, ei_relief_t relief);

/**
 * @brief   Sets the text of a frame.
 *
 * @param   frame   The frame to set the text to.
 * @param   text    The text to set.
 */
void ei_frame_set_text_label(ei_frame_t *frame, ei_string_t text);

/**
 * @brief   Sets the font of the text of a frame.
 *
 * @param   frame   The frame to set the font to.
 * @param   text_font   The font to set.
 */
void ei_frame_set_text_font(ei_frame_t *frame, ei_font_t text_font);

/**
 * @brief   Sets the color of the text of a frame.
 *
 * @param   frame   The frame to set the color to.
 * @param   text_color  The color to set.
 */
void ei_frame_set_text_color(ei_frame_t *frame, ei_color_t text_color);

/**
 * @brief   Sets the anchor of the text of a frame.
 *
 * @param   frame   The frame to set the anchor to.
 * @param   text_anchor The anchor to set.
 */
void ei_frame_set_text_anchor(ei_frame_t *frame, ei_anchor_t text_anchor);

/**
 * @brief   Sets the image of a frame.
 *
 * @param   frame   The frame to set the image to.
 * @param   image The image to set.
 */
void ei_frame_set_image_data(ei_frame_t *frame, ei_surface_t image);

/**
 * @brief   Sets the rectangle of the image of a frame.
 *
 * @param   frame   The frame to set the rectangle to.
 * @param   image_rect    The rectangle to set.
 */
void ei_frame_set_image_rect(ei_frame_t *frame, ei_rect_t *image_rect);

/**
 * @brief   Sets the anchor of the image of a frame.
 *
 * @param   frame   The frame to set the anchor to.
 * @param   image_anchor  The anchor to set.
 */
void ei_frame_set_image_anchor(ei_frame_t *frame, ei_anchor_t image_anchor);

#endif