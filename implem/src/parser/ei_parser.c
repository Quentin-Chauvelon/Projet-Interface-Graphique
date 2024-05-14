#include <stdio.h>
#include <math.h>

#include "../api/ei_parser.h"
#include "../api/ei_application.h"
#include "../implem/headers/parser/ei_lexer.h"
#include "../implem/headers/parser/ei_parser_types.h"
#include "../implem/headers/parser/ei_parser_option_value_configure.h"

void parse_list_number()
{
    while (true)
    {
        if (error == 0)
        {
            return;
        }

        skip_spaces();

        if (is_digit_token())
        {
            double value = parse_number();
            ei_add_to_list_number(value);
        }
        else if (get_current() == prefix[CB][0])
        {
            break;
        }
        else
        {
            printf("\033[0;31mError: Expected a digit or a closing curly bracket, got %c\033[0m\n", get_current());
            error = 0;
            return;
        }
    }
}

void parse_list_name()
{
    while (true)
    {
        if (error == 0)
        {
            return;
        }

        skip_spaces();

        if (is_name_token())
        {
            char *name = parse_name();
            ei_add_to_list_name(name);
        }
        else if (get_current() == prefix[CB][0])
        {
            break;
        }
        else
        {
            printf("\033[0;31mError: Expected a character or a closing curly bracket, got %c\033[0m\n", get_current());
            error = 0;
            return;
        }
    }
}

void parse_option_value_X(enum options_action action, ei_widget_t widget, char *option_name)
{
    if (is_digit_token())
    {
        parse_list_number();
        ei_set_option_value(action, widget, option_name, NULL, NULL);
    }
    else if (is_name_token())
    {
        parse_list_name();
        ei_set_option_value(action, widget, option_name, NULL, &(int){1});
    }
    else
    {
        printf("\033[0;31mError: Expected a digit or a name token, got %c\033[0m\n", get_current());
        error = 0;
        return;
    }
}

void parse_option_value(enum options_action action, ei_widget_t widget, char *option_name)
{
    skip_spaces();

    if (is_digit_token())
    {
        double value = parse_number();
        ei_set_option_value(action, widget, option_name, &value, NULL);
    }
    else if (is_name_token())
    {
        char *value = parse_name();
        ei_set_option_value(action, widget, option_name, value, &(int){0});
        free(value);
    }
    else if (get_current() == prefix[OB][0])
    {
        parse_token(OB);

        parse_option_value_X(action, widget, option_name);

        parse_token(CB);
    }
    else
    {
        printf("\033[0;31mError: Expected a name token, a digit or a '{', got %c\033[0m\n", get_current());
        error = 0;
        return;
    }
}

void parse_option(enum options_action action, ei_widget_t widget)
{
    char *name = parse_name();

    parse_token(EQUAL);

    parse_option_value(action, widget, name);

    free(name);
}

void parse_list_options(enum options_action action, ei_widget_t widget)
{
    skip_spaces();

    while (true)
    {
        if (error == 0)
        {
            return;
        }

        if (is_name_token())
        {
            parse_option(action, widget);
        }
        else if (get_current() == prefix[END_LINE][0])
        {
            break;
        }
        else
        {
            printf("\033[0;31mError: Expected a name token or new line, got %c\033[0m\n", get_current());
            error = 0;
            return;
        }
    }
}

void parse_widget_command()
{
    char *type = parse_widget_type();

    if (ei_widgetclass_from_name(type) == NULL)
    {
        printf("\033[0;31mError: Unknown widget type: \"%s\"\033[0m\n", type);
        error = 0;
        return;
    }

    char *name = parse_widget_name();
    char *parent = parse_parent_name();

    ei_widget_t parent_widget = ei_parse_widget_from_name(parent);

    if (parent_widget == NULL)
    {
        printf("\033[0;31mError: Unknow parent: %s\033[0m\n", parent);
        error = 0;
        return;
    }

    ei_widget_t widget = ei_widget_create(type, parent_widget, NULL, NULL);
    ei_add_widget_to_list(name, widget);

    free(type);
    free(parent);

    parse_list_options(CONFIGURE, widget);
}

void parse_place_command()
{
    // We already know the command start with place, so we can skip it
    free(parse_name());

    char *name = parse_name();
    ei_widget_t widget = ei_parse_widget_from_name(name);
    free(name);

    if (widget == NULL)
    {
        printf("\033[0;31mError: Unknown widget in place command\033[0m\n");
        error = 0;
        return;
    }

    parse_list_options(PLACE, widget);
}

void parse_command()
{
    skip_spaces();

    if (get_current() == prefix[END_LINE][0])
    {
        parse_token(END_LINE);
        return;
    }
    else if (is_name_token())
    {
        if (is_place_token())
        {
            parse_place_command();
        }
        else
        {
            parse_widget_command();
        }

        skip_spaces();

        parse_token(END_LINE);
    }

    return;
}

void parse_lists_commands()
{
    skip_spaces();

    while (true)
    {
        if (error == 0)
        {
            return;
        }

        if (is_name_token() || get_current() == prefix[END_LINE][0])
        {
            parse_command();
        }
        else if (get_current() == prefix[END_OF_FILE][0])
        {
            break;
        }
        else
        {
            printf("\033[0;31mError: Expected a name token, new line or end of line, got %c\033[0m\n", get_current());
            error = 0;
            return;
        }
    }
}

void parse_ig()
{
    parse_lists_commands();

    parse_token(END_OF_FILE);
}

int ei_parse_file(char *file_path)
{
    FILE *file = fopen(file_path, "r");

    if (file == NULL)
    {
        perror("Erreur lors de l'ouverture du fichier");
        return 0;
    }

    init_current();

    error = 1;

    // Allocate an arbitrary size at the start
    int size_allocated = 100;
    characters = malloc(sizeof(char) * size_allocated);
    char c;
    int i = 0;

    while ((c = fgetc(file)) != EOF)
    {
        // If we have reached the allocated size, double it
        if (i == size_allocated)
        {
            size_allocated *= 2;
            characters = realloc(characters, sizeof(char) * size_allocated);
        }

        // If we read a backslash, check if the next character is a \n, in which case,
        // ignore it. Otherwise, put the character back in the stream.
        // We do this because if we see an \\n, it's an IGNORE_END_LINE token
        if (c == 92)
        {
            int next = getc(file);
            if (next == 10)
            {
                continue;
            }
            else
            {
                ungetc(next, file);
            }
        }

        characters[i++] = c;
    }

    characters[i] = '\0';

    fclose(file);

    char *root_name = malloc(sizeof(char) * 5);
    strcpy(root_name, "root");

    // Add the root widget to the list
    ei_add_widget_to_list(root_name, ei_app_root_widget());

    parse_lists_commands();

    free(characters);
    characters = NULL;

    ei_empty_list_number();
    ei_empty_list_name();

    return error;
}