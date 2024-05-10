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

#endif