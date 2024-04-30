#include "../api/ei_widget.h"
#include "../api/ei_widgetclass.h"
#include "../implem/ei_implementation.h"

ei_widget_t ei_widget_create(ei_const_string_t class_name,
                             ei_widget_t parent,
                             ei_user_param_t user_data,
                             ei_widget_destructor_t destructor)
{
    ei_widget_t widget = malloc(sizeof(ei_impl_widget_t));

    widget->wclass = ei_widgetclass_from_name(class_name);
    widget->user_data = &user_data;
    widget->destructor = destructor;

    widget->parent = parent;
    widget->children_head = NULL;
    widget->children_tail = NULL;
    widget->next_sibling = NULL;

    if (parent != NULL)
    {
        parent->children_head = widget;
    }

    widget->geom_params = NULL;
    // widget->requested_size = parent->requested_size;
    // widget->screen_location = parent->screen_location;
    // widget->content_rect = NULL;

    return widget;
}