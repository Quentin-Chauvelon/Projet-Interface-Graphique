#include <math.h>

#include "../api/ei_types.h"
#include "../api/ei_event.h"
#include "../api/ei_utils.h"
#include "../api/ei_placer.h"
#include "../api/ei_application.h"
#include "../implem/headers/ei_button.h"
#include "../implem/headers/ei_toplevel.h"
#include "../implem/headers/ei_event_ext.h"
#include "../implem/headers/ei_internal_callbacks.h"
#include "../implem/headers/ei_application_ext.h"
#include "../implem/headers/ei_utils_ext.h"
#include "../implem/headers/ei_placer_ext.h"

typedef struct ei_move_top_level_params_t
{
    ei_widget_t widget;
    ei_point_t offset;
} ei_move_top_level_params_t;

static bool ei_button_press(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    ei_button_t *button = (ei_button_t *)widget;
    button->frame_appearance.relief = ei_relief_sunken;

    // Bind the button release and move event
    // The button should be released if the user releases the mouse button
    // or if they move the cursor out of the button
    ei_bind(ei_ev_mouse_buttonup, widget, NULL, ei_button_release, NULL);
    ei_bind(ei_ev_mouse_move, widget, NULL, ei_cursor_left_button, NULL);

    return false;
}

static bool ei_button_release(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    ei_button_t *button = (ei_button_t *)widget;
    button->frame_appearance.relief = ei_relief_raised;

    // Unbind the button release and move event
    ei_unbind(ei_ev_mouse_buttonup, widget, NULL, ei_button_release, NULL);
    ei_unbind(ei_ev_mouse_move, widget, NULL, ei_cursor_left_button, NULL);

    return false;
}

static bool ei_cursor_left_button(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    // If the widget beneath the cursor is not the button, it means the user moved
    // the cursor out of the button, and so, we want to release the button
    if (ei_get_picking_widget() != widget)
    {
        return ei_button_release(widget, event, user_param);
    }

    return false;
}

static bool ei_toplevel_pressed(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    ei_toplevel_t *toplevel = (ei_toplevel_t *)widget;

    ei_point_t mouse_position = event->param.mouse.where;

    if (toplevel->closable)
    {
        ei_rect_t close_button_rect = toplevel->close_button->widget.screen_location;

        // If the user clicked on the close button, close the toplevel
        if (ei_is_point_in_circle(mouse_position, ei_point(close_button_rect.top_left.x + (close_button_rect.size.width / 2), close_button_rect.top_left.y + (close_button_rect.size.height / 2)), close_button_rect.size.width / 2))
        {
            ei_widget_destroy(widget);

            return true;
        }
    }

    ei_rect_t title_bar = ei_toplevel_get_title_bar_rect(toplevel);

    // If the user clicked on the title bar
    if (mouse_position.x >= title_bar.top_left.x && mouse_position.x <= title_bar.top_left.x + title_bar.size.width && mouse_position.y >= title_bar.top_left.y && mouse_position.y <= title_bar.top_left.y + title_bar.size.height)
    {
        // Move the toplevel to the foreground
        if (widget != ei_app_root_widget()->children_tail)
        {
            if (widget == ei_app_root_widget()->children_head)
            {
                ei_app_root_widget()->children_head = widget->next_sibling;
            }
            else
            {
                ei_widget_t previous = ei_app_root_widget()->children_head;

                while (previous->next_sibling != widget)
                {
                    previous = previous->next_sibling;
                }

                previous->next_sibling = widget->next_sibling;
            }

            ei_app_root_widget()->children_tail->next_sibling = widget;
            ei_app_root_widget()->children_tail = widget;
            widget->next_sibling = NULL;
        }

        ei_move_top_level_params_t *params = malloc(sizeof(ei_move_top_level_params_t));
        params->widget = widget;

        // Store the position of the cursor relative to the left corner of the top level,
        // because when we are going to move it, we will need to keep the same offset
        params->offset.x = mouse_position.x - toplevel->widget.screen_location.top_left.x;
        params->offset.y = mouse_position.y - toplevel->widget.screen_location.top_left.y;

        ei_bind(ei_ev_mouse_move, NULL, "all", ei_toplevel_move, params);
        ei_bind(ei_ev_mouse_buttonup, NULL, "all", ei_toplevel_move_released, params);

        return true;
    }

    // If the user clicked on the resize square
    if (toplevel->resizable != ei_axis_none)
    {
        ei_rect_t resizable_square = ei_toplevel_get_resize_square_rect(toplevel);

        if (mouse_position.x >= resizable_square.top_left.x && mouse_position.x <= resizable_square.top_left.x + resizable_square.size.width && mouse_position.y >= resizable_square.top_left.y && mouse_position.y <= resizable_square.top_left.y + resizable_square.size.height)
        {
            ei_bind(ei_ev_mouse_move, NULL, "all", ei_toplevel_resize, toplevel);
            ei_bind(ei_ev_mouse_buttonup, NULL, "all", ei_toplevel_resize_released, toplevel);

            return true;
        }
    }

    return false;
}

static bool ei_toplevel_move(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    if (user_param == NULL)
    {
        return false;
    }

    ei_move_top_level_params_t params = *((ei_move_top_level_params_t *)user_param);

    if (widget != params.widget)
    {
        return false;
    }

    ei_toplevel_t *toplevel = (ei_toplevel_t *)widget;

    if (toplevel->widget.geom_params != NULL)
    {
        if (strcmp(toplevel->widget.geom_params->manager->name, "placer") == 0)
        {
            ei_placer_t *geom_params = (ei_placer_t *)toplevel->widget.geom_params;

            // If the toplevel is going to clip out of the top or left side, don't move it
            if (event->param.mouse.where.x - params.offset.x >= 0)
            {
                geom_params->x = event->param.mouse.where.x - params.offset.x;
            }

            if (event->param.mouse.where.y - params.offset.y >= 0)
            {
                geom_params->y = event->param.mouse.where.y - params.offset.y;
            }

            return true;
        }
    }

    return false;
}

static bool ei_toplevel_move_released(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    ei_unbind(ei_ev_mouse_move, NULL, "all", ei_toplevel_move, user_param);
    ei_unbind(ei_ev_mouse_buttonup, NULL, "all", ei_toplevel_move_released, user_param);

    return true;
}

static bool ei_toplevel_resize(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    if (widget != user_param)
    {
        return false;
    }

    ei_toplevel_t *toplevel = (ei_toplevel_t *)widget;

    // Update the geom parameters so that the geometry manager can recompute the widget's geometry
    if (toplevel->widget.geom_params != NULL)
    {
        if (strcmp(toplevel->widget.geom_params->manager->name, "placer") == 0)
        {
            ei_placer_t *geom_params = (ei_placer_t *)toplevel->widget.geom_params;

            geom_params->width = event->param.mouse.where.x - toplevel->widget.screen_location.top_left.x;
            geom_params->height = event->param.mouse.where.y - toplevel->widget.screen_location.top_left.y;

            // Resize the top level to the minimum size if it is smaller than the minimum size
            if (geom_params->width < (*toplevel->min_size).width)
            {
                geom_params->width = (*toplevel->min_size).width;
            }

            if (geom_params->height < (*toplevel->min_size).height)
            {
                geom_params->height = (*toplevel->min_size).height;
            }

            return true;
        }
    }

    return false;
}

static bool ei_toplevel_resize_released(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    ei_unbind(ei_ev_mouse_move, NULL, "all", ei_toplevel_resize, user_param);
    ei_unbind(ei_ev_mouse_buttonup, NULL, "all", ei_toplevel_resize_released, user_param);

    return false;
}

/**
 * @brief   Update the pick color based on a certain multiplier of the pick id
 *          This implements a tree preorder traversal algorithm.
 *
 * @param   widget      The widget to update the pick color for
 * @param   multiplier  The multiplier for the pick id
 */
static void ei_update_pick_color_from_id(ei_widget_t *widget, int multiplier)
{
    if (widget == NULL)
    {
        return;
    }

    // Update the pick color
    *(*widget)->pick_color = ei_get_color_from_id((*widget)->pick_id * multiplier);

    for (ei_widget_t children = (*widget)->children_head; children != NULL; children = children->next_sibling)
    {
        ei_update_pick_color_from_id(&children, multiplier);
    }

    return;
}

static bool toggle_offscreen_picking_surface_display(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    if (event->param.key_code == SDLK_g)
    {
        // Custom parameter to act as a backup of the root surface so that
        // it can be redrawn when turning off the offscreen picking surface.
        // Checking if the surface is NULL also allows to know if we should
        // toggle on or off the offscreen picking surface.
        ei_surface_t *root_surface_copy = (ei_surface_t *)user_param;

        ei_widget_t root = ei_app_root_widget();
        ei_update_pick_color_from_id(&root, *root_surface_copy != NULL ? 1 : 1000000);

        ei_app_root_widget()->wclass->drawfunc(ei_app_root_widget(), ei_app_root_surface(), ei_app_offscreen_picking_surface(), &widget->screen_location);

        hw_surface_lock(ei_app_root_surface());
        hw_surface_lock(ei_app_offscreen_picking_surface());

        // If the offscreen picking surface is not displayed, make a copy of the root surface
        // and display the offscreen picking surface
        if (*root_surface_copy == NULL)
        {
            printf("Displaying offscreen picking surface\n");
            *root_surface_copy = hw_surface_create(ei_app_root_surface(), hw_surface_get_size(ei_app_root_surface()), true);
            ei_copy_surface(*root_surface_copy, NULL, ei_app_root_surface(), NULL, false);

            ei_copy_surface(ei_app_root_surface(), NULL, ei_app_offscreen_picking_surface(), NULL, false);
        }
        // Otherwise, restore the root surface and delete the copy
        else
        {
            printf("Hiding offscreen picking surface\n");
            ei_copy_surface(ei_app_root_surface(), NULL, *root_surface_copy, NULL, false);

            hw_surface_free(*root_surface_copy);
            *root_surface_copy = NULL;
        }

        hw_surface_unlock(ei_app_root_surface());
        hw_surface_unlock(ei_app_offscreen_picking_surface());

        hw_surface_update_rects(ei_app_root_surface(), NULL);

        return true;
    }

    return false;
}

void ei_bind_all_internal_callbacks()
{
    ei_bind(ei_ev_mouse_buttondown, NULL, "button", ei_button_press, NULL);
    ei_bind(ei_ev_mouse_buttondown, NULL, "toplevel", ei_toplevel_pressed, NULL);

    ei_surface_t root_surface_copy = NULL;
    ei_bind(ei_ev_keydown, NULL, "all", toggle_offscreen_picking_surface_display, &root_surface_copy);
}

void test()
{
    toggle_offscreen_picking_surface_display(ei_app_root_widget(), NULL, NULL);
}