#include "../api/ei_widget.h"
#include "../api/ei_widgetclass.h"
#include "../api/ei_utils.h"
#include "../api/ei_types.h"
#include "../api/hw_interface.h"
#include "../api/ei_application.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_utils_ext.h"
#include "../implem/headers/ei_application_ext.h"
#include "../implem/headers/ei_widget_ext.h"

#include "../implem/headers/ei_button.h"

int pick_id = 0;

ei_widget_t ei_widget_create(ei_const_string_t class_name, ei_widget_t parent, ei_user_param_t user_data, ei_widget_destructor_t destructor)
{
    if (parent == NULL)
    {
        if (!TESTING)
        {
            printf("\033[0;31mError: the widget's parent cannot be NULL.\n\t at %s (%s:%d)\n", __func__, __FILE__, __LINE__);
        }
        return NULL;
    }

    return ei_widget_create_internal(class_name, parent, user_data, destructor);
}

ei_widget_t ei_widget_create_internal(ei_const_string_t class_name, ei_widget_t parent, ei_user_param_t user_data, ei_widget_destructor_t destructor)
{
    ei_widgetclass_t *wclass = ei_widgetclass_from_name(class_name);

    ei_widget_t widget = wclass->allocfunc();
    wclass->setdefaultsfunc(widget);

    widget->pick_id = pick_id++;
    widget->pick_color = malloc(sizeof(ei_color_t));
    *(widget->pick_color) = get_color_from_id(widget->pick_id);

    widget->wclass = wclass;

    if (user_data != NULL)
    {
        widget->user_data = &user_data;
    }
    else
    {
        widget->user_data = NULL;
    }

    if (destructor != NULL)
    {
        widget->destructor = destructor;
    }
    else
    {
        widget->destructor = NULL;
    }

    widget->parent = parent;
    widget->children_head = NULL;
    widget->children_tail = NULL;
    widget->next_sibling = NULL;

    if (parent != NULL)
    {
        if (parent->children_head == NULL)
        {
            parent->children_head = widget;
            parent->children_tail = widget;
        }
        else
        {
            parent->children_tail->next_sibling = widget;
            parent->children_tail = widget;
        }
    }

    widget->geom_params = NULL;
    widget->requested_size = ei_size_zero();
    widget->screen_location = ei_rect_zero();
    widget->content_rect = &widget->screen_location;
    widget->preferred_size = ei_size_zero();

    return widget;
}

void ei_widget_destroy(ei_widget_t widget)
{
    // If the widget being destroyed is the root widget and the app is still running, quit the application
    if (widget == ei_app_root_widget() && ei_is_app_running())
    {
        ei_app_quit_request();
        return;
    }

    ei_geometrymanager_unmap(widget);

    // If it is not the root widget
    if (widget->parent != NULL)
    {
        // Remove the widget from the widget tree
        // If it is the first children, point to the next sibling
        if (widget->parent->children_head == widget)
        {
            widget->parent->children_head = widget->next_sibling;
        }
        // If it is not the first children, find the previous widget and point to the next sibling
        else
        {
            ei_widget_t current = widget->parent->children_head;
            while (current->next_sibling != widget)
            {
                current = current->next_sibling;
            }

            current->next_sibling = widget->next_sibling;

            // If it is the last child, point the tail to the previous widget
            if (widget->parent->children_tail == widget)
            {
                widget->parent->children_tail = current;
            }
        }

        // If it is the last child, remove the pointer to the tail
        if (widget->parent->children_tail == widget)
        {
            widget->parent->children_tail = NULL;
        }
    }

    // Destroy all the descendants
    if (widget->children_head != NULL)
    {
        ei_widget_t current = widget->children_head;
        ei_widget_t next_sibling = NULL;

        while (current->next_sibling != NULL)
        {
            next_sibling = current->next_sibling;
            ei_widget_destroy(current);
            current = next_sibling;
        }

        ei_widget_destroy(current);
    }

    if (widget->destructor != NULL)
    {
        widget->destructor(widget);
    }

    // Don't free the geom_params field since it has already be freed in the geometry manager's unmap function

    free(widget->pick_color);

    if (widget->user_data != NULL)
    {
        free(widget->user_data);
    }

    if (widget->destructor != NULL)
    {
        free(widget->destructor);
    }

    widget->wclass->releasefunc(widget);
    widget = NULL;
}

bool ei_widget_is_displayed(ei_widget_t widget)
{
    return widget->geom_params != NULL;
}

ei_widget_t ei_widget_pick(ei_point_t *where)
{
    ei_surface_t offscreen_picking = ei_app_offscreen_picking_surface();

    uint8_t *buffer = hw_surface_get_buffer(offscreen_picking);

    // Each pixel is stored on 4 consecutive bytes with one byte for each component, with
    // the order depending on hw_surface_get_channel_indices.
    // So in order to access the 4 components of the first pixel,
    // we can simply do buffer[0], buffer[1], buffer[2] and buffer[3].
    // And so, since pixels are stored row by row, to access the first component of the
    // pixel at (x, y) coordinates, we can do buffer[(y * width + x) * 4] where
    // y * width allows us to move to the right row and + x to the right column,
    // then we multiply by 4 because each pixel is stored on 4 bytes.
    uint32_t offset = (where->y * hw_surface_get_size(offscreen_picking).width + where->x) * 4;

    int ir = 0;
    int ig = 0;
    int ib = 0;
    int ia = 0;
    hw_surface_get_channel_indices(offscreen_picking, &ir, &ig, &ib, &ia);

    // offset now points to the first component of the pixel, so in order to access the 4
    // components, we can simply use the indices returned by hw_surface_get_channel_indices.
    uint8_t red = buffer[offset + ir];
    uint8_t green = buffer[offset + ig];
    uint8_t blue = buffer[offset + ib];
    uint8_t alpha = buffer[offset + ia];

    int id = get_id_from_color((ei_color_t){red, green, blue, alpha});

    ei_widget_t current = ei_app_root_widget();

    // If the user clicked on the root widget, return NULL
    if (current->pick_id == id)
    {
        return NULL;
    }

    // Traverse the tree of widgets to find the one that was clicked
    while (true)
    {
        if (current->pick_id == id)
        {
            return current;
        }

        if (current->next_sibling != NULL)
        {
            current = current->next_sibling;
        }
        else if (current->children_head != NULL)
        {
            current = current->children_head;
        }
        else
        {
            break;
        }
    }

    return NULL;
}