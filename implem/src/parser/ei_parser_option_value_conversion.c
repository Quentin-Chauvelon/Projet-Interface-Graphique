#include <stdio.h>

#include "../api/ei_parser.h"
#include "../api/ei_types.h"
#include "../api/ei_placer.h"
#include "../api/ei_application.h"
#include "../api/ei_widget_configure.h"
#include "../api/ei_entry.h"
#include "../api/ei_widgetclass.h"
#include "../implem/headers/ei_implementation.h"
#include "../implem/headers/parser/ei_parser_option_value_conversion.h"
#include "../implem/headers/parser/ei_parser_types.h"

char *ei_option_value_to_string()
{
    ei_list_name_t *current = ei_list_name;

    int length = 0;

    while (current != NULL)
    {
        length += strlen(current->name) + 1;
        current = current->next;
    }

    // Don't add + 1 for the terminating character since it has already been
    // added by the last word in the previous loop for the space
    char *result = malloc(sizeof(char) * length);
    memset(result, 0, length);

    // If malloc failed, return
    if (result == NULL)
    {
        printf("\033[0;31mError: Couldn't allocate memory for string\033[0m\n");
        return NULL;
    }

    current = ei_list_name;

    while (current != NULL)
    {
        strcat(result, current->name);

        current = current->next;

        // Don't add a space for the last word
        if (current != NULL)
        {
            strcat(result, " ");
        }
    }

    result[length - 1] = '\0';

    ei_empty_list_name();

    return result;
}

ei_color_t ei_option_value_to_color()
{
    ei_verify_numbers_list_length(4);

    ei_color_t color = {ei_list_number->value, ei_list_number->next->value, ei_list_number->next->next->value, ei_list_number->next->next->next->value};

    ei_empty_list_number();

    return color;
}

ei_size_t ei_option_value_to_size()
{
    ei_verify_numbers_list_length(2);

    ei_size_t size = {ei_list_number->value, ei_list_number->next->value};

    ei_empty_list_number();

    return size;
}

ei_rect_t ei_option_value_to_rect()
{
    ei_verify_numbers_list_length(4);

    ei_rect_t rect = {ei_list_number->value, ei_list_number->next->value, ei_list_number->next->next->value, ei_list_number->next->next->next->value};

    ei_empty_list_number();

    return rect;
}

ei_anchor_t ei_option_value_to_anchor(void *option_value)
{
    if (strcmp((char *)option_value, "none") == 0)
    {
        return ei_anc_none;
    }
    else if (strcmp((char *)option_value, "center") == 0)
    {
        return ei_anc_center;
    }
    else if (strcmp((char *)option_value, "north") == 0)
    {
        return ei_anc_north;
    }
    else if (strcmp((char *)option_value, "northeast") == 0)
    {
        return ei_anc_northeast;
    }
    else if (strcmp((char *)option_value, "east") == 0)
    {
        return ei_anc_east;
    }
    else if (strcmp((char *)option_value, "southeast") == 0)
    {
        return ei_anc_southeast;
    }
    else if (strcmp((char *)option_value, "south") == 0)
    {
        return ei_anc_south;
    }
    else if (strcmp((char *)option_value, "southwest") == 0)
    {
        return ei_anc_southwest;
    }
    else if (strcmp((char *)option_value, "west") == 0)
    {
        return ei_anc_west;
    }
    else if (strcmp((char *)option_value, "northwest") == 0)
    {
        return ei_anc_northwest;
    }
    else
    {
        printf("\033[0;31mError: Unknown anchor value: %s\033[0m\n", ei_list_name->name);
        error = 0;
        return ei_anc_none;
    }
}

ei_axis_set_t ei_option_value_to_axis_set(void *option_value)
{
    if (strcmp((char *)option_value, "none") == 0)
    {
        return ei_axis_none;
    }
    else if (strcmp((char *)option_value, "x") == 0)
    {
        return ei_axis_x;
    }
    else if (strcmp((char *)option_value, "y") == 0)
    {
        return ei_axis_y;
    }
    else if (strcmp((char *)option_value, "both") == 0)
    {
        return ei_axis_both;
    }
    else
    {
        printf("\033[0;31mError: Unknown axis value: %s\033[0m\n", ei_list_name->name);
        error = 0;
        return ei_axis_none;
    }
}

ei_relief_t ei_option_value_to_relief(void *option_value)
{
    if (strcmp((char *)option_value, "none") == 0)
    {
        return ei_relief_none;
    }
    else if (strcmp((char *)option_value, "raised") == 0)
    {
        return ei_relief_raised;
    }
    else if (strcmp((char *)option_value, "sunken") == 0)
    {
        return ei_relief_sunken;
    }
    else
    {
        printf("\033[0;31mError: Unknown relief value: %s\033[0m\n", ei_list_name->name);
        error = 0;
        return ei_relief_none;
    }
}