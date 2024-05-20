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
 * @brief   Configures the attributes of widgets of the class "radio_button_group".
 *          Parameters obey the "optional parameters" protocol.
 *          By default, text properties and button colors of the group apply to all
 *          radio_buttons, but can be changed for each radio_button individually. Once
 *          a property is set for an individual radio_button, it will override the group's
 *          property for this radio_button even if the group's property is updated later.
 *
 * @param	widget	                The widget to configure.
 * @param	color	                The color of the radio button group, or NULL. Defaults to \ref ei_default_background_color.
 * @param	border_width	        The width of the border of the radio button group, or NULL. Defaults to 0.
 * @param	relief	                The relief of the radio button group, or NULL. Defaults to \ref ei_relief_none.
 * @param	text_font	            The font of the text of the radio button group, or NULL. Defaults to \ref ei_default_font.
 * @param	text_color	            The color of the text of the radio button group, or NULL. Defaults to \ref ei_font_default_color.
 * @param	buttons_color	        The color of the radio buttons, or NULL. Defaults to \ref ei_font_default_color.
 * @param	button_selected_color	The color of the radio buttons when they are selected, or NULL. Defaults to \ref ei_font_default_color.
 * @param	callback	            The callback function that will be called when a radio button is selected, or NULL. Defaults to NULL.
 *                                  The selected radio button can be retrieved using the \ref get_selected_radio_button function.
 */
void ei_radio_button_group_configure(ei_widget_t widget, const ei_color_t *color, int *border_width, ei_relief_t *relief, ei_font_t *text_font, ei_color_t *text_color, ei_color_t *buttons_color, ei_color_t *button_selected_color, ei_callback_t *callback);

/**
 * @brief   Configures the attributes of radio buttons.
 *		    Parameters obey the "optional parameters" protocol.
 *          By default, text properties and button colors of the group apply to all
 *          radio_buttons, but can be changed for each radio_button individually. Once
 *          a property is set for an individual radio_button, it will override the group's
 *          property for this radio_button even if the group's property is updated later.
 *
 * @param	widget		            The widget to configure.
 * @param	text		            The text to display on the radio button, or NULL. Defaults to NULL.
 * @param	text_font	            The font used to display the text, or NULL. Defaults to the group's text font.
 * @param	text_color	            The color used to display the text, or NULL. Defaults to the group's text color.
 * @param	button_color	        The color of the radio button, or NULL. Defaults to the group's button color.
 * @param	button_selected_color	The color of the radio button when it is selected, or NULL. Defaults to the group's selected button color.
 * @param	selected	            If true, the radio button will be selected (and any other radio button from
 *                                  the same group will be unselected). Defaults to false.
 */
void ei_radio_button_configure(ei_widget_t widget, ei_string_t *text, ei_font_t *text_font, ei_color_t *text_color, ei_color_t *button_color, ei_color_t *button_selected_color, bool *selected);

/**
 * @brief   Sets the text of a radio button
 *
 * @param	radio_button	The radio button to set the text for
 * @param	text	The text to display on the label of the radio button
 */
static inline void ei_radio_button_set_text(ei_widget_t widget, ei_string_t *text)
{
    ei_radio_button_configure(widget, text, NULL, NULL, NULL, NULL, NULL);
}

/**
 * @brief   Selects the given radio button.
 *          Will unselect any other radio button from the same group
 *
 * @param	radio_button	The radio button to select
 */
static inline void ei_radio_button_select(ei_widget_t widget)
{
    ei_radio_button_configure(widget, NULL, NULL, NULL, NULL, NULL, &(bool){true});
}

/**
 * @brief   Select the given radio button and unselects any other radio button from the same group
 *
 * @param   widget	The radio button to select
 */
void select_radio_button(ei_widget_t widget);

/**
 * @brief   Return if the given radio button is selected in its group
 *
 * @param   widget	The radio button to check for selection
 *
 * @return  True if the radio button is selected, false otherwise
 */
bool is_radio_button_selected(ei_widget_t widget);

/**
 * @brief   Return the selected radio button from the given group
 *
 * @param   widget	The radio button group to get the selected radio button from
 *
 * @return  The selected radio button
 */
ei_widget_t get_selected_radio_button(ei_widget_t widget);

/**
 * @brief   Return the text of the selected radio button from the given group
 *
 * @param   widget	The radio button group to get the selected radio button text from
 *
 * @return  The text of the selected radio button
 */
ei_string_t get_selected_radio_button_text(ei_widget_t widget);

/**
 * @brief   Unselect all radio buttons from the given group
 *
 * @param   widget	The radio button group to unselect the radio buttons from
 */
void clear_selection(ei_widget_t widget);

#endif