#ifndef EI_TOPLEVEL_H
#define EI_TOPLEVEL_H

#include "../implem/headers/ei_implementation.h"

typedef struct
{
    ei_impl_widget_t widget;
    ei_color_t color;
    int border_width;
    ei_string_t title;
    bool closable;
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
ei_widget_t toplevel_allocfunc();

/**
 * \brief	A function that releases the memory used by a widget before it is destroyed.
 *		The memory used for the \ref ei_widget_t structure itself must *not* be freed by
 *		the function. It frees the *fields* of the structure, if necessary.
 *		Can be set to NULL in \ref ei_widgetclass_t if no memory is used by a class of widget.
 *
 * @param	widget		The widget which resources are to be freed.
 */
void toplevel_releasefunc(ei_widget_t widget);

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
void toplevel_drawfunc(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper);

/**
 * \brief	A function that sets the default values for a toplevel.
 *
 * @param	widget		A pointer to the widget instance to initialize.
 */
void toplevel_setdefaultsfunc(ei_widget_t widget);

/**
 * \brief 	A function that is called to notify the widget that its geometry has been modified
 *		    by its geometry manager. Can set to NULL in \ref ei_widgetclass_t.
 *		    The new location can be retrieved by \ref ei_widget_get_screen_location.
 *
 * @param	widget		The widget instance to notify of a geometry change.
 */
void toplevel_geomnotifyfunc(ei_widget_t widget);

#endif