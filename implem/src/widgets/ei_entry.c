#include <stdlib.h>
#include <string.h>

#include "../api/ei_application.h"
#include "../api/ei_draw.h"
#include "../api/ei_utils.h"
#include "../api/ei_event.h"
#include "../api/ei_entry.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_entry.h"
#include "../implem/headers/ei_draw_ext.h"
#include "../implem/headers/ei_application_ext.h"
#include "../implem/headers/ei_utils_ext.h"
#include "../implem/headers/ei_entry_ext.h"
#include "../implem/headers/ei_internal_callbacks.h"

ei_widget_t ei_entry_allocfunc()
{
    return ei_widget_allocfunc(sizeof(ei_entry_t));
}

void ei_entry_releasefunc(ei_widget_t widget)
{
    ei_entry_t *entry = (ei_entry_t *)widget;

    free(entry);
    entry = NULL;
}

void ei_entry_drawfunc(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    DEBUG ? printf("Drawing widget %d\n", widget->pick_id) : 0;

    ei_entry_t *entry = (ei_entry_t *)widget;

    // Draw the visible entry
    if (entry->focused)
    {
        ei_draw_frame(surface, widget->screen_location, entry->widget_appearance.border_width, 0, entry->widget_appearance.color, ei_relief_none, &ei_entry_default_focused_border_color, clipper);
    }
    else
    {
        // The border should be 1 pixels smaller than on focus but also always be at least 2 pixels
        int border_width = entry->widget_appearance.border_width > 2
                               ? entry->widget_appearance.border_width - 1
                               : 2;

        ei_draw_frame(surface, widget->screen_location, border_width, 0, entry->widget_appearance.color, ei_relief_none, &ei_entry_default_unfocused_border_color, clipper);
    }

    // Draw the cursor
    ei_draw_cursor(surface, entry, clipper);

    // Draw the text
    ei_draw_entry_text(entry);

    // Draw the entry on the offscreen picking surface
    ei_draw_straight_frame(pick_surface, widget->screen_location, 0, *widget->pick_color, ei_relief_none, clipper);

    // Reduce the size of the clipper to the widget's content rect so that children
    // can't be drawn outside the widget's content rect
    ei_rect_t *children_clipper = malloc(sizeof(ei_rect_t));

    // If malloc failed, set the children clipper to the clipper
    if (children_clipper == NULL)
    {
        printf("\033[0;31mError: Couldn't allocate memory for children clipper.\n\t at %s (%s:%d)\033[0m\n", __func__, __FILE__, __LINE__);
        *children_clipper = *clipper;
    }
    else
    {
        *children_clipper = ei_get_children_clipper(*widget->content_rect, clipper);
    }

    ei_impl_widget_draw_children(widget, surface, pick_surface, children_clipper);

    free(children_clipper);
}

void ei_draw_cursor(ei_surface_t surface, ei_entry_t *entry, ei_rect_t *clipper)
{
    ei_point_t cursor_position;
    if (entry->cursor != NULL)
    {
        // Put the cursor between the character it is pointing to and the next one
        cursor_position = ei_point(
            entry->widget.screen_location.top_left.x + entry->cursor->position + entry->cursor->character_width + ei_entry_default_padding + ei_entry_default_letter_spacing / 2 - entry->characters_position_offset,
            entry->widget.screen_location.top_left.y + ei_entry_default_padding);
    }
    else
    {
        // Put the cursor at the start of the entry
        cursor_position = ei_point(
            entry->widget.screen_location.top_left.x + ei_entry_default_padding,
            entry->widget.screen_location.top_left.y + ei_entry_default_padding);
    }

    // If the cursor is out of bounds of the entry, move the text left/right
    if (cursor_position.x > entry->widget.content_rect->top_left.x + entry->widget.content_rect->size.width)
    {
        // Increase the offset by the number of pixels the cursor is out of bounds
        entry->characters_position_offset -= (entry->widget.content_rect->top_left.x + entry->widget.content_rect->size.width) - cursor_position.x;

        cursor_position.x = entry->widget.content_rect->top_left.x + entry->widget.content_rect->size.width;
    }
    else if (cursor_position.x < entry->widget.content_rect->top_left.x)
    {
        // Decrease the offset by the number of pixels the cursor is out of bounds
        entry->characters_position_offset -= entry->widget.content_rect->top_left.x - cursor_position.x;

        cursor_position.x = entry->widget.content_rect->top_left.x;
    }

    ei_rect_t cursor_rect = ei_rect(
        cursor_position,
        ei_size(ei_entry_default_cursor_width, entry->widget.screen_location.size.height - ei_entry_default_padding * 2));

    ei_color_t cursor_color = entry->cursor_visible
                                  ? ei_entry_default_focused_border_color
                                  : entry->widget_appearance.color;

    // The text is clipped to the content rect of the entry but the cursor
    // can be drawn to the left or the right the text, hence, we need to
    // increase the size of the clipper to the left and the right of the cursor
    // by 2 times its size to allow to make sure that the cursor is always visible
    const ei_rect_t cursor_clipper = ei_rect_add(*entry->widget.content_rect, -ei_entry_default_cursor_width * 2, 0, ei_entry_default_cursor_width * 2 * 2, 0);

    ei_draw_rectangle(ei_app_root_surface(), cursor_rect, cursor_color, &cursor_clipper);
}

void ei_draw_entry_text(ei_entry_t *entry)
{
    if (entry->first_character == NULL)
    {
        return;
    }

    for (ei_entry_character_t *character = entry->first_character; character != NULL; character = character->next)
    {
        char text[2] = {character->character, '\0'};

        ei_point_t where = ei_point(
            entry->widget.screen_location.top_left.x + ei_entry_default_padding + character->position - entry->characters_position_offset,
            entry->widget.screen_location.top_left.y + ei_entry_default_padding);

        if (where.x < entry->widget.content_rect->top_left.x || where.x > entry->widget.content_rect->top_left.x + entry->widget.content_rect->size.width)
        {
            continue;
        }

        ei_draw_text(ei_app_root_surface(), &where, text, entry->text.font, entry->text.color, entry->widget.content_rect);
    }
}

void ei_entry_setdefaultsfunc(ei_widget_t widget)
{
    ei_entry_t *entry = (ei_entry_t *)widget;

    entry->widget = *widget;

    entry->widget_appearance.color = ei_default_background_color;
    entry->widget_appearance.border_width = 0;

    entry->text.label = NULL;
    entry->text.font = ei_default_font;
    entry->text.color = ei_font_default_color;
    entry->text.anchor = ei_anc_center;

    entry->text_length = 0;

    entry->previous = NULL;
    entry->next = NULL;

    // Add fake character at the start of the entry. We need this because
    // for n characters, there is n + 1 cursor positions (1 character -->
    // cursor can be on the left or right)
    ei_entry_character_t *fake_character = malloc(sizeof(ei_entry_character_t));
    fake_character->character = '\0';
    fake_character->previous = NULL;
    fake_character->next = NULL;
    fake_character->position = 0;
    fake_character->character_width = 0;

    entry->first_character = fake_character;
    entry->last_character = fake_character;
    entry->cursor = fake_character;

    // Find the entry before this one in the widget tree
    // If there is one, link them
    ei_widget_t root = ei_app_root_widget();
    ei_widget_t *previous_entry = NULL;
    ei_get_previous_entry(&root, (ei_widget_t *)entry, &previous_entry);

    if (previous_entry != NULL)
    {
        entry->previous = (ei_entry_t *)previous_entry;
        ((ei_entry_t *)previous_entry)->next = entry;
    }

    entry->cursor_visible = false;
    entry->focused = false;
    entry->characters_position_offset = 0;
}

void ei_entry_geomnotifyfunc(ei_widget_t widget)
{
    ei_entry_t *entry = (ei_entry_t *)widget;

    int border_width = entry->widget_appearance.border_width;

    // Compute the content rect of the entry (size of the entry without its border and the padding)
    *widget->content_rect = ei_rect_add(widget->screen_location, border_width, border_width, -border_width * 2, -border_width * 2);
    *widget->content_rect = ei_rect_add(widget->screen_location, ei_entry_default_padding, ei_entry_default_padding, -ei_entry_default_padding * 2, -ei_entry_default_padding * 2);
}

ei_size_t ei_entry_get_natural_size(ei_entry_t *entry)
{
    if (&entry->widget == ei_app_root_widget())
    {
        return entry->widget.screen_location.size;
    }

    ei_size_t size = ei_size_zero();

    if (entry->widget_appearance.border_width > 0)
    {
        size.width += entry->widget_appearance.border_width * 2;
        size.height += entry->widget_appearance.border_width * 2;
    }

    return size;
}

void ei_update_requested_char_size(ei_entry_t *entry, int requested_char_size)
{
    ei_surface_t m_surface = hw_text_create_surface("m", entry->text.font, entry->text.color);
    ei_size_t size = hw_surface_get_size(m_surface);

    entry->widget.requested_size.width = size.width * requested_char_size + ei_entry_default_padding * 2;
    entry->widget.requested_size.height = size.height + ei_entry_default_padding * 2;

    entry->widget.screen_location.size = entry->widget.requested_size;
}

void ei_entry_release_focus(ei_widget_t widget)
{
    ei_entry_t *entry = (ei_entry_t *)widget;

    // Unbind the event for this entry
    ei_unbind(ei_ev_keydown, widget, NULL, ei_entry_keyboard_key_down, NULL);

    // Cancel application-generated event if there is one
    if (entry->blinking_app_id != NULL)
    {
        hw_event_cancel_app(entry->blinking_app_id);
        entry->blinking_app_id = NULL;
    }

    entry->focused = false;
    entry->cursor_visible = false;
}

ei_entry_character_t *ei_get_character_at_position(ei_entry_t *entry, ei_point_t position)
{
    ei_entry_character_t *character = entry->first_character;
    int position_x = entry->widget.screen_location.top_left.x + ei_entry_default_padding;

    while (character != NULL)
    {
        // If it is the last character, return it
        if (character->next == NULL)
        {
            return character;
        }

        // If the position is between the character and the next one, return the closest one
        if (position.x >= position_x - entry->characters_position_offset && position.x <= position_x + character->character_width - entry->characters_position_offset)
        {
            return position.x - position_x < position_x + character->character_width - position.x
                       ? character->previous
                       : character;
        }

        // Increase the position by the size of the character + the padding
        position_x += character->character_width + ei_entry_default_letter_spacing;

        character = character->next;
    }

    return NULL;
}

void ei_entry_add_character(ei_entry_t *entry, char character)
{
    ei_entry_character_t *entry_character = malloc(sizeof(ei_entry_character_t));

    entry_character->character = *(char *)malloc(sizeof(char));
    strcpy(&entry_character->character, &character);

    entry_character->previous = NULL;
    entry_character->next = NULL;

    // Insert the character at the position of the cursor
    entry_character->previous = entry->cursor;
    entry_character->next = entry->cursor->next;

    if (entry->cursor->next != NULL)
    {
        entry->cursor->next->previous = entry_character;
    }

    entry->cursor->next = entry_character;

    // If the new character is the last one, update the last character
    if (entry_character->next == NULL)
    {
        entry->last_character = entry_character;
    }

    entry_character->position = entry_character->previous != NULL
                                    ? entry_character->previous->position + entry_character->previous->character_width + ei_entry_default_letter_spacing
                                    : 0;

    // Calculate the width of the character
    char text[2] = {entry_character->character, '\0'};
    ei_surface_t text_surface = hw_text_create_surface(text, entry->text.font, entry->text.color);
    entry_character->character_width = hw_surface_get_size(text_surface).width;

    entry->cursor = entry_character;
    entry->text_length++;

    // Update the position of all the character after what we just wrote
    if (entry_character->next != NULL)
    {
        ei_compute_positions_after_character(entry, entry->cursor);
    }
}

void ei_entry_erase_character(ei_entry_t *entry, ei_entry_character_t *character)
{
    // If the cursor is pointing to the fake character, return
    if (character == entry->first_character)
    {
        return;
    }

    character->previous->next = character->next;

    if (character->next != NULL)
    {
        character->next->previous = character->previous;
    }

    // If the cursor pointed to the last character, update it
    if (character == entry->last_character)
    {
        entry->last_character = character->previous;
    }

    ei_entry_character_t *character_to_delete = character;
    free(character_to_delete);

    entry->cursor = character->previous;

    if (entry->cursor->previous != NULL)
    {
        ei_compute_positions_after_character(entry, entry->cursor->previous);
    }
    else
    {
        ei_compute_positions_after_character(entry, entry->cursor);
    }

    entry->text_length--;
}

void ei_compute_positions_after_character(ei_entry_t *entry, ei_entry_character_t *character)
{
    ei_entry_character_t *current_character = character->next;

    while (current_character != NULL)
    {
        current_character->position = current_character->previous != NULL
                                          ? current_character->previous->position + current_character->previous->character_width + ei_entry_default_letter_spacing
                                          : 0;

        current_character = current_character->next;
    }
}

void ei_restart_blinking_timer(ei_entry_t *entry, bool force_visible)
{
    // Cancel application-generated event if there is one and generate a new one
    if (entry->blinking_app_id != NULL)
    {
        hw_event_cancel_app(entry->blinking_app_id);
    }

    ei_app_event_params_t *params = malloc(sizeof(ei_app_event_params_t));
    params->id = 1;
    params->data = entry;

    entry->blinking_app_id = hw_event_schedule_app(ei_entry_default_blinking_interval, params);

    if (force_visible)
    {
        entry->cursor_visible = true;
    }
}