/**
 * @file	ei_entry.h
 *
 * @brief 	Definition of an entry widget class.
 *          Defines custom functions from \ref ei_widgetclass.h
 *
 * @see    \ref ei_widgetclass.h
 *
 * @authors Seydina BADIANE
 * @authors Christian BOSSE
 * @authors Quentin CHAUVELON
 */

#ifndef EI_ENTRY_H
#define EI_ENTRY_H

#include "../api/ei_types.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_widget_ext.h"

static const int ei_entry_default_padding = 5;
static const int ei_entry_default_letter_spacing = 2;
static const ei_color_t ei_entry_default_focused_border_color = (ei_color_t){230, 230, 230, 255};
static const ei_color_t ei_entry_default_selection_color = (ei_color_t){0, 116, 255, 204};

typedef struct ei_entry_character_t
{
    char character;                        // Character
    struct ei_entry_character_t *previous; // Pointer to the previous character
    struct ei_entry_character_t *next;     // Pointer to the next character
    int position;                          // Position of the character based on the first character
    int character_width;                   // Width of the character
} ei_entry_character_t;

typedef struct ei_entry_t
{
    ei_impl_widget_t widget;                  // Common attributes for all types of widgets
    ei_widget_appearance_t widget_appearance; // Appearance fields common to frame, buttons, toplevels, and entries
    ei_text_properties_t text;                // Text properties
    int text_length;                          // Number of characters in the entry
    struct ei_entry_t *previous;              // Pointer to the previous entry
    struct ei_entry_t *next;                  // Pointer to the next entry
    ei_entry_character_t *first_character;    // First character of the entry
    ei_entry_character_t *last_character;     // Last character of the entry
    ei_entry_character_t *cursor;             // Position of the cursor
    bool cursor_visible;                      // Boolean indicating if the cursor is visible (blinking)
    bool focused;                             // Boolean indicating if the entry is focused
    ei_string_t placeholder_text;             // Placeholder text
} ei_entry_t;

/**
 * \brief	A function that allocates a block of memory that is big enough to store the
 *		attributes of an entry.
 *
 * @return			A block of memory big enough to represent a widget of this class,
 * 				with all bytes set to 0.
 */
ei_widget_t ei_entry_allocfunc();

/**
 * \brief	A function that releases the memory used by a widget before it is destroyed.
 *		The memory used for the \ref ei_widget_t structure itself must *not* be freed by
 *		the function. It frees the *fields* of the structure, if necessary.
 *		Can be set to NULL in \ref ei_widgetclass_t if no memory is used by a class of widget.
 *
 * @param	widget		The widget which resources are to be freed.
 */
void ei_entry_releasefunc(ei_widget_t widget);

/**
 * \brief	A function that draws an entry.
 * 		The function must also draw the children of the widget.
 *
 * @param	widget		A pointer to the widget instance to draw.
 * @param	surface		A locked surface where to draw the widget. The actual location of the widget in the
 *				surface is stored in its "screen_location" field.
 * @param	pick_surface	The picking offscreen.
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle
 *				(expressed in the surface reference entry).
 */
void ei_entry_drawfunc(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper);

/**
 * \brief	A function that sets the default values for an entry.
 *
 * @param	widget		A pointer to the widget instance to initialize.
 */
void ei_entry_setdefaultsfunc(ei_widget_t widget);

/**
 * \brief 	A function that is called to notify the widget that its geometry has been modified
 *		    by its geometry manager. Can set to NULL in \ref ei_widgetclass_t.
 *		    The new location can be retrieved by \ref ei_widget_get_screen_location.
 *
 * @param	widget		The widget instance to notify of a geometry change.
 */
void ei_entry_geomnotifyfunc(ei_widget_t widget);
/**
 * @brief   Returns the natural size of an entry
 *
 * @param   entry   The entry to return the natural size for.
 *
 * @return  The natural size of the entry.
 */
ei_size_t ei_entry_get_natural_size(ei_entry_t *entry);

/**
 * @brief   Update the size of the entry to fit the 'm' character the amount of times given
 *
 * @param   entry               The entry for which to update the size
 * @param   requested_char_size The number of times the entry should be able to hold the 'm' character
 */
void ei_update_requested_char_size(ei_entry_t *entry, int requested_char_size);

#endif