/**
 * @file    ei_parser_option_value_configure.h
 *
 * @brief   Defines functions to set values of options for widgets during parsing
 *
 * @authors Seydina BADIANE
 * @authors Christian BOSSE
 * @authors Quentin CHAUVELON
 */

#ifndef EI_PARSER_OPTION_VALUE_CONFIGURE_H
#define EI_PARSER_OPTION_VALUE_CONFIGURE_H

#include "../api/ei_types.h"
#include "../implem/headers/parser/ei_parser_types.h"

/**
 * @brief   Set the value of an option for a widget (configure or place).
 *          For example, if action is OPTION_CONFIGURE, the widget is a
 *          toplevel, option_name is "title" and option_value is "Toplevel",
 *          the function will call ei_toplevel_configure with only the parameter
 *          title non NULL and equal to "Toplevel".
 *
 * @param action        The action to perform (configure or place)
 * @param widget        The widget to set the option value for
 * @param option_name   The name of the option
 * @param option_value  The value of the option
 * @param cast_type     The type to cast the value to if there can be multiple types
 */
void ei_set_option_value(enum options_action action, ei_widget_t widget, char *option_name, void *option_value, int *cast_tye);

#endif