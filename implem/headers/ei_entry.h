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

#ifndef EI_ENTRY_WIDGET_H
#define EI_ENTRY_WIDGET_H

#include "../api/ei_types.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/ei_widget_ext.h"
#include "../implem/headers/ei_types_ext.h"

static const int ei_entry_default_padding = 8;
static const int ei_entry_default_letter_spacing = 0;
static const int ei_entry_default_cursor_width = 1;
static const int ei_entry_default_blinking_interval = 700;
static const int ei_entry_max_double_click_interval = 600;
static const ei_color_t ei_entry_default_unfocused_border_color = (ei_color_t){120, 120, 120, 255};
static const ei_color_t ei_entry_default_focused_border_color = (ei_color_t){50, 50, 50, 255};
static const ei_color_t ei_entry_default_selection_color = (ei_color_t){78, 143, 222, 255};

extern ei_string_t clipboard;

typedef struct ei_entry_character_t
{
    char character;                        // Character
    struct ei_entry_character_t *previous; // Pointer to the previous character
    struct ei_entry_character_t *next;     // Pointer to the next character
    int position;                          // Position of the character based on the first character
    int character_width;                   // Width of the character
} ei_entry_character_t;

typedef enum ei_selection_direction_t
{
    ei_selection_direction_none = 0,
    ei_selection_direction_left,
    ei_selection_direction_right
} ei_selection_direction_t;

typedef struct ei_entry_t
{
    ei_impl_widget_t widget;                         // Common attributes for all types of widgets
    ei_widget_appearance_t widget_appearance;        // Appearance fields common to frame, buttons, toplevels, and entries
    ei_text_properties_t text;                       // Text properties
    int text_length;                                 // Number of characters in the entry
    struct ei_entry_t *previous;                     // Pointer to the previous entry
    struct ei_entry_t *next;                         // Pointer to the next entry
    ei_entry_character_t *first_character;           // First character of the entry
    ei_entry_character_t *last_character;            // Last character of the entry
    ei_entry_character_t *cursor;                    // Position of the cursor
    ei_entry_character_t *selection_start_character; // Pointer to the first character of the selection
    ei_entry_character_t *selection_end_character;   // Pointer to the last character of the selection
    ei_selection_direction_t selection_direction;    // Direction of the selection (left, right or none (if no selection)). This allows to know if the selection should expand or shrink based on the keys pressed
    bool cursor_visible;                             // Boolean indicating if the cursor is visible (blinking)
    bool focused;                                    // Boolean indicating if the entry is focused
    int characters_position_offset;                  // Offset of the characters position once the line reaches the end of the entry which allows to scroll left/right and display the right characters
    void *blinking_app_id;                           // Id of the blinking application-generated event. Allows to cancel blinking whenever an event related to the entry is raised
    ei_app_event_params_t *blinking_params;          // Parameters of the blinking event. Need to keep a reference to them in case the event is cancelled to free them
    void *multiple_click_app_id;                     // Id of the double or triple click application-generated event. Allows to cancel the event if the user clicked twice or three times fast enough to trigger a double or triple click
    ei_app_event_params_t *multiple_click_params;    // Parameters of the double or triple click event. Need to keep a reference to them in case the event is cancelled to free them
    bool triple_clicked;                             // Boolean indicating if the user has clicked three times on the entry
} ei_entry_t;

/**
 * \brief	A function that allocates a block of memory that is big enough to store the
 *		attributes of an entry.
 *
 * @return			A block of memory big enough to represent a widget of this class,
 * 				with all bytes set to 0.
 */
ei_widget_t
ei_entry_allocfunc();

void ei_draw_cursor(ei_surface_t surface, ei_entry_t *entry, ei_rect_t *clipper);

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

/**
 * @brief	Release the focus from the entry widget
 *
 * @param	widget  The widget to release focus from
 */
void ei_entry_release_focus(ei_widget_t widget);

/**
 * @brief   Draw the cursor of the given entry
 *
 * @param	surface 	Where to draw the text. The surface must be *locked* by
 *				        \ref hw_surface_lock.
 * @param	entry		The entry for which to draw the cursor.
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle.
 */
void ei_draw_cursor(ei_surface_t surface, ei_entry_t *entry, ei_rect_t *clipper);

/**
 * @brief   Draw the text of the given entry
 *
 * @param   entry   The entry for which to draw the text
 * @param   clipper If not NULL, the drawing is restricted within this rectangle
 */
void ei_draw_entry_text(ei_entry_t *entry, ei_rect_t *clipper);

/**
 * @brief   Return the character that is closest to the given position (where the user clicked)
 *
 * @param   entry       The entry for which to get the character
 * @param   position    The position where the user clicked
 *
 * @return  The character that is closest to the given position
 */
ei_entry_character_t *ei_get_character_at_position(ei_entry_t *entry, ei_point_t position);

/**
 * @brief   Add a character at cursor position for the given entry
 *
 * @param   entry       The entry for which to add the character
 * @param   character   The character to add
 */
void ei_entry_add_character(ei_entry_t *entry, char character);

/**
 * @brief   Erase the character at cursor position for the given entry
 *
 * @param   entry       The entry for which to erase the character
 * @param   character   A pointer to the charcter to erase
 */
void ei_entry_erase_character(ei_entry_t *entry, ei_entry_character_t *character);

/**
 * @brief   Erase the characters in the selection
 *
 * @param   entry       The entry for which to erase the selection
 */
void ei_erase_selection(ei_entry_t *entry);

/**
 * @brief   Recompute the position of all the characters after the give one.
 *          This is primarly used when writing a character in the middle of the entry
 *          since we need to shift the position of all the characters at the right
 *          of the one the user typed
 *
 * @param   entry       The entry for which to recompute the positions
 * @param   character   The character after which to recompute the positions
 */
void ei_compute_positions_after_character(ei_entry_t *entry, ei_entry_character_t *character);

/**
 * @brief   Restart the blinking timer of the given entry. Cancel the previous event
 *          if there is one and schedule the next one
 *
 * @param   entry           The entry for which to restart the blinking timer
 * @param   force_visible   If true, the cursor will be set to visible
 */
void ei_restart_blinking_timer(ei_entry_t *entry, bool force_visible);

/**
 * @brief   Update the selection by settings the first and last character
 *          of said selection
 *
 * @param   entry           The entry for which to update the selection
 * @param   start_character The first character of the selection
 * @param   end_character   The last character of the selection
 * @param   direction       The direction of the selection (left, right or none). Allows to know
 *                          if the selection should expand or shrink based on the keys pressed
 */
void ei_set_selection_characters(ei_entry_t *entry, ei_entry_character_t *start_character, ei_entry_character_t *end_character, ei_selection_direction_t direction);

/**
 * @brief   Copy the characters in the selection to the clipboard
 *
 * @param   entry   The entry for which to copy the selection
 */
void ei_copy_to_clipboard(ei_entry_t *entry);

#endif