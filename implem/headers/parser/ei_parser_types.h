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

typedef struct ei_list_number_t
{
    double value;
    struct ei_list_number_t *next;
} ei_list_number_t;

typedef struct ei_list_name_t
{
    char *name;
    struct ei_list_name_t *next;
} ei_list_name_t;

typedef struct ei_widgets_list_t
{
    char *name;
    ei_widget_t widget;
    struct ei_widgets_list_t *next;
} ei_widgets_list_t;

enum options_action
{
    OPTION_CONFIGURE,
    OPTION_PLACE
};

void ei_add_widget_to_list(char *name, ei_widget_t widget);

void ei_add_to_list_number(double value);

void ei_empty_list_number();

void ei_verify_numbers_list_length(int length);

void ei_add_to_list_name(char *name);

void ei_empty_list_name();

void ei_verify_names_list_length(int length);

#endif