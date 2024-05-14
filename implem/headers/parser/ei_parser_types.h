/**
 * @file    ei_parser_types.h
 *
 * @brief   Defines various types used by the parser as well as functions to work with them
 *
 * @authors Seydina BADIANE
 * @authors Christian BOSSE
 * @authors Quentin CHAUVELON
 */

#ifndef EI_PARSER_TYPES_H
#define EI_PARSER_TYPES_H

#include "../api/ei_types.h"

extern struct ei_widgets_list_t *ei_widget_list;
extern struct ei_list_number_t *ei_list_number;
extern struct ei_list_name_t *ei_list_name;
extern int error;

/**
 * @brief   A struct to hold a list of numbers which can then be converted
 *          to other structs such as color, size, rect...
 */
typedef struct ei_list_number_t
{
    double value;
    struct ei_list_number_t *next;
} ei_list_number_t;

/**
 * @brief   A struct to hold a list of names which can then be converted
 *         to a single string
 */
typedef struct ei_list_name_t
{
    char *name;
    struct ei_list_name_t *next;
} ei_list_name_t;

/**
 * @brief   A struct to hold a list of widgets which allows to associate
 *          a name to a widget. Allows mapping from the names in the parsed
 *          file to the widgets instantiated by the parser (for events for example)
 */
typedef struct ei_widgets_list_t
{
    char *name;
    ei_widget_t widget;
    struct ei_widgets_list_t *next;
} ei_widgets_list_t;

/**
 * @brief   An enum describing the what the option refers to (configuring
 *          or placing a widget)
 */
enum options_action
{
    OPTION_CONFIGURE,
    OPTION_PLACE
};

/**
 * @brief   Associates the given name and the given widget by adding them to
 *          the list of widgets
 */
void ei_add_widget_to_list(char *name, ei_widget_t widget);

/**
 * @brief   Adds the given number to the list of numbers
 */
void ei_add_to_list_number(double value);

/**
 * @brief   Empties the list of numbers and frees the memory
 */
void ei_empty_list_number();

/**
 * @brief   Checks that the list of numbers has the given length
 *          and sets the error flag to true otherwise
 */
void ei_verify_numbers_list_length(int length);

/**
 * @brief   Adds the given name to the list of names
 */
void ei_add_to_list_name(char *name);

/**
 * @brief   Empties the list of names and frees the memory
 */
void ei_empty_list_name();

/**
 * @brief   Checks that the list of names has the given length
 *          and sets the error flag to true otherwise
 */
void ei_verify_names_list_length(int length);

#endif