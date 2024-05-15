#include "../api/ei_application.h"
#include "../api/ei_types.h"
#include "../api/ei_widget_configure.h"
#include "../api/ei_utils.h"
#include "../api/ei_event.h"
#include "../implem/headers/ei_entry.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_placer_ext.h"
#include "../implem/headers/ei_internal_callbacks.h"
#include "../implem/headers/ei_entry_ext.h"
#include "../implem/headers/ei_event_ext.h"

void ei_entry_configure(ei_widget_t widget, int *requested_char_size, const ei_color_t *color, int *border_width, ei_font_t *text_font, ei_color_t *text_color)
{
    ei_entry_t *entry = (ei_entry_t *)widget;

    // If the color is NULL, override the color only if it's the first time this function is called on the widget
    if (color != NULL)
    {
        entry->widget_appearance.color = *color;
    }
    else
    {
        if (!entry->widget.instantiated)
        {
            entry->widget_appearance.color = ei_default_background_color;
        }
    }

    if (border_width != NULL && *border_width >= 2)
    {
        entry->widget_appearance.border_width = *border_width;
    }
    else
    {
        if (!entry->widget.instantiated)
        {
            entry->widget_appearance.border_width = 2;
        }
    }

    if (text_font != NULL)
    {
        entry->text.font = *text_font;
    }
    else
    {
        if (!entry->widget.instantiated)
        {
            entry->text.font = ei_default_font;
        }
    }

    if (text_color != NULL)
    {
        entry->text.color = *text_color;
    }
    else
    {
        if (!entry->widget.instantiated)
        {
            entry->text.color = ei_font_default_color;
        }
    }

    if (requested_char_size != NULL)
    {
        ei_update_requested_char_size(entry, *requested_char_size);
    }
    else
    {
        if (!entry->widget.instantiated)
        {
            ei_update_requested_char_size(entry, 10);
        }
    }

    // Save the widget as instantiated so that any values set by the user won't be overriden
    // later on if another call is made with NULL values
    entry->widget.instantiated = true;

    // Update the geometry of the widget in case the size has changed
    if (ei_widget_is_displayed(widget))
    {
        widget->geom_params->manager->runfunc(widget);
    }

    widget->wclass->geomnotifyfunc(widget);
}

void ei_release_focused_entry(ei_widget_t *widget)
{
    if (widget == NULL)
    {
        return;
    }

    // If the widget is a focused entry, release the focus
    if (strcmp((*widget)->wclass->name, "entry") == 0 && ((ei_entry_t *)*widget)->focused)
    {
        ei_entry_release_focus(*widget);
    }

    // Check all children
    for (ei_widget_t children = (*widget)->children_head; children != NULL; children = children->next_sibling)
    {
        ei_release_focused_entry(&children);
    }
}

void ei_get_previous_entry(ei_widget_t *widget, ei_widget_t *entry, ei_widget_t **previous)
{
    if (widget == NULL)
    {
        return;
    }

    // If we find an entry, save it
    if (strcmp((*widget)->wclass->name, "entry") == 0 && (ei_widget_t *)*widget != entry)
    {
        *previous = (ei_widget_t *)*widget;
    }

    // If we found the entry, return the previous one
    if ((ei_widget_t *)*widget == entry)
    {
        return;
    }

    for (ei_widget_t children = (*widget)->children_head; children != NULL; children = children->next_sibling)
    {
        ei_get_previous_entry(&children, entry, previous);
    }
}

void ei_entry_set_text(ei_widget_t widget, ei_const_string_t text)
{
    ei_entry_t *entry = (ei_entry_t *)widget;

    for (int i = 0; i < strlen(text); i++)
    {
        ei_entry_add_character(entry, text[i]);
    }

    // Move the cursor back to the first character so that if it's a long
    // that doesn't fit in the entry, it shows the begginning of the text
    // instead of the end
    entry->cursor = entry->first_character;
}

ei_const_string_t ei_entry_get_text(ei_widget_t widget)
{
    ei_entry_t *entry = (ei_entry_t *)widget;

    return ei_entry_get_text_between_characters(widget, entry->first_character, entry->last_character);
}

ei_const_string_t ei_entry_get_text_between_characters(ei_widget_t widget, ei_entry_character_t *first_character, ei_entry_character_t *last_character)
{
    ei_entry_t *entry = (ei_entry_t *)widget;

    // Since the first character is a fake one and its character is '\0',
    // skip it. Otherwise, the text would end instantly
    if (entry->first_character->next == NULL)
    {
        return "";
    }

    int text_length = 0;

    // If the first character is pointing to the fake character, move it
    // to the first real character
    if (first_character == entry->first_character)
    {
        first_character = entry->first_character->next;
    }

    // If the first and last character point respectively to the first and last
    // characters of the text, we already know the lenth of the text
    if (first_character == entry->first_character && last_character == entry->last_character)
    {
        text_length = entry->text_length;
    }
    else
    {
        // Calculate the length of the text between the two characters
        ei_entry_character_t *character = first_character;

        while (character != last_character)
        {
            text_length++;
            character = character->next;
        }

        // Add the last character
        text_length++;
    }

    // Free the previous label if there is one
    if (entry->text.label != NULL)
    {
        free(entry->text.label);
        entry->text.label = NULL;
    }

    entry->text.label = malloc(sizeof(char) * text_length + 1);
    int i = 0;

    entry->text.label[i++] = first_character->character;

    ei_entry_character_t *character = first_character;
    while (character != NULL && character != last_character)
    {
        character = character->next;
        entry->text.label[i++] = character->character;
    }

    // Add the terminating character at the end of the string
    entry->text.label[i] = '\0';

    return entry->text.label;
}

void ei_entry_give_focus(ei_widget_t widget)
{
    // Release the focus if an entry already has it
    ei_widget_t root = ei_app_root_widget();
    ei_release_focused_entry(&root);

    ei_entry_t *entry = (ei_entry_t *)widget;

    // Bind the event for this entry
    ei_bind_internal(ei_ev_keydown, widget, NULL, ei_entry_keyboard_key_down, NULL, 20);

    entry->focused = true;
    entry->cursor_visible = true;

    // Start blinking the cursor
    ei_restart_blinking_timer((ei_entry_t *)widget, true);
}