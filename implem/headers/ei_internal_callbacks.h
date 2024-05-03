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
 * @return  false
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
 * @return  false
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
 * @return  false
 */
static bool ei_cursor_left_button(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param);

/**
 * @brief   Binds all the internal callbacks to the corresponding widgets/tags
 */
void ei_bind_all_internal_callbacks();

#endif