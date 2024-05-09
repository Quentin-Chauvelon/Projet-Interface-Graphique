#ifndef EI_TOPLEVEL_H
#define EI_TOPLEVEL_H

#include "../api/ei_types.h"
#include "../api/hw_interface.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_widget_ext.h"
#include "../implem/headers/ei_button.h"

static const ei_color_t k_default_toplevel_title_bar_background_color = (ei_color_t){100, 100, 100, 225};
static const int k_default_toplevel_title_corner_radius = 15;
static const int k_default_toplevel_title_left_padding = 7;
static const int k_default_toplevel_close_button_size = 16;
static const int k_default_toplevel_resize_square_size = 12;

typedef struct
{
    ei_impl_widget_t widget;
    ei_widget_appearance_t widget_appearance; // Appearance fields common to frames, toplevels, toplevels, and entries
    ei_string_t title;
    bool closable;
    ei_button_t *close_button;
    ei_axis_set_t resizable;
    ei_size_ptr_t min_size;
} ei_toplevel_t;

/**
 * \brief	A function that allocates a block of memory that is big enough to store the
 *		attributes of a toplevel.
 *
 * @return			A block of memory big enough to represent a widget of this class,
 * 				with all bytes set to 0.
 */
ei_widget_t ei_toplevel_allocfunc();

/**
 * \brief	A function that releases the memory used by a widget before it is destroyed.
 *		The memory used for the \ref ei_widget_t structure itself must *not* be freed by
 *		the function. It frees the *fields* of the structure, if necessary.
 *		Can be set to NULL in \ref ei_widgetclass_t if no memory is used by a class of widget.
 *
 * @param	widget		The widget which resources are to be freed.
 */
void ei_toplevel_releasefunc(ei_widget_t widget);

/**
 * \brief	A function that draws a toplevel.
 * 		The function must also draw the children of the widget.
 *
 * @param	widget		A pointer to the widget instance to draw.
 * @param	surface		A locked surface where to draw the widget. The actual location of the widget in the
 *				surface is stored in its "screen_location" field.
 * @param	pick_surface	The picking offscreen.
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle
 *				(expressed in the surface reference toplevel).
 */
void ei_toplevel_drawfunc(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper);

/**
 * \brief	A function that sets the default values for a toplevel.
 *
 * @param	widget		A pointer to the widget instance to initialize.
 */
void ei_toplevel_setdefaultsfunc(ei_widget_t widget);

/**
 * \brief 	A function that is called to notify the widget that its geometry has been modified
 *		    by its geometry manager. Can set to NULL in \ref ei_widgetclass_t.
 *		    The new location can be retrieved by \ref ei_widget_get_screen_location.
 *
 * @param	widget		The widget instance to notify of a geometry change.
 */
void ei_toplevel_geomnotifyfunc(ei_widget_t widget);

/**
 * @brief   Returns the natural size of a toplevel
 *
 * @param   toplevel   The toplevel to return the natural size for.
 *
 * @return  The natural size of the toplevel.
 */
ei_size_t ei_toplevel_get_natural_size(ei_toplevel_t *toplevel);

/**
 * @brief   Instantiates the close button with the default parameters
 *
 * @return  The close button
 */
ei_button_t *ei_toplevel_instantiate_close_button(ei_toplevel_t *toplevel);

/**
 * @brief   Returns the rect of the title bar rectangle of a toplevel
 *
 * @param   toplevel The toplevel to return the rect of the title bar for
 *
 * @return  The rect of the title bar rectangle of the toplevel
 */
ei_rect_t ei_toplevel_get_title_bar_rect(ei_toplevel_t *toplevel);

/**
 * @brief   Returns the rect of the resize square of a toplevel
 *
 * @param   toplevel The toplevel to return the rect of the resize square for
 *
 * @return  The rect of the resize square of the toplevel
 */
ei_rect_t ei_toplevel_get_resize_square_rect(ei_toplevel_t *toplevel);

/**
 * @brief   Computes the minimum size needed for the toplevel
 *
 * @param   widget  The toplevel to compute the minimum size for
 * @param   width   The width of the minimum size
 * @param   height  The height of the minimum size
 */
void ei_toplevel_get_min_size(ei_widget_t widget, int *width, int *height);

#endif