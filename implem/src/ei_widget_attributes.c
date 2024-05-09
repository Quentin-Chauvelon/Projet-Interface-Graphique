#include "../api/ei_widget.h"
#include "../api/ei_types.h"
#include "../api/ei_widget_attributes.h"
#include "../api/ei_widget_configure.h"
#include "../implem/headers/ei_implementation.h"

ei_widgetclass_t *ei_widget_get_class(ei_widget_t widget)
{
    return widget->wclass;
}

const ei_color_t *ei_widget_get_pick_color(ei_widget_t widget)
{
    return (*widget).pick_color;
}

ei_widget_t ei_widget_get_parent(ei_widget_t widget)
{
    return (*widget).parent;
}

ei_widget_t ei_widget_get_first_child(ei_widget_t widget)
{
    return (*widget).children_head;
}

ei_widget_t ei_widget_get_last_child(ei_widget_t widget)
{
    return (*widget).children_tail;
}

ei_widget_t ei_widget_get_next_sibling(ei_widget_t widget)
{
    return (*widget).next_sibling;
}

void *ei_widget_get_user_data(ei_widget_t widget)
{
    return (*widget).user_data;
}

const ei_size_t *ei_widget_get_requested_size(ei_widget_t widget)
{
    return &(widget->requested_size);
}

void ei_widget_set_requested_size(ei_widget_t widget, ei_size_t requested_size)
{
    if (strcmp(widget->wclass->name, "frame") == 0)
    {
        ei_frame_configure(widget, &requested_size, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    else if (strcmp(widget->wclass->name, "button") == 0)
    {
        ei_button_configure(widget, &requested_size, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    else if (strcmp(widget->wclass->name, "toplevel") == 0)
    {
        ei_toplevel_configure(widget, &requested_size, NULL, NULL, NULL, NULL, NULL, NULL);
    }
}

const ei_rect_t *ei_widget_get_screen_location(ei_widget_t widget)
{
    return &(widget->screen_location);
}

const ei_rect_t *ei_widget_get_content_rect(ei_widget_t widget)
{
    return widget->content_rect;
}

void ei_widget_set_content_rect(ei_widget_t widget, const ei_rect_t *content_rect)
{
    if (strcmp(widget->wclass->name, "frame") == 0)
    {
        ei_frame_configure(widget, &((ei_rect_t *)content_rect)->size, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    else if (strcmp(widget->wclass->name, "button") == 0)
    {
        ei_button_configure(widget, &((ei_rect_t *)content_rect)->size, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    else if (strcmp(widget->wclass->name, "toplevel") == 0)
    {
        ei_toplevel_configure(widget, &((ei_rect_t *)content_rect)->size, NULL, NULL, NULL, NULL, NULL, NULL);
    }

    widget->content_rect->top_left = content_rect->top_left;
}
