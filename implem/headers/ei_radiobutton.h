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

static const int k_default_radio_button_size = 14;
static int k_default_radio_button_border_width = 3;
static const ei_color_t k_default_radio_button_selected_color = (ei_color_t){0xc2, 0x19, 0x70, 0xff};
static const int k_default_radio_button_group_padding = 8; // Padding between the group's border and the radio buttons (button + label)
static const int k_default_radio_button_group_spacing = 6; // Spacing between the radio buttons
static const int k_default_radio_button_label_spacing = 6; // Spacing between the button and the label of a radio button

/**
 * @brief   A radio button widget always works with other radio_button widgets. At any
 *          moment, only one button can be active. Radio buttons are used to allow 1 choice among
 *          n. They are connected like a double linked chain.
 *          It is a structure that have one button to represent tha activated state or the deactivated state.
 */
typedef struct ei_impl_radio_button_t
{
    ei_impl_widget_t widget;                         // Common attributes for all types of widgets
    ei_text_properties_t text;                       // Name taken by the radio_button
    bool selected;                                   // To know if the radio_button is activated or not
    struct ei_impl_radio_button_t *next_sibling;     // A Pointer the next radio_button of the group
    struct ei_impl_radio_button_t *previous_sibling; // A Pointer the previous radio_button of the group
    ei_color_t button_selected_color;                // The color of the radio_button when it is selected
    struct ei_radio_button_group_t *group;           // A pointer to the group of radio_buttons
} ei_impl_radio_button_t;

typedef struct ei_radio_button_group_t
{
    ei_impl_widget_t widget;                  // Common attributes for all types of widgets
    ei_widget_appearance_t widget_appearance; // Appearance fields common to frames, buttons, toplevels, entries and radio buttons
    ei_relief_t relief;                       // The relief of the widget
    ei_text_properties_t text;                // Widgets properties for all radio_buttons
    ei_radio_button_t first_radio_button;     // A pointer to the first radio_button on the group
    ei_color_t buttons_color;                 // The color of the button of the radio_buttons
    ei_color_t button_selected_color;         // The color of the radio_button when it is selected
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
 * \brief A function that change a state of a radio_button from desactivated to an activated state
 *      and check if there are only one activated radio_button  in the group.
 *          When We use this function to activate a radio_button the last that was activated become desactivated.
 *
 * @param radio_button A pointer to the radio_button we have to change the state
 *
 * @param selected  The state of the radio_button
 *
 * @return True if the change is done or the check was good and false if not.
 *
 */
bool ei_check_change_radio_button_state(ei_radio_button_t radio_button, bool selected);

/**
 * @brief   Update the position of the button of a radio button
 *
 * @param radio_button  The radio button to update the position of its button
 */
void ei_update_radio_button_position(ei_radio_button_t radio_button);

#endif