/**
 * @file    ei_internal_callbacks.h
 *
 * @brief   Defines internal callbacks function that will be used on the widgets/tags
 *
 * @authors Seydina BADIANE
 * @authors Christian BOSSE
 * @authors Quentin CHAUVELON
 */

#ifndef EI_INTERNAL_CALLBACKS_H
#define EI_INTERNAL_CALLBACKS_H

/**
 * @brief   Callback function that is called when the user presses the mouse button over the button
 *
 * @param   widget      The button the user pressed
 * @param   event       The event information
 * @param   user_param  The user parameters
 *
 * @return  True if the event was handled, false otherwise
 */
static bool ei_button_press(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param);

/**
 * @brief   Callback function that is called when the user releases the mouse button over the button
 *          if it had been pressed before
 *
 * @param   widget      The button the user pressed
 * @param   event       The event information
 * @param   user_param  The user parameters
 *
 * @return  True if the event was handled, false otherwise
 */
static bool ei_button_release(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param);

/**
 * @brief   Callback function that is called when the user moves the cursor out of the button
 *          if it had been pressed before
 *
 * @param   widget      The button the user pressed
 * @param   event       The event information
 * @param   user_param  The user parameters
 *
 * @return  True if the event was handled, false otherwise
 */
static bool ei_cursor_left_button(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param);

/**
 * @brief   Callback function that is called when the use presses on a toplevel
 *
 * @param   widget      The toplevel the user pressed
 * @param   event       The event information
 * @param   user_param  The user parameters
 *
 * @return  True if the event was handled, false otherwise
 */
static bool ei_toplevel_pressed(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param);

/**
 * @brief   Callback function that is called when the user moves the cursor around after pressing the
 *          title bar of the toplevel
 *
 * @param   widget      The toplevel the user pressed
 * @param   event       The event information
 * @param   user_param  The user parameters
 *
 * @return  True if the event was handled, false otherwise
 */
static bool ei_toplevel_move(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param);

/**
 * @brief   Callback function that is called when the user releases the mouse button over the toplevel
 *          if they were moving it before
 *
 * @param   widget      The toplevel the user pressed
 * @param   event       The event information
 * @param   user_param  The user parameters
 *
 * @return  True if the event was handled, false otherwise
 */
static bool ei_toplevel_move_released(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param);

/**
 * @brief   Callback function that is called when the user moves the cursor around after pressing the
 *          resize square of the toplevel
 *
 * @param   widget      The toplevel the user pressed
 * @param   event       The event information
 * @param   user_param  The user parameters
 *
 * @return  True if the event was handled, false otherwise
 */
static bool ei_toplevel_resize(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param);

/**
 * @brief   Callback function that is called when the user releases the mouse button over the toplevel
 *          if they were resizing it before
 *
 * @param   widget      The toplevel the user pressed
 * @param   event       The event information
 * @param   user_param  The user parameters
 *
 * @return  True if the event was handled, false otherwise
 */
static bool ei_toplevel_resize_released(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param);

/**
 * @brief   Callback function that is called when the user clicks on an entry
 *
 * @param   widget      The entry the user pressed
 * @param   event       The event information
 * @param   user_param  The user parameters
 *
 * @return  True if the event was handled, false otherwise
 */
static bool ei_entry_pressed(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param);

/**
 * @brief   Callback function that is called when the user clicks on an entry for the second time
 *
 * @param   widget      The entry the user pressed
 * @param   event       The event information
 * @param   user_param  The user parameters
 *
 * @return  True if the event was handled, false otherwise
 */
bool ei_entry_double_click(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param);

/**
 * @brief   Callback function that is called when the user clicks on an entry for the third time
 *          after a valid double click
 *
 * @param   widget      The entry the user pressed
 * @param   event       The event information
 * @param   user_param  The user parameters
 *
 * @return  True if the event was handled, false otherwise
 */
bool ei_entry_triple_click(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param);

/**
 * @brief   Callback function that is called when the user moves the mouse after clicking on an entry
 *
 * @param   widget      The currently focused entry
 * @param   event       The event information
 * @param   user_param  The user parameters
 *
 * @return  True if the event was handled, false otherwise
 */
bool ei_entry_move(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param);

/**
 * @brief   Callback function that is called when the use releases the mouse button after clicking on an entry.
 *
 * @param   widget      The currently focused entry
 * @param   event       The event information
 * @param   user_param  The user parameters
 *
 * @return  True if the event was handled, false otherwise
 */
bool ei_entry_move_released(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param);

/**
 * @brief   Callback function that is called when the user presses a key after focusing on an entry.
 *
 * @param   widget      The currently focused entry
 * @param   event       The event information
 * @param   user_param  The user parameters
 *
 * @return  True if the event was handled, false otherwise
 */
bool ei_entry_keyboard_key_down(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param);

/**
 * @brief   Binds all the internal callbacks to the corresponding widgets/tags
 */
void ei_bind_all_internal_callbacks();

#endif