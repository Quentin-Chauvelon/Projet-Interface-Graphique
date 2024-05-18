/**
 * @file	ei_radiobutton.h
 *
 * @brief 	Definition of a radiobutton widget class.
 *          Defines custom functions from \ref ei_widgetclass.h
 *
 * @see    \ref ei_widgetclass.h
 *
 * @authors Seydina BADIANE
 * @authors Christian BOSSE
 * @authors Quentin CHAUVELON
 */

#ifndef EI_RADIOBUTTON_H
#define EI_RADIOBUTTON_H

#include "../api/ei_types.h"
#include "../implem/headers/ei_types_ext.h"
#include "../api/hw_interface.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_widget_ext.h"
#include "../implem/headers/ei_button.h"


/**
 * @brief A radio button widget always works with other radiobutton widgets. At any
 *   moment, only one button can be active. Radio buttons are used to allow 1 choice among
 *   n. They are connected like a double linked chain. 
 *   It is a structure that have one button to represent tha activated state or the deactivated state. 
 *  
*/
typedef struct ei_radiobutton_t
{
    
    ei_text_properties_t text; // Name taken by the radiobutton
    bool selected;             // To know if the radiobutton is activated or not
    struct ei_radiobutton_t *next_sibling;     // A Pointer the next radiobutton of the group
    struct ei_radiobutton_t *previous_sibling; // A Pointer the next radiobutton of the group
    ei_callback_t callback; //Callback function to call when click
    ei_button_t* button ; // A pointer to a button we have to draw before the text 
} ei_radiobutton_t;

typedef struct ei_radiobutton_group_t
{
    ei_impl_widget_t widget; // Common attributes for all types of widgets
    ei_radiobutton_t * radiobutton; //A pointer to the first radiobutton on the group
    ei_widget_appearance_t window; //Appearance of the back_ground of the radiobuttons group, of the window
    ei_relief_t relief; //The relief of the window
}ei_radiobutton_group_t;

/**
 * \brief	A function that allocates a block of memory that is big enough to store the
 *		attributes of a radiobutton group. And a block of memory for the first radiobutton of the group.
 *
 * @return			A block of memory big enough to represent a widget of this class,
 * 				with all bytes set to 0.
 */
ei_widget_t ei_radiobutton_allocfunc_group();

/**
 * \brief	A function that releases the memory used by a widget before it is destroyed.
 *		The memory used for the \ref ei_widget_t structure itself must *not* be freed by
 *		the function. It frees the *fields* of the structure, if necessary.
 *		Can be set to NULL in \ref ei_widgetclass_t if no memory is used by a class of widget.
 *
 * @param	widget		The widget which resources are to be freed.
 */
void ei_radiobutton_releasefunc_group(ei_widget_t widget);

/**
 * \brief	A function that draws every radiobutton of a group with a button before everyone.
 * 		
 *
 * @param	widget		A pointer to the widget instance to draw,a pointer to the radiobuttons group.
 * @param	surface		A locked surface where to draw the widget. The actual location of the widget in the
 *				surface is stored in its "screen_location" field.
 * @param	pick_surface	The picking offscreen.
 * @param   border_width      The border width of the frame 
 * @param	clipper		      If not NULL, the drawing is restricted within this rectangle
 *				(expressed in the surface reference frame).
    @param  screen_location   The Screen location of the frame 
 */
void ei_radiobutton_drawfunc_group(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper);

/**
 * \brief	A function that sets the default values for a radiobutton group. This group contain only one radiobutton
 *              with default values.
 *
 * @param	widget		A pointer to the widget instance to initialize, a pointer to the radiobuttons group. 
 */
void ei_radiobutton_setdefaultsfunc_group(ei_widget_t widget);

/**
 * \brief A function that sets the default values for a radiobutton
 * 
 * @param radiobutton A pointer to the radiobutton.
 * 
*/
ei_radiobutton_t* ei_radiobutton_setdefaultsfunc( ei_radiobutton_t* radiobutton);

/**
 * \brief A function that add another radiobutton to form a group of radiobuttons.If the radiobutton is actived, the last
 *          one who was actived change to a desactived state.
 * 
 * @param widget A pointer to the group of radiobuttons.
 * 
 * @param radiobutton A pointer to the radiobutton to add
*/
void ei_add_radiobutton( ei_widget_t widget, ei_radiobutton_t* radiobutton);

/**
 * \brief A function that change a state of a radiobutton from desactivated to an activated state
 *      and check if there are only one activated radiobutton  in the group.
 *          When We use this function to activate a radiobutton the last that was activated become desactivated.
 *
 * @param radiobutton A pointer to the radiobutton we have to change the state
 *
 * @param selected  The state of the radiobutton
 *
 * @return True if the change is done or the check was good and false if not.
 *
 */
bool ei_check_change_radiobutton_state(ei_radiobutton_t *radiobutton, bool selected);

/**
 * \brief 	A function that is called to notify the widget that its geometry has been modified
 *		    by its geometry manager. Can set to NULL in \ref ei_widgetclass_t.
 *		    The new location can be retrieved by \ref ei_widget_get_screen_location.
            But useless for a radiobutton,only use for to define correctly the widget. 
        
 *
 * @param	widget		The widget instance to notify of a geometry change.
 */
void ei_radiobutton_geomnotifyfunc(ei_widget_t widget);

#endif