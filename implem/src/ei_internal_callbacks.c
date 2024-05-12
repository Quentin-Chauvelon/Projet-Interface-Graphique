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
#include "../implem/headers/ei_entry_ext.h"

typedef struct ei_move_top_level_params_t
{
    ei_widget_t widget;
    ei_point_t offset;
} ei_move_top_level_params_t;

typedef struct ei_entry_drag_params_t
{
    ei_widget_t widget;
    ei_entry_character_t *previous_character;
} ei_entry_drag_params_t;

static bool ei_button_press(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    ei_button_t *button = (ei_button_t *)widget;
    button->frame_appearance.relief = ei_relief_sunken;

    // Bind the button release and move event
    // The button should be released if the user releases the mouse button
    // or if they move the cursor out of the button
    ei_bind(ei_ev_mouse_buttonup, NULL, "all", ei_button_release, widget);
    ei_bind(ei_ev_mouse_move, widget, NULL, ei_cursor_left_button, NULL);

    return true;
}

static bool ei_button_release(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    ei_button_t *button = (ei_button_t *)widget;
    button->frame_appearance.relief = ei_relief_raised;

    ei_app_invalidate_rect(&button->widget.screen_location);

    // Unbind the button release and move event
    ei_unbind(ei_ev_mouse_buttonup, NULL, "all", ei_button_release, user_param);
    ei_unbind(ei_ev_mouse_move, (ei_widget_t)user_param, NULL, ei_cursor_left_button, NULL);

    return false;
}

static bool ei_cursor_left_button(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    ei_button_t *button = (ei_button_t *)widget;

    // If the widget beneath the cursor is the button, show the button as pressed
    if (ei_get_picking_widget() == widget)
    {
        button->frame_appearance.relief = ei_relief_sunken;
    }
    // Otherwise, show the button as raised
    else
    {
        button->frame_appearance.relief = ei_relief_raised;
    }

    return true;
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

    return true;
}

static bool ei_entry_pressed(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    ei_entry_t *entry = (ei_entry_t *)widget;

    // If a double or triple click event is scheduled, we don't
    // want to handle single clicks for now, except if the user clicked
    // on another character, in which case it's not a double or triple click
    if (entry->multiple_click_app_id != NULL && entry->cursor == ei_get_character_at_position(entry, event->param.mouse.where))
    {
        return false;
    }

    entry->cursor = ei_get_character_at_position(entry, event->param.mouse.where);

    // Remove the selection if the user clicks on the entry once
    ei_set_selection_characters(entry, NULL, NULL, ei_selection_direction_none);

    ei_entry_give_focus(widget);

    ei_app_event_params_t *click_params = malloc(sizeof(ei_app_event_params_t));

    // If malloc failed, exit since the program can't run without the event
    if (click_params == NULL)
    {
        printf("\033[0;31mError: Couldn't allocate memory to handle entry click.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        exit(1);
    }

    click_params->id = 2;
    click_params->data = entry;

    // Schedule an event that will unbind the double click callback in a set amount of time
    entry->multiple_click_app_id = hw_event_schedule_app(ei_entry_max_double_click_interval, click_params);
    entry->multiple_click_params = click_params;

    // Bind the event to listen for a double click
    ei_bind(ei_ev_mouse_buttondown, widget, NULL, ei_entry_double_click, NULL);

    ei_entry_drag_params_t *params = malloc(sizeof(ei_entry_drag_params_t));

    // If malloc failed, exit since the program can't run without the event
    if (params == NULL)
    {
        printf("\033[0;31mError: Couldn't allocate memory to handle entry click.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        exit(1);
    }

    params->widget = widget;
    params->previous_character = entry->cursor;

    // Listen for mouse movement and release to select text by dragging the mouse
    ei_bind(ei_ev_mouse_move, NULL, "all", ei_entry_move, params);
    ei_bind(ei_ev_mouse_buttonup, NULL, "all", ei_entry_move_released, params);

    return true;
}

bool ei_entry_double_click(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    ei_entry_t *entry = (ei_entry_t *)widget;

    // Cancel the scheduled event since the user clicked twice fast enough
    if (entry->multiple_click_app_id != NULL)
    {
        hw_event_cancel_app(entry->multiple_click_app_id);
        entry->multiple_click_app_id = NULL;

        if (entry->multiple_click_params != NULL)
        {
            free(entry->multiple_click_params);
            entry->multiple_click_params = NULL;
        }
    }

    // Check if the user clicked the same characters twice, otherwise it shouldn't select
    if (ei_get_character_at_position(entry, event->param.mouse.where) == entry->cursor)
    {
        ei_entry_character_t *start_character = entry->cursor;
        ei_entry_character_t *end_character = entry->cursor;

        // Find previous space or beginning of text
        while (start_character != NULL && start_character->character != ' ' && start_character->previous != NULL)
        {
            start_character = start_character->previous;
        }

        // Find next space or end of text
        while (end_character != NULL && end_character->character != ' ' && end_character->next != NULL)
        {
            end_character = end_character->next;
        }

        // If the start character is the fake character, move to the next character
        if (start_character != NULL && start_character == entry->first_character)
        {
            start_character = entry->first_character->next;
        }

        // If the start character is a space, move to the next character
        if (start_character != NULL && &start_character->character != NULL && start_character->character == ' ')
        {
            start_character = start_character->next;
        }

        // If the end character is a space, move to the previous character
        if (end_character != NULL && end_character->character == ' ')
        {
            end_character = end_character->previous;
        }

        ei_set_selection_characters(entry, start_character, end_character, ei_selection_direction_right);

        ei_app_event_params_t *params = malloc(sizeof(ei_app_event_params_t));
        params->id = 3;
        params->data = entry;

        // Schedule an event that will unbind the triple click callback in a set amount of time
        entry->multiple_click_app_id = hw_event_schedule_app(ei_entry_max_double_click_interval, params);
        entry->multiple_click_params = params;

        // Bind the triple click event
        ei_bind(ei_ev_mouse_buttondown, widget, NULL, ei_entry_triple_click, user_param);
    }

    // Unbind the double click event
    ei_unbind(ei_ev_mouse_buttondown, widget, NULL, ei_entry_double_click, user_param);

    return true;
}

bool ei_entry_triple_click(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    ei_entry_t *entry = (ei_entry_t *)widget;

    // Cancel the scheduled event since the user clicked twice fast enough
    if (entry->multiple_click_app_id != NULL)
    {
        hw_event_cancel_app(entry->multiple_click_app_id);
        entry->multiple_click_app_id = NULL;

        if (entry->multiple_click_params != NULL)
        {
            free(entry->multiple_click_params);
            entry->multiple_click_params = NULL;
        }
    }

    // Check if the user clicked the same characters three times, otherwise it shouldn't select
    if (ei_get_character_at_position(entry, event->param.mouse.where) == entry->cursor)
    {
        // Select the whole text
        ei_set_selection_characters(entry, entry->first_character, entry->last_character, ei_selection_direction_right);
    }

    // Unbind the triple click event
    ei_unbind(ei_ev_mouse_buttondown, widget, NULL, ei_entry_triple_click, user_param);

    return true;
}

bool ei_entry_move(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    if (user_param == NULL)
    {
        return false;
    }

    ei_entry_drag_params_t params = *((ei_entry_drag_params_t *)user_param);
    ei_entry_t *entry = (ei_entry_t *)params.widget;

    ei_entry_character_t *character = ei_get_character_at_position(entry, event->param.mouse.where);

    // If the character under the cursor is different from the one that was under the cursor at
    // the last call to this callback function, we need to update the selection
    if (character != NULL && params.previous_character != NULL && character != params.previous_character)
    {
        // If the character is more to the right than the previous one, or
        // if we are at the first character (need this condition since the
        // the first character has a size of 0, the > check will fail and
        // we can't use >= since it has other side effects, so this is the
        // easiest fix)
        if (character->position > params.previous_character->position ||
            (character->position == params.previous_character->position &&
             character == entry->first_character->next))
        {
            // If there is no active selection, selection the character under the cursor
            if (entry->selection_direction == ei_selection_direction_none)
            {
                ei_set_selection_characters(entry, character, character, ei_selection_direction_right);
            }
            else
            {
                // If there is already a selection going to the right, update the end of the selection
                if (entry->selection_direction == ei_selection_direction_right)
                {
                    ei_set_selection_characters(entry, entry->selection_start_character, character, ei_selection_direction_right);
                }
                // If there is already a selection going to the left, update the start of the selection
                // or remove the selection if we reached the end of the text (since we can't select any
                // further characters)
                else
                {
                    character->next != NULL
                        ? ei_set_selection_characters(entry, character->next, entry->selection_end_character, ei_selection_direction_left)
                        : ei_set_selection_characters(entry, NULL, NULL, ei_selection_direction_none);
                }
            }
        }
        else
        {
            // If there is no active selection, selection the character under the cursor
            if (entry->selection_direction == ei_selection_direction_none)
            {
                ei_set_selection_characters(entry, character->next, character->next, ei_selection_direction_left);
            }
            else
            {
                // If there is already a selection going to the left, update the start of the selection
                if (entry->selection_direction == ei_selection_direction_left)
                {
                    ei_set_selection_characters(entry, character->next, entry->selection_end_character, ei_selection_direction_left);
                }
                // If there is already a selection going to the right, update the end of the selection
                else
                {
                    ei_set_selection_characters(entry, entry->selection_start_character, character, ei_selection_direction_right);
                }
            }
        }

        // If there was an active selection and the user unselected all the characters
        // (selection is now reversed), remove the selection since no characters are selected
        if (entry->selection_start_character != NULL &&
            entry->selection_end_character != NULL &&
            entry->selection_start_character->position > entry->selection_end_character->position)
        {
            ei_set_selection_characters(entry, NULL, NULL, ei_selection_direction_none);
        }
    }

    entry->cursor = character;

    // Update the previous character for the callback call
    ((ei_entry_drag_params_t *)user_param)->previous_character = entry->cursor;

    return true;
}

bool ei_entry_move_released(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    ei_unbind(ei_ev_mouse_move, NULL, "all", ei_entry_move, user_param);
    ei_unbind(ei_ev_mouse_buttonup, NULL, "all", ei_entry_move_released, user_param);

    return true;
}

bool ei_entry_keyboard_key_down(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param)
{
    ei_entry_t *entry = (ei_entry_t *)widget;

    bool handled = false;
    bool restart_blinking_timer = true;

    // Store the entry to get focus in a variable because we must
    // cancel the bliking timer event before giving focus (otherwise
    // it will restart), which can only be done at the very end of
    // this function, so we need to save the entry and give focus
    // at the very end after cancelling the event.
    // The reason we can't cancel the event right now is because
    // we don't want to reset the bliking for all keys (not if the
    // user presses ctrl/shift/alt alone for example)
    ei_entry_t *entry_to_give_focus_to = NULL;

    // Unfocus the entry if the user presses the escape key
    if (event->param.key_code == SDLK_RETURN)
    {
        ei_entry_release_focus(widget);

        // Set handled to false, otherwise, blinking will restart
        restart_blinking_timer = false;

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

                // When moving to the previous entry, select all text if there is some. Set the origin to the first character
                if (entry->previous->text_length > 0)
                {
                    ei_set_selection_characters(entry->previous, entry->previous->first_character, entry->previous->last_character, ei_selection_direction_right);
                }

                entry_to_give_focus_to = entry->previous;

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

                // When moving to the next entry, select all text if there is some. Set the origin to the first character
                if (entry->next->text_length > 0)
                {
                    ei_set_selection_characters(entry->next, entry->next->first_character, entry->next->last_character, ei_selection_direction_right);
                }

                entry_to_give_focus_to = entry->next;

                handled = true;
            }
        }
    }
    else if (event->param.key_code == SDLK_RIGHT)
    {
        // Move to the next character is there is one
        if (entry->cursor != NULL && entry->cursor->next != NULL)
        {
            // If the user also pressed ctrl, move to the next word
            if (ei_mask_has_modifier(event->modifier_mask, ei_mod_ctrl_left) ||
                ei_mask_has_modifier(event->modifier_mask, ei_mod_ctrl_right))
            {
                ei_entry_character_t *first_character = entry->cursor->next;
                bool set_end_character = entry->selection_end_character == NULL || entry->cursor == entry->selection_end_character;
                bool shift_pressed = ei_mask_has_modifier(event->modifier_mask, ei_mod_shift_left) || ei_mask_has_modifier(event->modifier_mask, ei_mod_shift_right);

                // While we have spaces, skip them
                while (entry->cursor->next != NULL && entry->cursor->next->character == ' ')
                {
                    // If the user pressed shift, select all spaces
                    if (shift_pressed)
                    {
                        entry->selection_start_character != NULL
                            ? set_end_character
                                  ? ei_set_selection_characters(entry, entry->selection_start_character, entry->cursor->next, ei_selection_direction_right)
                                  : ei_set_selection_characters(entry, entry->cursor->next, entry->selection_end_character, ei_selection_direction_right)
                        : set_end_character
                            ? ei_set_selection_characters(entry, entry->cursor->next, entry->cursor->next, ei_selection_direction_right)
                            : ei_set_selection_characters(entry, entry->cursor->next, entry->selection_end_character, ei_selection_direction_right);
                    }

                    entry->cursor = entry->cursor->next;
                }

                while (entry->cursor->next)
                {
                    // Once we reach a space, it's the end of the word, so stop
                    if (entry->cursor->next->character == ' ')
                    {
                        // If the selection is going from right to left and we have not reached the end,
                        // the last character of the word will be included in the selection, which
                        // we don't want, so move the selection start right
                        if (shift_pressed && !set_end_character && first_character != entry->selection_start_character)
                        {
                            ei_set_selection_characters(entry, entry->cursor->next, entry->selection_end_character, ei_selection_direction_right);
                        }

                        break;
                    }

                    // If the user pressed shift, add the character to the selection
                    if (shift_pressed)
                    {
                        // If there is no selection active, select the character, otherwise
                        // only update the end of the selection
                        // And if set_end_character is true, update the whole selection,
                        // otherwise only update the start
                        entry->selection_start_character != NULL
                            ? set_end_character
                                  ? ei_set_selection_characters(entry, entry->selection_start_character, entry->cursor->next, ei_selection_direction_right)
                                  : ei_set_selection_characters(entry, entry->cursor->next, entry->selection_end_character, ei_selection_direction_right)
                        : set_end_character
                            ? ei_set_selection_characters(entry, entry->cursor->next, entry->cursor->next, ei_selection_direction_right)
                            : ei_set_selection_characters(entry, entry->cursor->next, entry->selection_end_character, ei_selection_direction_right);

                        // If there already is a selection from the right to the left
                        // and the cursor reaches the end of that selection, start
                        // updating the end character to reverse the selection
                        // and create a selection from that character towards the right
                        if (entry->selection_end_character != NULL &&
                            entry->cursor != NULL &&
                            entry->cursor == entry->selection_end_character)
                        {
                            set_end_character = true;
                        }
                    }

                    entry->cursor = entry->cursor->next;
                }

                // If the user pressed shift and the start of the selection is
                // the last character
                if (shift_pressed)
                {
                    if (entry->selection_start_character != NULL &&
                        entry->selection_end_character != NULL &&
                        entry->selection_start_character == entry->last_character)
                    {
                        // If the start and end character are the same, that means only
                        // the last character is selected and since which shouldn't happen
                        // remove the selection.
                        // Otherwise, if the start and end character are different, it means
                        // that the start character is the last one and the end character is
                        // the one prior to the last one (the selection is reversed) which
                        // shouldn't happen either, so update the selection to only select
                        // the last character.
                        // Although both of these cases shouldn't happen, they do because
                        // in the while loop above the selection is updated before moving
                        // the cursor to the next character, and so, the last character
                        // is not treated properly for the selection
                        entry->selection_start_character == entry->selection_end_character
                            ? ei_set_selection_characters(entry, NULL, NULL, ei_selection_direction_none)
                            : ei_set_selection_characters(entry, entry->selection_start_character, entry->selection_start_character, ei_selection_direction_right);
                    }
                }
            }
            // Otherwise, move to the next character
            else
            {
                // If the user also pressed shift, select the next character
                if (ei_mask_has_modifier(event->modifier_mask, ei_mod_shift_left) ||
                    ei_mask_has_modifier(event->modifier_mask, ei_mod_shift_right))
                {
                    // If there is no selection active, select the next character
                    if (entry->selection_direction == ei_selection_direction_none && entry->cursor->next != NULL)
                    {
                        ei_set_selection_characters(entry, entry->cursor->next, entry->cursor->next, ei_selection_direction_right);
                    }
                    // If there is a selection going to the right, update the end of the selection only
                    else if (entry->selection_direction == ei_selection_direction_right && entry->cursor->next != NULL)
                    {
                        ei_set_selection_characters(entry, entry->selection_start_character, entry->cursor->next, entry->selection_direction);
                    }
                    // If there is a selection going to the right, update the start of the selection only and move the
                    // cursor by two spots otherwise it only updates the cursor position and not the selection
                    else if (entry->selection_direction == ei_selection_direction_left && entry->cursor->next != NULL)
                    {
                        ei_set_selection_characters(entry, entry->cursor->next->next, entry->selection_end_character, entry->selection_direction);
                    }
                }
                // Otherwise, remove the selection
                else
                {
                    // If there already was a selection, the cursor should be moved to the end of the selection,
                    // therefore, we move the cursor before the end since it will be moved forward at the end of this function
                    if (entry->selection_direction != ei_selection_direction_none && entry->selection_end_character->previous != NULL)
                    {
                        entry->cursor = entry->selection_end_character->previous;
                    }

                    ei_set_selection_characters(entry, NULL, NULL, ei_selection_direction_none);
                }

                entry->cursor = entry->cursor->next;
            }
        }

        // If the selection is reversed (start after end), remove it
        if (entry->selection_start_character != NULL &&
            entry->selection_end_character != NULL &&
            entry->selection_start_character->position > entry->selection_end_character->position)
        {
            ei_set_selection_characters(entry, NULL, NULL, ei_selection_direction_none);
        }

        // If the user is not pressing shift, remove the selection
        if (!ei_mask_has_modifier(event->modifier_mask, ei_mod_shift_left) &&
            !ei_mask_has_modifier(event->modifier_mask, ei_mod_shift_right))
        {
            ei_set_selection_characters(entry, NULL, NULL, ei_selection_direction_none);
        }

        handled = true;
    }
    else if (event->param.key_code == SDLK_LEFT)
    {
        // If the user also pressed ctrl, move to the previous word
        if (ei_mask_has_modifier(event->modifier_mask, ei_mod_ctrl_left) ||
            ei_mask_has_modifier(event->modifier_mask, ei_mod_ctrl_right))
        {
            ei_entry_character_t *last_character = entry->cursor;
            ei_entry_character_t *first_selected_character = entry->selection_start_character;
            bool set_start_character = entry->selection_start_character == NULL || (entry->cursor->next != NULL && entry->cursor->next == entry->selection_start_character);
            bool shift_pressed = ei_mask_has_modifier(event->modifier_mask, ei_mod_shift_left) || ei_mask_has_modifier(event->modifier_mask, ei_mod_shift_right);

            // While we have spaces, skip them
            while (entry->cursor->previous != NULL && entry->cursor->previous->character == ' ')
            {
                // If the user pressed shift, select all spaces
                entry->selection_end_character != NULL
                    ? set_start_character
                          ? ei_set_selection_characters(entry, entry->cursor, entry->selection_end_character, ei_selection_direction_left)
                          : ei_set_selection_characters(entry, entry->selection_start_character, entry->cursor, ei_selection_direction_left)
                : set_start_character
                    ? ei_set_selection_characters(entry, entry->cursor, entry->cursor, ei_selection_direction_left)
                    : ei_set_selection_characters(entry, entry->selection_start_character, entry->selection_end_character, ei_selection_direction_left);

                entry->cursor = entry->cursor->previous;
            }

            while (entry->cursor->previous)
            {
                // Once we reach a space, it's the start of the word, so stop
                if (entry->cursor->previous->character == ' ')
                {
                    // If the user pressed shift, add the character to the selection
                    if (shift_pressed)
                    {
                        // If there is no selection active, select the character, otherwise
                        // only update the start of the selection
                        // And if set_start_character is true, update the whole selection,
                        // otherwise only update the end
                        entry->selection_end_character != NULL
                            ? set_start_character
                                  ? ei_set_selection_characters(entry, entry->cursor, entry->selection_end_character, ei_selection_direction_left)
                                  : ei_set_selection_characters(entry, entry->selection_start_character, entry->cursor, ei_selection_direction_left)
                        : set_start_character
                            ? ei_set_selection_characters(entry, entry->cursor, entry->cursor, ei_selection_direction_left)
                            : ei_set_selection_characters(entry, entry->selection_start_character, entry->selection_end_character, ei_selection_direction_left);

                        // If there already is a selection from the left to the right
                        // and the cursor reaches the start of that selection, start
                        // updating the start character to reverse the selection
                        // and create a selection from that character towards the left
                        if (entry->selection_start_character != NULL &&
                            entry->cursor->next != NULL &&
                            entry->cursor->next == entry->selection_start_character)
                        {
                            set_start_character = true;
                        }

                        // If the selection is going from left to right and we have not reached the start,
                        // the first character of the word will be included in the selection, which
                        // we don't want, so move the selection end left
                        if (shift_pressed && !set_start_character && last_character != entry->selection_end_character)
                        {
                            ei_set_selection_characters(entry, entry->selection_start_character, entry->cursor->previous, ei_selection_direction_left);
                        }
                    }

                    entry->cursor = entry->cursor->previous;
                    break;
                }

                // If the user pressed shift, add the character to the selection
                if (shift_pressed)
                {
                    entry->selection_end_character != NULL
                        ? set_start_character
                              ? ei_set_selection_characters(entry, entry->cursor, entry->selection_end_character, ei_selection_direction_left)
                              : ei_set_selection_characters(entry, entry->selection_start_character, entry->cursor, ei_selection_direction_left)
                    : set_start_character
                        ? ei_set_selection_characters(entry, entry->cursor, entry->cursor, ei_selection_direction_left)
                        : ei_set_selection_characters(entry, entry->selection_start_character, entry->selection_end_character, ei_selection_direction_left);
                }

                // If there already is a selection from the left to the right
                // and the cursor reaches the start of that selection, start
                // updating the start character to reverse the selection
                // and create a selection from that character towards the left
                if (entry->selection_start_character != NULL &&
                    entry->cursor->next != NULL &&
                    entry->cursor->next == entry->selection_start_character)
                {
                    set_start_character = true;
                }

                entry->cursor = entry->cursor->previous;
            }

            // If the user selected the first visible character from left to right
            // and then moves the cursor to the left, the first character would
            // still be selected, so remove the selection
            if (first_selected_character != NULL &&
                first_selected_character == entry->selection_start_character &&
                first_selected_character == entry->selection_end_character)
            {
                ei_set_selection_characters(entry, NULL, NULL, ei_selection_direction_none);
            }
        }
        // Otherwise, move to the previous character
        else
        {
            // If the user also pressed shift, select the previous character
            if (ei_mask_has_modifier(event->modifier_mask, ei_mod_shift_left) ||
                ei_mask_has_modifier(event->modifier_mask, ei_mod_shift_right))
            {
                if (entry->selection_direction == ei_selection_direction_none && entry->cursor->previous != NULL)
                {
                    ei_set_selection_characters(entry, entry->cursor, entry->cursor, ei_selection_direction_left);
                }
                else if (entry->selection_direction == ei_selection_direction_left && entry->cursor->previous != NULL)
                {
                    ei_set_selection_characters(entry, entry->cursor, entry->selection_end_character, entry->selection_direction);
                }
                else if (entry->selection_direction == ei_selection_direction_right && entry->cursor->previous != NULL)
                {
                    ei_set_selection_characters(entry, entry->selection_start_character, entry->cursor->previous, entry->selection_direction);
                }
            }
            // Otherwise, remove the selection
            else
            {
                // If there already was a selection, the cursor should be moved to the start of the selection
                if (entry->selection_direction != ei_selection_direction_none && entry->cursor->next != NULL)
                {
                    entry->cursor = entry->selection_start_character;
                }

                ei_set_selection_characters(entry, NULL, NULL, ei_selection_direction_none);
            }

            entry->cursor = entry->cursor->previous;
        }

        // If the selection is reversed (start after end), remove it
        if (entry->selection_start_character != NULL &&
            entry->selection_end_character != NULL &&
            entry->selection_start_character->position > entry->selection_end_character->position)
        {
            ei_set_selection_characters(entry, entry->selection_end_character, entry->selection_end_character, ei_selection_direction_none);
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
        // If the user pressed shift, select all characters from the cursor to the start
        if ((ei_mask_has_modifier(event->modifier_mask, ei_mod_shift_left) ||
             ei_mask_has_modifier(event->modifier_mask, ei_mod_shift_right)) &&
            entry->cursor != entry->first_character)
        {
            ei_set_selection_characters(entry, entry->first_character, entry->cursor, ei_selection_direction_left);
        }
        // Otherwise, remove the selection
        else
        {
            ei_set_selection_characters(entry, NULL, NULL, ei_selection_direction_none);
        }

        entry->cursor = entry->first_character;

        handled = true;
    }
    // move to the last character
    else if (event->param.key_code == SDLK_END)
    {
        // If the user pressed shift, select all characters from the cursor to the end
        if ((ei_mask_has_modifier(event->modifier_mask, ei_mod_shift_left) ||
             ei_mask_has_modifier(event->modifier_mask, ei_mod_shift_right)) &&
            entry->cursor->next != NULL)
        {
            ei_set_selection_characters(entry, entry->cursor->next, entry->last_character, ei_selection_direction_right);
        }
        // Otherwise, remove the selection
        else
        {
            ei_set_selection_characters(entry, NULL, NULL, ei_selection_direction_none);
        }

        entry->cursor = entry->last_character;

        handled = true;
    }
    // Erase the character at cursor position
    else if (event->param.key_code == SDLK_BACKSPACE)
    {
        // If there is a selection active, erase it
        if (entry->selection_direction != ei_selection_direction_none)
        {
            ei_erase_selection(entry);
        }
        else
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

            // If the user pressed ctrl+A, select all the text
            if (ei_mask_has_modifier(event->modifier_mask, ei_mod_ctrl_left) ||
                ei_mask_has_modifier(event->modifier_mask, ei_mod_ctrl_right))
            {
                if (event->param.key_code == SDLK_a)
                {
                    entry->cursor = entry->first_character;
                    ei_set_selection_characters(entry, entry->first_character, entry->last_character, ei_selection_direction_right);
                }
                else if (event->param.key_code == SDLK_c)
                {
                    ei_copy_to_clipboard(entry);
                }
                else if (event->param.key_code == SDLK_x)
                {
                    ei_copy_to_clipboard(entry);

                    ei_erase_selection(entry);
                }
                else if (event->param.key_code == SDLK_v)
                {
                    if (clipboard != NULL)
                    {
                        // Save the character after the cursor since we want to move
                        // the cursor at the end of the pasted text
                        ei_entry_character_t *next_character = entry->cursor->next;

                        ei_entry_set_text(&entry->widget, clipboard);

                        // Move the cursor at the end of the pasted text
                        entry->cursor = next_character != NULL
                                            ? next_character->previous
                                            : entry->last_character;
                    }
                }
            }
            else
            {

                // If the shift key is pressed, make the letter uppercase
                if (ei_mask_has_modifier(event->modifier_mask, ei_mod_shift_left) ||
                    ei_mask_has_modifier(event->modifier_mask, ei_mod_shift_right))
                {
                    event->param.key_code = toupper(event->param.key_code);
                }

                ei_entry_add_character(entry, event->param.key_code);
            }

            handled = true;
        }
    }

    // Reset the blinking timer if the key press has been handled by the event
    if (restart_blinking_timer)
    {
        ei_restart_blinking_timer(entry, true);
    }

    // Give the focus to the entry that was saved to get focus
    if (entry_to_give_focus_to != NULL)
    {
        ei_entry_give_focus(&entry_to_give_focus_to->widget);
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