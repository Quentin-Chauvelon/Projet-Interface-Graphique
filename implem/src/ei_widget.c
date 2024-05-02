#include "../api/ei_widget.h"
#include "../api/ei_widgetclass.h"
#include "../api/ei_utils.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_utils_ext.h"

int pick_id = 0;

ei_widget_t ei_widget_create(ei_const_string_t class_name,
                             ei_widget_t parent,
                             ei_user_param_t user_data,
                             ei_widget_destructor_t destructor)
{
    ei_widgetclass_t *wclass = ei_widgetclass_from_name(class_name);

    ei_widget_t widget = wclass->allocfunc();
    wclass->setdefaultsfunc(widget);

    widget->pick_id = pick_id++;
    widget->pick_color = malloc(sizeof(ei_color_t));
    *(widget->pick_color) = get_color_from_id(widget->pick_id);

    widget->wclass = wclass;

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
    widget->requested_size = ei_size(0, 0);
    widget->screen_location = ei_rect(ei_point(0, 0), widget->requested_size);
    widget->content_rect = &widget->screen_location;

    return widget;
}