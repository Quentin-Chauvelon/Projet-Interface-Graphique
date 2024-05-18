#include "../api/ei_widget.h"
#include "../api/ei_widgetclass.h"
#include "../api/ei_utils.h"
#include "../api/ei_types.h"
#include "../api/ei_event.h"
#include "../api/hw_interface.h"
#include "../api/ei_application.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_utils_ext.h"
#include "../implem/headers/ei_application_ext.h"
#include "../implem/headers/ei_widget_ext.h"
#include "../implem/headers/ei_draw_ext.h"
#include "../implem/headers/ei_toplevel.h"
#include "../implem/headers/ei_types_ext.h"

int pick_id = 0;

ei_widget_t ei_widget_create(ei_const_string_t class_name, ei_widget_t parent, ei_user_param_t user_data, ei_widget_destructor_t destructor)
{
    if (parent == NULL)
    {
        if (!TESTING)
        {
            printf("\033[0;31mError: the widget's parent cannot be NULL.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        }

        return NULL;
    }

    return ei_widget_create_internal(class_name, parent, user_data, destructor);
}

ei_widget_t ei_widget_create_internal(ei_const_string_t class_name, ei_widget_t parent, ei_user_param_t user_data, ei_widget_destructor_t destructor)
{
    ei_widgetclass_t *wclass = ei_widgetclass_from_name(class_name);

    // If the widget class doesn't exist, exit since the program can't run without it
    if (wclass == NULL)
    {
        printf("\033[0;31mError: the widget class %s doesn't exist.\n\t at %s (%s:%d)\033[0m\n", class_name, __func__, __FILE__, __LINE__);
        exit(1);
    }

    ei_widget_t widget = wclass->allocfunc();

    widget->pick_id = pick_id++;
    widget->pick_color = malloc(sizeof(ei_color_t));

    // If malloc failed, exit since the program can't run without the pick color
    if (widget->pick_color == NULL)
    {
        printf("\033[0;31mError: Couldn't allocate memory for the widget's pick color.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        exit(1);
    }

    *(widget->pick_color) = ei_get_color_from_id(widget->pick_id);

    widget->wclass = wclass;

    if (user_data != NULL)
    {
        widget->user_data = user_data;
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

    widget->content_rect = malloc(sizeof(ei_rect_t));

    // If malloc failed, exit since the program can't run without the content rect
    if (widget->content_rect == NULL)
    {
        printf("\033[0;31mError: Couldn't allocate memory for widget's content rect.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        exit(1);
    }

    widget->content_rect->top_left = widget->screen_location.top_left;
    widget->content_rect->size = widget->screen_location.size;

    // Add the name of the widget to its tag list by default
    ei_tag_t tag = malloc(strlen(wclass->name) + 1);
    strcpy(tag, wclass->name);

    ei_add_tag_to_widget(widget, tag);

    free(tag);

    wclass->setdefaultsfunc(widget);

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
        // If the widget is not the close button of a toplevel, update the hierarchy
        if (strcmp(widget->parent->wclass->name, "toplevel") != 0 ||
            ((ei_toplevel_t *)widget->parent)->close_button == NULL ||
            (ei_widget_t *)((ei_toplevel_t *)widget->parent)->close_button != (ei_widget_t *)widget)
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

    if (widget->content_rect != NULL)
    {
        free(widget->content_rect);
    }

    if (widget->destructor != NULL)
    {
        widget->destructor(widget);
    }

    while (widget->tags != NULL)
    {
        ei_remove_tag_from_widget(widget, widget->tags->tag);
    }

    // Don't free the geom_params field since it has already be freed in the geometry manager's unmap function

    free(widget->pick_color);

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

    hw_surface_lock(offscreen_picking);

    uint8_t *buffer = hw_surface_get_buffer(offscreen_picking);

    hw_surface_unlock(offscreen_picking);

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

    int id = ei_get_id_from_color((ei_color_t){red, green, blue, alpha});

    ei_widget_t root = ei_app_root_widget();
    ei_widget_t result = ei_get_widget_from_pick_id(&root, id);

    return result;
}

ei_widget_t ei_get_widget_from_pick_id(ei_widget_t *widget, int pick_id)
{
    if (widget == NULL)
    {
        return NULL;
    }

    if ((*widget)->pick_id == pick_id)
    {
        return *widget;
    }

    for (ei_widget_t children = (*widget)->children_head; children != NULL; children = children->next_sibling)
    {
        ei_widget_t result = ei_get_widget_from_pick_id(&children, pick_id);

        // If we found the right widget, return it
        if (result != NULL)
        {
            return result;
        }
    }

    return NULL;
}

ei_widget_t ei_widget_allocfunc(size_t widget_size)
{
    ei_widget_t widget = malloc(widget_size);

    // If malloc failed, exit since the program will crash if the user tries to use the widget
    if (widget == NULL)
    {
        printf("\033[0;31mError: Couldn't allocate memory for the widget widget.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        exit(1);
    }

    memset(widget, 0, widget_size);

    return widget;
}

void ei_draw_frame_appearance(ei_surface_t surface, ei_widget_t widget, ei_text_properties_t text, ei_image_properties_t image, const ei_rect_t *clipper)
{
    // Draw the text
    if (text.label != NULL)
    {
        int width = 0;
        int height = 0;
        hw_text_compute_size(text.label, text.font, &width, &height);

        ei_point_t where = ei_get_position_in_parent_from_anchor(*widget->content_rect, ei_size(width, height), text.anchor);

        ei_draw_text(surface, &where, text.label, text.font, text.color, clipper);
    }

    // Draw the image
    if (image.data != NULL)
    {
        ei_size_t original_image_size = hw_surface_get_size(image.data);

        ei_size_t image_size = image.rect == NULL
                                   ? original_image_size
                                   : image.rect->size;

        ei_rect_t image_rect = image.rect != NULL
                                   ? *image.rect
                                   : ei_rect(ei_point_zero(), image_size);

        // Crop the image to only get the subpart defined by image rect
        image_rect = ei_get_intersection_rectangle(image_rect, ei_rect(ei_point_zero(), original_image_size));

        // If the image is not contained in the cropped image (outside of image rec),
        // reset the image to its original size and remove the cropping
        if (image_rect.top_left.x == 0 && image_rect.top_left.y == 0 && image_rect.size.width == 0 && image_rect.size.height == 0)
        {
            image_rect = ei_rect(ei_point_zero(), original_image_size);
        }

        ei_point_t where = ei_get_position_in_parent_from_anchor(*widget->content_rect, image_rect.size, image.anchor);

        ei_draw_image(surface, image.data, &image_rect, &where, clipper);
    }
}

ei_rect_t ei_get_children_clipper(ei_rect_t content_rect, const ei_rect_t *clipper)
{
    return clipper != NULL
               ? ei_get_intersection_rectangle(content_rect, *clipper)
               : content_rect;
}

void ei_calculate_frame_appearance_natural_size(ei_frame_appearance_t frame_appearance, int border_width, ei_size_t *size)
{
    if (border_width > 0)
    {
        size->width += border_width * 2;
        size->height += border_width * 2;
    }

    if (frame_appearance.text.label != NULL)
    {
        int width = 0;
        int height = 0;
        hw_text_compute_size(frame_appearance.text.label, frame_appearance.text.font, &width, &height);

        size->width += width;
        size->height += height;
    }

    if (frame_appearance.image.data != NULL)
    {
        ei_size_t image_size = frame_appearance.image.rect == NULL
                                   ? hw_surface_get_size(frame_appearance.image.data)
                                   : frame_appearance.image.rect->size;

        size->width += image_size.width;
        size->height += image_size.height;
    }
}

void ei_add_tag_to_widget(ei_widget_t widget, ei_tag_t tag)
{
    ei_widget_tags_t *widget_tag = malloc(sizeof(ei_widget_tags_t));

    // If malloc failed, exit since the program can't run without the tag
    if (widget_tag == NULL)
    {
        printf("\033[0;31mError: Couldn't allocate memory for tag.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        exit(1);
    }

    widget_tag->tag = malloc(sizeof(char) * (strlen(tag) + 1));
    strcpy(widget_tag->tag, tag);

    widget_tag->next = NULL;

    if (widget->tags == NULL)
    {
        widget->tags = widget_tag;
    }
    else
    {
        // Add the tag at the end of the linked list of the tags applied to the widget
        for (ei_widget_tags_t *current = widget->tags; current != NULL; current = current->next)
        {
            if (current->next == NULL)
            {
                current->next = widget_tag;
                return;
            }
        }
    }
}

void ei_remove_tag_from_widget(ei_widget_t widget, ei_tag_t tag)
{
    ei_widget_tags_t *current = widget->tags;

    if (strcmp(current->tag, tag) == 0)
    {
        widget->tags = current->next;

        free(current->tag);
        free(current);

        return;
    }

    ei_widget_tags_t *previous = NULL;

    while (current != NULL)
    {
        if (strcmp(current->tag, tag) == 0)
        {
            previous->next = current->next;

            free(current->next);

            return;
        }

        previous = current;
        current = current->next;
    }
}