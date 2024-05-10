#include <math.h>

#include "../api/ei_types.h"
#include "../api/ei_event.h"
#include "../api/ei_utils.h"
#include "../api/ei_placer.h"
#include "../api/ei_application.h"
#include "../api/ei_entry.h"
#include "../api/hw_interface.h"
#include "../implem/headers/ei_button.h"
#include "../implem/headers/ei_toplevel.h"
#include "../implem/headers/ei_entry.h"
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
        ei_point_t close_button_center = ei_point(close_button_rect.top_left.x + (close_button_rect.size.width / 2), close_button_rect.top_left.y + (close_button_rect.size.height / 2));

        // If the user clicked on the close button, close the toplevel
        if (ei_is_point_in_circle(mouse_position, close_button_center, close_button_rect.size.width / 2))
        {
            ei_widget_destroy(widget);

            return true;
        }
    }

    ei_rect_t title_bar = ei_toplevel_get_title_bar_rect(toplevel);

    // If the user clicked on the title bar
    if (mouse_position.x >= title_bar.top_left.x &&
        mouse_position.x <= title_bar.top_left.x + title_bar.size.width &&
        mouse_position.y >= title_bar.top_left.y &&
        mouse_position.y <= title_bar.top_left.y + title_bar.size.height)
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

        // If malloc failed, exit since the program can't run without the event
        if (params == NULL)
        {
            printf("\033[0;31mError: Couldn't allocate memory to handle toplevel click.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
            exit(1);
        }

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

        if (mouse_position.x >= resizable_square.top_left.x &&
            mouse_position.x <= resizable_square.top_left.x + resizable_square.size.width &&
            mouse_position.y >= resizable_square.top_left.y &&
            mouse_position.y <= resizable_square.top_left.y + resizable_square.size.height)
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

    if (ei_widget_is_displayed(&toplevel->widget))
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
    if (ei_widget_is_displayed(&toplevel->widget))
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

static bool ei_entry_pressed(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    ei_entry_t *entry = (ei_entry_t *)widget;

    entry->cursor = ei_get_character_at_position(entry, event->param.mouse.where);

    ei_entry_give_focus(widget);

    return false;
}

bool ei_entry_keyboard_key_down(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    ei_entry_t *entry = (ei_entry_t *)widget;

    bool handled = false;

    // Unfocus the entry if the user presses the escape key
    if (event->param.key_code == SDLK_RETURN)
    {
        ei_entry_release_focus(widget);

        handled = true;
    }
    else if (event->param.key_code == SDLK_TAB)
    {
        // Focus the previous entry if the user presses shift+tab
        if (ei_mask_has_modifier(event->modifier_mask, ei_mod_shift_left))
        {
            if (entry->previous != NULL)
            {
                // If the cursor is not over any character, put it to the first one
                if (entry->previous->cursor == NULL)
                {
                    entry->previous->cursor = entry->previous->first_character;
                }

                ei_entry_give_focus(&entry->previous->widget);

                handled = true;
            }
        }
        // Focus the next entry if the user presses tab
        else
        {
            if (entry->next != NULL)
            {
                // If the cursor is not over any character, put it to the first one
                if (entry->next->cursor == NULL)
                {
                    entry->next->cursor = entry->next->first_character;
                }

                ei_entry_give_focus(&entry->next->widget);

                handled = true;
            }
        }
    }
    else if (event->param.key_code == SDLK_RIGHT)
    {
        // Move to the next character is there is one
        if (entry->cursor != NULL && entry->cursor->next != NULL)
        {
            // if the user also pressed ctrl, move to the next word
            if (ei_mask_has_modifier(event->modifier_mask, ei_mod_ctrl_left) ||
                ei_mask_has_modifier(event->modifier_mask, ei_mod_ctrl_right))
            {
                // If the first next character is a space, skip it
                if (entry->cursor->next != NULL && entry->cursor->next->character == ' ')
                {
                    entry->cursor = entry->cursor->next;
                }

                while (entry->cursor->next)
                {
                    if (entry->cursor->next->character == ' ')
                    {
                        break;
                    }

                    entry->cursor = entry->cursor->next;
                }
            }
            // Otherwise, move to the next character
            else
            {
                entry->cursor = entry->cursor->next;
            }
        }

        handled = true;
    }
    else if (event->param.key_code == SDLK_LEFT)
    {
        // if the user also pressed ctrl, move to the previous word
        if (ei_mask_has_modifier(event->modifier_mask, ei_mod_ctrl_left) ||
            ei_mask_has_modifier(event->modifier_mask, ei_mod_ctrl_right))
        {
            // If the first previous character is a space, skip it
            if (entry->cursor->previous != NULL && entry->cursor->previous->character == ' ')
            {
                entry->cursor = entry->cursor->previous;
            }

            while (entry->cursor->previous)
            {
                if (entry->cursor->previous->character == ' ')
                {
                    entry->cursor = entry->cursor->previous;
                    break;
                }

                entry->cursor = entry->cursor->previous;
            }
        }
        // Otherwise, move to the previous character
        else
        {
            entry->cursor = entry->cursor->previous;
        }

        // If the cursor is pointing not pointing anything,
        // move it back to the fake character
        if (entry->cursor == NULL)
        {
            entry->cursor = entry->first_character;
        }

        handled = true;
    }
    // Move to the first character
    else if (event->param.key_code == SDLK_HOME)
    {
        entry->cursor = entry->first_character;

        handled = true;
    }
    // move to the last character
    else if (event->param.key_code == SDLK_END)
    {
        entry->cursor = entry->last_character;

        handled = true;
    }
    // Erase the character at cursor position
    else if (event->param.key_code == SDLK_BACKSPACE)
    {
        // If the user also pressed ctrl, delete the whole word
        if (ei_mask_has_modifier(event->modifier_mask, ei_mod_ctrl_left) ||
            ei_mask_has_modifier(event->modifier_mask, ei_mod_ctrl_right))
        {
            while (entry->cursor->previous)
            {
                if (entry->cursor->previous->character == ' ')
                {
                    ei_entry_erase_character(entry, entry->cursor);
                    break;
                }

                ei_entry_erase_character(entry, entry->cursor);
            }
        }
        // Otherwise, only erase the previous character
        else
        {
            ei_entry_erase_character(entry, entry->cursor);
        }

        handled = true;
    }
    // Erase the character after the cursor
    else if (event->param.key_code == SDLK_DELETE)
    {
        // If the user also pressed ctrl, delete the whole word
        if (ei_mask_has_modifier(event->modifier_mask, ei_mod_ctrl_left) ||
            ei_mask_has_modifier(event->modifier_mask, ei_mod_ctrl_right))
        {
            // If the first next character is a space, erase it
            if (entry->cursor->next != NULL && entry->cursor->next->character == ' ')
            {
                ei_entry_erase_character(entry, entry->cursor->next);
            }

            while (entry->cursor->next)
            {
                if (entry->cursor->next == NULL)
                {
                    break;
                }

                if (entry->cursor->next->character == ' ')
                {
                    break;
                }

                ei_entry_erase_character(entry, entry->cursor->next);
            }
        }
        // Otherwise, only erase the previous character
        else if (entry->cursor->next)
        {
            ei_entry_erase_character(entry, entry->cursor->next);
        }

        handled = true;
    }
    else
    {
        // If the key pressed in a displayable character, add it to the entry
        if (event->param.key_code >= 32 && event->param.key_code <= 126)
        {
            // If the shift key is pressed, make the letter uppercase
            if (ei_mask_has_modifier(event->modifier_mask, ei_mod_shift_left))
            {
                event->param.key_code = toupper(event->param.key_code);
            }

            ei_entry_add_character(entry, event->param.key_code);

            handled = true;
        }
    }

    // Reset the blinking timer if the key press has been handled by the event
    if (handled)
    {
        ei_restart_blinking_timer(entry, true);
    }

    return handled;
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
    if (event->param.key_code == SDLK_g && ei_mask_has_modifier(event->modifier_mask, ei_mod_ctrl_left))
    {
        // Custom parameter to act as a backup of the root surface so that
        // it can be redrawn when turning off the offscreen picking surface.
        // Checking if the surface is NULL also allows to know if we should
        // toggle on or off the offscreen picking surface.
        ei_surface_t *root_surface_copy = (ei_surface_t *)user_param;
        ei_surface_t root_surface = ei_app_root_surface();

        ei_widget_t root = ei_app_root_widget();
        ei_update_pick_color_from_id(&root, *root_surface_copy != NULL ? 1 : 1000000);

        root->wclass->drawfunc(root, root_surface, ei_app_offscreen_picking_surface(), &widget->screen_location);

        hw_surface_lock(root_surface);
        hw_surface_lock(ei_app_offscreen_picking_surface());

        // If the offscreen picking surface is not displayed, make a copy of the root surface
        // and display the offscreen picking surface
        if (*root_surface_copy == NULL)
        {
            printf("Displaying offscreen picking surface\n");
            *root_surface_copy = hw_surface_create(root_surface, hw_surface_get_size(root_surface), true);
            ei_copy_surface(*root_surface_copy, NULL, root_surface, NULL, false);

            ei_copy_surface(root_surface, NULL, ei_app_offscreen_picking_surface(), NULL, false);
        }
        // Otherwise, restore the root surface and delete the copy
        else
        {
            printf("Hiding offscreen picking surface\n");
            ei_copy_surface(root_surface, NULL, *root_surface_copy, NULL, false);

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
    ei_bind(ei_ev_mouse_buttondown, NULL, "entry", ei_entry_pressed, NULL);

    ei_surface_t root_surface_copy = NULL;
    ei_bind(ei_ev_keydown, NULL, "all", toggle_offscreen_picking_surface_display, &root_surface_copy);
}

void test()
{
    toggle_offscreen_picking_surface_display(ei_app_root_widget(), NULL, NULL);
}