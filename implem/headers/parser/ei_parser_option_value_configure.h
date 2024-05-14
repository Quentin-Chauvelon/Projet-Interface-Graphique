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

void ei_set_option_value(enum options_action action, ei_widget_t widget, char *option_name, void *option_value, int *cast_tye);

#endif