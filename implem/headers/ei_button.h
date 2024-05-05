/**
 * @file	ei_button.h
 *
 * @brief 	Definition of a button widget class.
 *          Defines custom functions from \ref ei_widgetclass.h
 *
 * @see    \ref ei_widgetclass.h
 *
 * @authors Seydina BADIANE
 * @authors Christian BOSSE
 * @authors Quentin CHAUVELON
 */

#ifndef EI_BUTTON_H
#define EI_BUTTON_H

#include "../api/ei_types.h"
#include "../api/hw_interface.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_widget_ext.h"

typedef struct
{
    ei_impl_widget_t widget;                  // Common attributes for all types of widgets
    ei_widget_appearance_t widget_appearance; // Appearance fields common to frames, buttons, toplevels, and entries
    ei_frame_appearance_t frame_appearance;   // Appearance fields common to frames and buttons
    int corner_radius;                        // Radius of the corners
    ei_callback_t callback;                   // Callback function to call when clicked
    ei_user_param_t *user_param;              // Custom parameter to pass to the callback
} ei_button_t;

/**
 * \brief	A function that allocates a block of memory that is big enough to store the
 *		attributes of a button.
 *
 * @return			A block of memory big enough to represent a widget of this class,
 * 				with all bytes set to 0.
 */
ei_widget_t button_allocfunc();

/**
 * \brief	A function that releases the memory used by a widget before it is destroyed.
 *		The memory used for the \ref ei_widget_t structure itself must *not* be freed by
 *		the function. It frees the *fields* of the structure, if necessary.
 *		Can be set to NULL in \ref ei_widgetclass_t if no memory is used by a class of widget.
 *
 * @param	widget		The widget which resources are to be freed.
 */
void button_releasefunc(ei_widget_t widget);

/**
 * \brief	A function that draws a button.
 * 		The function must also draw the children of the widget.
 *
 * @param	widget		A pointer to the widget instance to draw.
 * @param	surface		A locked surface where to draw the widget. The actual location of the widget in the
 *				surface is stored in its "screen_location" field.
 * @param	pick_surface	The picking offscreen.
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle
 *				(expressed in the surface reference frame).
 */
void button_drawfunc(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper);

/**
 * \brief	A function that sets the default values for a button.
 *
 * @param	widget		A pointer to the widget instance to initialize.
 */
void button_setdefaultsfunc(ei_widget_t widget);

/**
 * \brief 	A function that is called to notify the widget that its geometry has been modified
 *		    by its geometry manager. Can set to NULL in \ref ei_widgetclass_t.
 *		    The new location can be retrieved by \ref ei_widget_get_screen_location.
 *
 * @param	widget		The widget instance to notify of a geometry change.
 */
void button_geomnotifyfunc(ei_widget_t widget);

void get_button_color(ei_button_t *button);

#endif