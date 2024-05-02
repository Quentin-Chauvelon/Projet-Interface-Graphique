#include "../api/ei_event.h"
#include "../implem/headers/ei_event_ext.h"
#include "../implem/headers/ei_button.h"

#include "../api/ei_application.h"

ei_event_bind_t *first_event_bind = NULL;

void ei_bind(ei_eventtype_t eventtype, ei_widget_t widget, ei_tag_t tag, ei_callback_t callback, void *user_param)
{
    ei_event_bind_t *event_bind = malloc(sizeof(ei_event_bind_t));
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

void handle_event(ei_event_t event)
{
    ei_event_bind_t *current = first_event_bind;

    while (current != NULL)
    {
        if (current->eventtype == event.type)
        {
            // If the event is related to a widget
            if (current->widget != NULL)
            {
                if (current->callback(current->widget, &event, current->user_param))
                {
                    return;
                }
            }

            // If the event is related to a tag
            else if (current->tag != NULL)
            {
                ei_widget_t current_widget = ei_app_root_widget();

                // Traverse the widget tree
                while (true)
                {
                    // If the tag is "all" or the tag is the same as the widget class name (button, frame, ...), call the callback function
                    if (strcmp(current->tag, "all") == 0 || strcmp(current->tag, current_widget->wclass->name) == 0)
                    {
                        if (current->callback(current_widget, &event, current->user_param))
                        {
                            return;
                        }
                    }

                    if (current_widget->next_sibling != NULL)
                    {
                        current_widget = current_widget->next_sibling;
                    }
                    else if (current_widget->children_head != NULL)
                    {
                        current_widget = current_widget->children_head;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }

        current = current->next;
    }

    // User pressed a mouse button
    if (event.type == ei_ev_mouse_buttondown)
    {
        ei_widget_t widget = ei_widget_pick(&event.param.mouse.where);

        // If the user clicked on a button, call its callback function
        if (widget != NULL && strcmp(widget->wclass->name, "button") == 0)
        {
            ei_button_t *button = (ei_button_t *)widget;

            if (button->callback != NULL)
            {
                button->callback(widget, &event, button->user_param);
            }
        }
    }
}