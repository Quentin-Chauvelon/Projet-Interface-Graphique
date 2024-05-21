/**
 * @file    ei_entry_ext.h
 *
 * @brief   Extends the \ref ei_entry.h file and add functions to handle entries.
 *
 * @see     \ref ei_entry.h
 *
 * @authors Seydina BADIANE
 * @authors Christian BOSSE
 * @authors Quentin CHAUVELON
 */

#ifndef EI_ENTRY_EXT_H
#define EI_ENTRY_EXT_H

#include "../api/ei_types.h"
#include "../implem/headers/ei_entry.h"

/**
 * @brief   Search all widgets and release the focused entry if there is one.
 *          This implements a tree preorder traversal algorithm
 *
 * @param   widget  The current widget
 */
void ei_release_focused_entry(ei_widget_t *widget);

/**
 * @brief   Search all widgets to find the entry that is before the given entry
 *          in the widget tree.
 *          This implements a tree preorder traversal algorithm
 *
 * @param   widget      The current widget
 * @param   entry       The entry to find the previous one for
 * @param   previous    A pointer to the previous entry
 *
 * @return  The previous entry if it exists, NULL otherwise
 */
void ei_get_previous_entry(ei_widget_t *widget, ei_widget_t *entry, ei_widget_t **previous);

/**
 * @brief   Returns a string representing the text between the two given characters
 *          of the entry
 *
 * @param   widget          The current widget
 * @param   first_character The first character to get the text from
 * @param   last_character  The last character to get the text from
 *
 * @return  The text between the two characters
 */
ei_const_string_t ei_entry_get_text_between_characters(ei_widget_t widget, ei_entry_character_t *first_character, ei_entry_character_t *last_character);

/**
 * @brief   Returns if an entry is currently focused or not
 * 
 * @return  True if an entry is focused, false otherwise
*/
bool is_an_entryfocused();

#endif