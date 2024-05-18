/**
 * @file    ei_widget_configure_ext.h
 *
 * @brief   Extends the \ref ei_widget_configure.h file and add functions to make the code more modular.
 *
 * @see     \ref ei_widget_configure.h
 *
 * @authors Seydina BADIANE
 * @authors Christian BOSSE
 * @authors Quentin CHAUVELON
 */

#ifndef EI_WIDGET_CONFIGURE_EXT_H
#define EI_WIDGET_CONFIGURE_EXT_H

#include "../api/ei_types.h"
#include "../api/hw_interface.h"
#include "../implem/headers/ei_widget_ext.h"

/**
 * @brief   Configures the widget appearance of a widget (color, border width).
 *          Parameters obey the "optional parameters" protocol: if a parameter is "NULL",
 *          the value of the configured widget for that parameter does not change.
 *
 * @param widget_appearance     The widget appearance to configure
 * @param instantiated          True if the widget has alreaddy been instantiated, false otherwise
 *                              If true, NULL parameters won't update the values, if false, they will.
 */
void ei_widget_appearance_configure(ei_widget_appearance_t *widget_appearance, bool instantiated, const ei_color_t *color, int *border_width, int default_border_width);

/**
 * @brief   Configures the text properties of a widget.
 *          Parameters obey the "optional parameters" protocol: if a parameter is "NULL",
 *          the value of the configured widget for that parameter does not change.
 *
 * @param widget_appearance     The widget appearance to configure
 * @param instantiated          True if the widget has alreaddy been instantiated, false otherwise
 *                              If true, NULL parameters won't update the values, if false, they will.
 * @param text                  The text to display in the widget
 * @param text_font             The font used to display the text
 * @param text_color            The color used to display the text
 * @param text_anchor           The anchor of the text
 */
void ei_text_configure(ei_text_properties_t *text_properties, bool instantiated, ei_string_t *text, ei_font_t *text_font, ei_color_t *text_color, ei_anchor_t *text_anchor);

/**
 * @brief   Configures the image properties of a widget.
 *          Parameters obey the "optional parameters" protocol: if a parameter is "NULL",
 *          the value of the configured widget for that parameter does not change.
 *
 * @param widget_appearance     The widget appearance to configure
 * @param instantiated          True if the widget has alreaddy been instantiated, false otherwise
 *                              If true, NULL parameters won't update the values, if false, they will.
 * @param img                   The image to display in the widget
 * @param img_rect              The rectangle defines a subpart of "img" to use as the image displayed in the widget
 * @param img_anchor            The anchor of the image
 */
void ei_image_configure(ei_image_properties_t *image_properties, bool instantiated, ei_surface_t *img, ei_rect_ptr_t *img_rect, ei_anchor_t *img_anchor);

/**
 * @brief   Finalizes the configuration of a widget. Must be called at the end of the configure function
 *          of any widget. Will update the instantiated flag and update the geometry of the widget
 *
 * @param widget        The widget to finalize configuration for
 * @param instantiated  The instantiated flag to update
 */
void ei_widget_configure_finalize(ei_widget_t widget, bool *instantiated);

#endif