/**
 * @file    ei_event_ext.h
 *
 * @brief   Extends the \ref ei_event.h file and add functions to handle events.
 *
 * @see     \ref ei_event.h
 *
 * @authors Seydina BADIANE
 * @authors Christian BOSSE
 * @authors Quentin CHAUVELON
 */

#ifndef EI_EVENT_EXT_H
#define EI_EVENT_EXT_H

#include "../api/ei_event.h"

/**
 * @brief   Pointer to the widget under the mouse cursor
 */
static ei_widget_t picking_widget = NULL;

/**
 * @brief	Describes an event binding.
 */
typedef struct ei_event_bind_t
{
    ei_eventtype_t eventtype;     ///< The type of the event
    ei_widget_t widget;           ///< The widget to bind the event to
    ei_tag_t tag;                 ///< The tag to bind the event to
    ei_callback_t callback;       ///< The callback (i.e. the function to call).
    ei_user_param_t user_param;   ///< User parameter that will be passed to the callback when it is called
    struct ei_event_bind_t *next; ///< Pointer to the next event bind
} ei_event_bind_t;

/**
 * @brief   Handles an event
 *
 * @param   event   The event to handle
 */
void ei_handle_event(ei_event_t event);

/**
 * @brief   Returns the widget under the mouse cursor
 *
 * @return  The widget under the mouse cursor
 */
ei_widget_t ei_get_picking_widget();

/**
 * @brief   Unbinds all the events. Must be called by \ref ei_app_free
 */
void ei_unbind_all_events();

#endif