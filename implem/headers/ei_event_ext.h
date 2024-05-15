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
 * @brief	Describes an event binding.
 */
typedef struct ei_event_bind_t
{
    ei_eventtype_t eventtype;     ///< The type of the event
    ei_widget_t widget;           ///< The widget to bind the event to
    ei_tag_t tag;                 ///< The tag to bind the event to
    ei_callback_t callback;       ///< The callback (i.e. the function to call).
    ei_user_param_t user_param;   ///< User parameter that will be passed to the callback when it is called
    int priority;                 ///< The priority of the event bind. Highest priority is at the head of the list and lowest (0) at the tail
    struct ei_event_bind_t *previous; ///< Pointer to the previous event bind
    struct ei_event_bind_t *next; ///< Pointer to the next event bind
} ei_event_bind_t;

/**
 * @brief	Describes a list of events to unbind
 */
typedef struct ei_event_to_unbind_t
{
    struct ei_event_bind_t *event;     ///< Pointer to the event to unbind
    struct ei_event_to_unbind_t *next; ///< Pointer to the next event to unbind
} ei_event_to_unbind_t;

/**
 * \brief	Binds a callback to an event type and a widget or a tag based on its priority.
 *
 * @param	eventtype	The type of the event.
 * @param	widget		The callback is only called if the event is related to this widget.
 *				This parameter must be NULL if the "tag" parameter is not NULL.
 * @param	tag		The callback is only called if the event is related to a widget that
 *				has this tag. A tag can be a widget class name, or the tag "all".
 *				This parameter must be NULL is the "widget" parameter is not NULL.
 * @param	callback	The callback (i.e. the function to call).
 * @param	user_param	A user parameter that will be passed to the callback when it is called.
 * @param	priority	The priority of the callback. The lowest priority is 0.
 */
void ei_bind_internal(ei_eventtype_t eventtype, ei_widget_t widget, ei_tag_t tag, ei_callback_t callback, void *user_param, int priority);

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

/**
 * @brief   Unbinds all the events that have been registered for unbind
 *           by the handling of the last event raised
 */
void ei_unbind_events_registered_for_unbind();

#endif