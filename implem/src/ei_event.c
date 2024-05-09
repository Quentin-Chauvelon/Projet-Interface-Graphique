#include "../api/ei_event.h"
#include "../implem/headers/ei_event_ext.h"
#include "../implem/headers/ei_button.h"

#include "../api/ei_application.h"

ei_event_bind_t *first_event_bind = NULL;

void ei_bind(ei_eventtype_t eventtype, ei_widget_t widget, ei_tag_t tag, ei_callback_t callback, void *user_param)
{
    ei_event_bind_t *event_bind = malloc(sizeof(ei_event_bind_t));

    // If malloc failed, return
    if (event_bind == NULL)
    {
        printf("\033[0;31mError: Couldn't allocate memory to bind event.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        return;
    }

    event_bind->eventtype = eventtype;
    event_bind->widget = widget;
    event_bind->tag = tag;
    event_bind->callback = callback;
    event_bind->user_param = user_param;
    event_bind->next = NULL;

    if (first_event_bind == NULL)
    {
        first_event_bind = event_bind;
    }
    else
    {
        ei_event_bind_t *current = first_event_bind;

        while (current->next != NULL)
        {
            current = current->next;
        }

        current->next = event_bind;
    }
}

void ei_unbind(ei_eventtype_t eventtype, ei_widget_t widget, ei_tag_t tag, ei_callback_t callback, void *user_param)
{
    ei_event_bind_t *current = first_event_bind;
    ei_event_bind_t *previous = NULL;

    while (current != NULL)
    {
        if (current->eventtype == eventtype && current->widget == widget && current->tag == tag && current->callback == callback && current->user_param == user_param)
        {
            // If the event to unbind is the first element of the linked list, set the next element as the first element
            if (previous == NULL)
            {
                first_event_bind = current->next;
            }
            // Otherwise, link the previous element to the next one
            else
            {
                previous->next = current->next;
            }

            free(current);
            break;
        }

        previous = current;
        current = current->next;
    }
}

void ei_unbind_all_events()
{
    ei_event_bind_t *current = first_event_bind;
    ei_event_bind_t *next = NULL;

    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
}

/**
 * @brief Calls the callback function for the given binded event
 *        It also updates the widget's geometry after calling the callback function
 *        in case the callback changed the widget's geometry.
 *
 * @param callback      The callback function to call
 * @param widget        The widget to pass to the callback function
 * @param event         The event to pass to the callback function
 * @param user_param    The user_param to pass to the callback function
 *
 * @return true if the callback returned true (was handled), false otherwise
 */
static bool call_callback_function(ei_callback_t callback, ei_widget_t widget, ei_event_t event, void *user_param)
{
    // Make a copy of the widget since the event might be unbinded in the callback function
    // and so, the widget will point to a random memory address
    ei_widget_t widget_copy = widget;

    // The callback must be called before computing the geometry since it might change the widget's geometry
    bool handled = callback(widget, &event, user_param);

    if (widget_copy->geom_params != NULL)
    {
        widget_copy->geom_params->manager->runfunc(widget_copy);
    }

    return handled;
}

/**
 * @brief Handle an event related to a widget
 *
 * @param binded_event  The event for which to call the callback function (binded via \ref ei_bind)
 * @param event         The last raised event information
 * @param filter_widget If not NULL, only call the callback function if the event's widget is the same as this parameter.
 *                      This is primarly used to filter mouse events that require picking (and should only be called on
 *                      the widget beneath the mouse cursor). If NULL, the callback function will be called the widget
 *
 * @return true if the callback returned true (was handled), false otherwise
 */
static bool handle_widget_event(ei_event_bind_t *binded_event, ei_event_t event, ei_widget_t filter_widget)
{
    // If the widget is not the same as the filter widget, if there is one, return false
    if (filter_widget != NULL && binded_event->widget != filter_widget)
    {
        return false;
    }

    return call_callback_function(binded_event->callback, binded_event->widget, event, binded_event->user_param);
}

/**
 * @brief   Handle an event related to a tag
 *          This implements a tree preorder traversal algorithm.
 *
 * @param widget        The widget to start from
 * @param binded_event  The event for which to call the callback function (binded via \ref ei_bind)
 * @param event         The last raised event information
 * @param filter_widget if not NULL, only call the callback function if the event's widget is the same as this parameter
 *                      This is primarly used to filter mouse events that require picking (and should only be called on
 *                      the widget beneath the mouse cursor). If NULL, the callback function will be called on all widgets
 *                      matching the tag.
 *
 * @return true if the event returned true (was handled), false otherwise
 */
static bool handle_tag_event(ei_widget_t *widget, ei_event_bind_t *binded_event, ei_event_t event, ei_widget_t filter_widget)
{
    if (widget == NULL)
    {
        return false;
    }

    // If the widget is not the same as the filter widget, if there is one, continue to the next widget
    if (filter_widget == NULL || *widget == filter_widget)
    {
        // If the tag is "all" or the tag is the same as the widget class name (button, frame, ...), call the callback function
        if (strcmp(binded_event->tag, "all") == 0 || strcmp(binded_event->tag, (*widget)->wclass->name) == 0)
        {
            bool handled = call_callback_function(binded_event->callback, *widget, event, binded_event->user_param);
            if (handled)
            {
                return true;
            }
        }
    }

    for (ei_widget_t children = (*widget)->children_head; children != NULL; children = children->next_sibling)
    {
        bool handled = handle_tag_event(&children, binded_event, event, filter_widget);

        if (handled)
        {
            return true;
        }
    }

    return false;
}

/**
 * @brief Handle an event raised when the user presses a mouse button
 *        Calls the callback function of the button beneath the mouse cursor if there is one
 *
 * @param event The event information
 *
 * @return true if the event returned true (was handled), false otherwise
 */
static bool handle_mouse_button_down_event(ei_event_t event)
{
    if (picking_widget != NULL && strcmp(picking_widget->wclass->name, "button") == 0)
    {
        ei_button_t *button = (ei_button_t *)picking_widget;

        if (button->callback != NULL)
        {
            return call_callback_function(button->callback, picking_widget, event, button->user_param);
        }
    }

    return false;
}

void ei_handle_event(ei_event_t event)
{
    ei_event_bind_t *current_event = first_event_bind;

    // Keep a reference to the widget beneath the mouse cursor, this way, we don't have
    // to call the function multiple times in the callback functions
    picking_widget = ei_widget_pick(&event.param.mouse.where);

    ei_widget_t filter_widget = NULL;

    // If the event is a mouse down event, use filter_widget to only call the calllback fucntion
    // of the widget beneath the mouse cursor even if other widgets are binded to the event
    // and match the widget/tag.
    // Don't filter other mouse events for now since they don't require picking
    if (event.type == ei_ev_mouse_buttondown || event.type == ei_ev_mouse_buttonup)
    {
        // If there is no widget beneath the mouse cursor, it means that the user clicked on the root widget
        // thus, we want to set the filter_widget to the root widget
        filter_widget = picking_widget == NULL ? ei_app_root_widget() : picking_widget;
    }

    while (current_event != NULL)
    {
        if (current_event->eventtype == event.type)
        {
            // If the event is related to a widget
            if (current_event->widget != NULL)
            {
                if (handle_widget_event(current_event, event, filter_widget))
                {
                    return;
                }
            }

            // If the event is related to a tag
            else if (current_event->tag != NULL)
            {
                ei_widget_t root = ei_app_root_widget();

                if (handle_tag_event(&root, current_event, event, filter_widget))
                {
                    return;
                }
            }
        }

        current_event = current_event->next;
    }

    // User pressed a mouse button
    if (event.type == ei_ev_mouse_buttondown)
    {
        // The return value of this function does not really matter since it is the last handled event
        handle_mouse_button_down_event(event);
    }
}

ei_widget_t ei_get_picking_widget()
{
    return picking_widget;
}