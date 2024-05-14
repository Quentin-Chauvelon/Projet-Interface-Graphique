#include <stdio.h>

#include "../api/ei_types.h"
#include "../api/ei_parser.h"
#include "../implem/headers/parser/ei_parser_types.h"

ei_widgets_list_t *ei_widget_list;
ei_list_number_t *ei_list_number;
ei_list_name_t *ei_list_name;
int error = 1;

void ei_add_widget_to_list(char *name, ei_widget_t widget)
{
    ei_widgets_list_t *widget_to_add = malloc(sizeof(ei_widgets_list_t));
    widget_to_add->name = name;
    widget_to_add->widget = widget;
    widget_to_add->next = NULL;

    ei_widgets_list_t *current = ei_widget_list;

    if (current == NULL)
    {
        ei_widget_list = widget_to_add;
    }
    else
    {
        while (current->next != NULL)
        {
            current = current->next;
        }

        current->next = widget_to_add;
    }
}

ei_widget_t ei_parse_widget_from_name(char *name)
{
    for (ei_widgets_list_t *current = ei_widget_list; current != NULL; current = current->next)
    {
        if (strcmp(current->name, name) == 0)
        {
            return current->widget;
        }
    }

    return NULL;
}

void free_name_to_widget_list()
{
    ei_widgets_list_t *current = ei_widget_list;

    while (current != NULL)
    {
        ei_widgets_list_t *next = current->next;

        free(current->name);
        free(current);

        current = next;
    }

    ei_widget_list = NULL;
}

void ei_add_to_list_number(double value)
{
    ei_list_number_t *new_number = malloc(sizeof(ei_list_number_t));
    new_number->value = value;
    new_number->next = NULL;

    ei_list_number_t *current = ei_list_number;

    if (current == NULL)
    {
        ei_list_number = new_number;
    }
    else
    {
        while (current->next != NULL)
        {
            current = current->next;
        }

        current->next = new_number;
    }
}

void ei_empty_list_number()
{
    ei_list_number_t *current = ei_list_number;

    while (current != NULL)
    {
        ei_list_number_t *next = current->next;
        free(current);
        current = next;
    }

    ei_list_number = NULL;
}

void ei_verify_numbers_list_length(int length)
{
    int count = 0;

    ei_list_number_t *current = ei_list_number;

    while (current != NULL)
    {
        count++;
        current = current->next;
    }

    if (count != length)
    {
        printf("\033[0;31mError: Expected %d numbers in list, got %d\033[0m\n", length, count);
        error = 0;
        return;
    }
}

void ei_add_to_list_name(char *name)
{
    ei_list_name_t *widget_name = malloc(sizeof(ei_list_name_t));
    widget_name->name = name;
    widget_name->next = NULL;

    ei_list_name_t *current = ei_list_name;

    if (current == NULL)
    {
        ei_list_name = widget_name;
    }
    else
    {
        while (current->next != NULL)
        {
            current = current->next;
        }

        current->next = widget_name;
    }
}

void ei_empty_list_name()
{
    ei_list_name_t *current = ei_list_name;

    while (current != NULL)
    {
        ei_list_name_t *next = current->next;
        free(current->name);
        free(current);
        current = next;
    }

    ei_list_name = NULL;
}

void ei_verify_names_list_length(int length)
{
    int count = 0;

    ei_list_name_t *current = ei_list_name;

    while (current != NULL)
    {
        count++;
        current = current->next;
    }

    if (count != length)
    {
        printf("\033[0;31mError: Expected %d names in list, got %d\033[0m\n", length, count);
        error = 0;
        return;
    }
}