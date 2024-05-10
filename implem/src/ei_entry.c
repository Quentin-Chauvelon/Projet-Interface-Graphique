#include "../api/ei_application.h"
#include "../api/ei_types.h"
#include "../api/ei_widget_configure.h"
#include "../api/ei_utils.h"
#include "../api/ei_event.h"
#include "../implem/headers/ei_entry.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_placer_ext.h"
#include "../implem/headers/ei_internal_callbacks.h"

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
    if (strcmp((*widget)->wclass->name, "entry") == 0)
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
        ei_entry_character_t *character = malloc(sizeof(ei_entry_character_t));

        character->character = *(char *)malloc(sizeof(char));
        strcpy(&character->character, &text[i]);

        character->next = NULL;

        // If the entry is empty, update the first chracter
        if (entry->first_character == NULL)
        {
            entry->first_character = character;
        }

        character->previous = entry->last_character;
        entry->last_character = character;

        // If the previous is not NULL, set its next to this character
        if (character->previous != NULL)
        {
            character->previous->next = character;
        }

        character->position = character->previous != NULL
                                  ? character->previous->position + character->previous->character_width + ei_entry_default_letter_spacing
                                  : 0;

        char text[2] = {character->character, '\0'};
        ei_surface_t text_surface = hw_text_create_surface(text, entry->text.font, entry->text.color);
        character->character_width = hw_surface_get_size(text_surface).width;
    }
}

ei_const_string_t ei_entry_get_text(ei_widget_t widget)
{
    ei_entry_t *entry = (ei_entry_t *)widget;

    char *text = malloc(sizeof(char) * entry->text_length + 1);
    int i = 0;

    ei_entry_character_t *character = entry->first_character;
    while (character != NULL)
    {
        text[i++] = character->character;
        character = character->next;
    }

    // Add the terminating character at the end of the string
    text[i] = '\0';

    return text;
}

void ei_entry_give_focus(ei_widget_t widget)
{
    // Release the focus if an entry already has it
    ei_widget_t root = ei_app_root_widget();
    ei_release_focused_entry(&root);

    ei_entry_t *entry = (ei_entry_t *)widget;

    // Bind the event for this entry
    ei_bind(ei_ev_keydown, widget, NULL, ei_entry_keyboard_key_down, NULL);

    entry->focused = true;
    entry->cursor_visible = true;
}