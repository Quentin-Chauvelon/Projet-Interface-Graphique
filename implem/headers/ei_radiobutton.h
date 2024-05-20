/**
 * @file	ei_radio_button.h
 *
 * @brief 	Definition of a radio_button widget class.
 *          Defines custom functions from \ref ei_widgetclass.h
 *
 * @see    \ref ei_widgetclass.h
 *
 * @authors Seydina BADIANE
 * @authors Christian BOSSE
 * @authors Quentin CHAUVELON
 */

#ifndef EI_RADIO_BUTTON_H
#define EI_RADIO_BUTTON_H

#include "../api/ei_types.h"
#include "../implem/headers/ei_types_ext.h"
#include "../api/hw_interface.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_widget_ext.h"
#include "../implem/headers/ei_button.h"
#include "../implem/headers/api/ei_radio_button.h"

static const int k_default_radio_button_size = 12;
static int k_default_radio_button_border_width = 3;
static const ei_color_t k_default_radio_button_color = (ei_color_t){0xbf, 0xbf, 0xbf, 0xff};
static const ei_color_t k_default_radio_button_selected_color = (ei_color_t){0xc2, 0x19, 0x70, 0xff};
static const ei_color_t k_default_random_radio_button_color = (ei_color_t){0xde, 0x4c, 0x8a, 0x00}; // Random transparent color used instead of NULL since colors aren't pointers
static const int k_default_radio_button_group_padding = 12;                                         // Padding between the group's border and the radio buttons (button + label)
static const int k_default_radio_button_group_spacing = 2;                                          // Spacing between the radio buttons
static const int k_default_radio_button_label_spacing = 10;                                         // Spacing between the button and the label of a radio button

/**
 * @brief   A radio button that contains a button and a label. To make only one selectable at a time (1 in n), a
 *          `ref ei_radio_button_group_t must be created and the radio buttons parented to it.
 */
typedef struct ei_radio_button_t
{
    ei_impl_widget_t widget;                         // Common attributes for all types of widgets
    ei_text_properties_t text;                       // Name taken by the radio_button
    ei_color_t button_color;                         // The color of the button of the radio_button
    ei_color_t button_selected_color;                // The color of the radio_button when it is selected
    bool selected;                                   // To know if the radio_button is activated or not
} ei_radio_button_t;

/**
 * @brief   A group that contains radio buttons. It allows to select one of its children. The radio
 *          buttons group can only have radio buttons as children.
 */
typedef struct ei_radio_button_group_t
{
    ei_impl_widget_t widget;                  // Common attributes for all types of widgets
    ei_widget_appearance_t widget_appearance; // Appearance fields common to frames, buttons, toplevels, entries and radio buttons
    ei_relief_t relief;                       // The relief of the widget
    ei_text_properties_t text;                // Widgets properties for all radio_buttons
    ei_color_t buttons_color;                 // The color of the button of the radio_buttons
    ei_color_t buttons_selected_color;        // The color of the radio_button when it is selected
    ei_callback_t callback;                   // Callback function that will be called when a radio button is selected
} ei_radio_button_group_t;

/**
 * \brief	A function that allocates a block of memory that is big enough to store the
 *		attributes of a radio_button group. And a block of memory for the first radio_button of the group.
 *
 * @return			A block of memory big enough to represent a widget of this class,
 * 				with all bytes set to 0.
 */
ei_widget_t ei_radio_button_group_allocfunc();

/**
 * \brief	A function that releases the memory used by a widget before it is destroyed.
 *		The memory used for the \ref ei_widget_t structure itself must *not* be freed by
 *		the function. It frees the *fields* of the structure, if necessary.
 *		Can be set to NULL in \ref ei_widgetclass_t if no memory is used by a class of widget.
 *
 * @param	widget		The widget which resources are to be freed.
 */
void ei_radio_button_group_releasefunc(ei_widget_t widget);

/**
 * \brief	A function that draws every radio_button of a group with a button before everyone.
 *
 *
 * @param	widget		A pointer to the widget instance to draw,a pointer to the radio_buttons group.
 * @param	surface		A locked surface where to draw the widget. The actual location of the widget in the
 *				surface is stored in its "screen_location" field.
 * @param	pick_surface	The picking offscreen.
 * @param   border_width      The border width of the frame
 * @param	clipper		      If not NULL, the drawing is restricted within this rectangle
 *				(expressed in the surface reference frame).
    @param  screen_location   The Screen location of the frame
 */
void ei_radio_button_group_drawfunc(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper);

/**
 * \brief	A function that sets the default values for a radio_button group. This group contain only one radio_button
 *              with default values.
 *
 * @param	widget		A pointer to the widget instance to initialize, a pointer to the radio_buttons group.
 */
void ei_radio_button_group_setdefaultsfunc(ei_widget_t widget);

/**
 * \brief 	A function that is called to notify the widget that its geometry has been modified
 *		    by its geometry manager. Can set to NULL in \ref ei_widgetclass_t.
 *		    The new location can be retrieved by \ref ei_widget_get_screen_location.
 *
 * @param	widget		The widget instance to notify of a geometry change.
 */
void ei_radio_button_group_geomnotifyfunc(ei_widget_t widget);

/**
 * @brief   Returns the natural size of a radio button group (to fit all radio buttons)
 *
 * @param   radio_button_group  The radio button group to get the natural size for
 *
 * @return  The natural size of the radio_button_group
 */
ei_size_t ei_radio_button_group_get_natural_size(ei_radio_button_group_t *radio_button_group);

/**
 * \brief	A function that allocates a block of memory that is big enough to store the
 *		attributes of a radio_button group. And a block of memory for the first radio_button of the group.
 *
 * @return			A block of memory big enough to represent a widget of this class,
 * 				with all bytes set to 0.
 */
ei_widget_t ei_radio_button_allocfunc();

/**
 * \brief	A function that releases the memory used by a widget before it is destroyed.
 *		The memory used for the \ref ei_widget_t structure itself must *not* be freed by
 *		the function. It frees the *fields* of the structure, if necessary.
 *		Can be set to NULL in \ref ei_widgetclass_t if no memory is used by a class of widget.
 *
 * @param	widget		The widget which resources are to be freed.
 */
void ei_radio_button_releasefunc(ei_widget_t widget);

/**
 * \brief	A function that draws every radio_button of a group with a button before everyone.
 *
 *
 * @param	widget		A pointer to the widget instance to draw,a pointer to the radio_buttons group.
 * @param	surface		A locked surface where to draw the widget. The actual location of the widget in the
 *				surface is stored in its "screen_location" field.
 * @param	pick_surface	The picking offscreen.
 * @param   border_width      The border width of the frame
 * @param	clipper		      If not NULL, the drawing is restricted within this rectangle
 *				(expressed in the surface reference frame).
    @param  screen_location   The Screen location of the frame
 */
void ei_radio_button_drawfunc(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper);

/**
 * \brief	A function that sets the default values for a radio_button group. This group contain only one radio_button
 *              with default values.
 *
 * @param	widget		A pointer to the widget instance to initialize, a pointer to the radio_buttons group.
 */
void ei_radio_button_setdefaultsfunc(ei_widget_t widget);

/**
 * \brief 	A function that is called to notify the widget that its geometry has been modified
 *		    by its geometry manager. Can set to NULL in \ref ei_widgetclass_t.
 *		    The new location can be retrieved by \ref ei_widget_get_screen_location.
 *
 * @param	widget		The widget instance to notify of a geometry change.
 */
void ei_radio_button_geomnotifyfunc(ei_widget_t widget);

/**
 * @brief   Update the position of the radio buttons of the given group
 *
 * @param radio_button_group The radio button group to update the position of the radio buttons for
 */
void ei_radio_buttons_update_position(ei_radio_button_group_t *radio_button_group);

#endif