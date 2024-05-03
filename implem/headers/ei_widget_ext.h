/**
 * @file    ei_widget_ext.h
 *
 * @brief   Extends the \ref ei_widget.h file to add new functions.
 *
 * @see     \ref ei_widget.h
 *
 * @authors Seydina BADIANE
 * @authors Christian BOSSE
 * @authors Quentin CHAUVELON
 */

#ifndef EI_WIDGET_EXT_H
#define EI_WIDGET_EXT_H

#include "../api/ei_widget.h"
#include "../api/ei_types.h"

/**
 * @brief	Creates a new instance of a widget of some particular class, as a descendant of
 *		    an existing widget. This function is called by \ref ei_widget_create.
 *          The main difference is that this function allows for parent to be NULL (used for the root widget).
 *
 *		The widget is not displayed on screen until it is managed by a geometry manager.
 *		When no more needed, the widget must be released by calling \ref ei_widget_destroy.
 *
 * @param	class_name	The name of the class of the widget that is to be created.
 * @param	parent 		A pointer to the parent widget. Can be NULL.
 * @param	user_data	A pointer provided by the programmer for private use. May be NULL.
 * @param	destructor	A pointer to a function to call before destroying a widget structure. May be NULL.
 *
 * @return			The newly created widget, or NULL if there was an error.
 */
ei_widget_t ei_widget_create_internal(ei_const_string_t class_name, ei_widget_t parent, ei_user_param_t user_data, ei_widget_destructor_t destructor);

#endif