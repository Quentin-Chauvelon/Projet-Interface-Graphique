#include "../api/ei_widget.h"
#include "../api/ei_widgetclass.h"
#include "../api/ei_utils.h"
#include "../api/ei_types.h"
#include "../api/hw_interface.h"
#include "../api/ei_application.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_utils_ext.h"
#include "../implem/headers/ei_application_ext.h"

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

    // Iterate through the tree of widgets to find the one that was clicked
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