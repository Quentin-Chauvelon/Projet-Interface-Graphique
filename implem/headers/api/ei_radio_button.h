/**
 * @file	ei_radio_button.h
 *
 * @brief 	API for handling radio buttons widgets
 *
 * @see    \ref ei_widgetclass.h
 *
 * @authors Seydina BADIANE
 * @authors Christian BOSSE
 * @authors Quentin CHAUVELON
 */

#ifndef EI_RADIO_BUTTON_API_H
#define EI_RADIO_BUTTON_API_H

#include "ei_types.h"

/**
 * @brief   Configures the attributes of widgets of the class "frame".
 *		    Parameters obey the "optional parameters" protocol.
 *
 * @param	widget		The widget to configure.
 * @param	text		The text to display on the radio button, or NULL.
 * @param	text_font	The font used to display the text, or NULL.
 * @param	text_color	The color used to display the text, or NULL.
 * @param	text_anchor	The anchor of the text, or NULL.
 * @param	selected	If true, the radio button will be selected (and any other radio button from
 *                      the same group will be unselected)
 */
void ei_radio_button_configure(ei_widget_t widget, ei_string_t *text, ei_font_t *text_font, const ei_color_t *text_color, ei_anchor_t *text_anchor, bool *selected);

/**
 * @brief   Sets the text of a radio button
 *
 * @param	widget	The radio button widget
 * @param	text	The text to display on the label of the radio button
 */
static inline void ei_radio_button_set_text(ei_widget_t widget, ei_string_t *text)
{
    ei_radio_button_configure(widget, text, NULL, NULL, NULL, NULL);
}

/**
 * @brief   Selects the given radio button.
 *          Will unselect any other radio button from the same group
 *
 * @param	widget	The radio button to select
 */
static inline void ei_radio_button_select(ei_widget_t widget)
{
    ei_radio_button_configure(widget, NULL, NULL, NULL, NULL, &(bool){true});
}

#endif