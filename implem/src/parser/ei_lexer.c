#include <stdio.h>
#include <stdbool.h>

#include "../implem/headers/parser/ei_parser_types.h"
#include "../implem/headers/parser/ei_lexer.h"

char *characters = NULL;

void init_current()
{
    current_char = 0;
}

char get_current()
{
    return characters[current_char];
}

void update_current()
{
    current_char++;
}

void skip_spaces()
{
    while (get_current() == ' ' || get_current() == '\t')
    {
        update_current();
    }
}

bool is_digit_token()
{
    skip_spaces();

    return get_current() >= '0' && get_current() <= '9' || get_current() == '.';
}

bool is_name_token()
{
    skip_spaces();

    return (get_current() >= 'a' && get_current() <= 'z') ||
           (get_current() >= 'A' && get_current() <= 'Z') ||
           get_current() == '_';
}

bool is_place_token()
{
    skip_spaces();

    return get_current() == 'p' &&
           characters[current_char + 1] != '\0' && characters[current_char + 1] == 'l' &&
           characters[current_char + 2] != '\0' && characters[current_char + 2] == 'a' &&
           characters[current_char + 3] != '\0' && characters[current_char + 3] == 'c' &&
           characters[current_char + 4] != '\0' && characters[current_char + 4] == 'e';
}

int parse_digit()
{
    skip_spaces();

    if (get_current() >= '0' && get_current() <= '9')
    {
        // Convert the character to an integer
        int result = get_current() - '0';

        update_current();

        return result;
    }
}

int parse_integer()
{
    skip_spaces();

    int result = 0;

    while (get_current() >= '0' && get_current() <= '9')
    {
        result = result * 10 + parse_digit();
    }

    return result;
}

double parse_number()
{
    skip_spaces();

    int integer = get_current() != '.'
                      ? parse_integer()
                      : 0;

    if (get_current() == '.')
    {
        parse_token(DOT);

        int decimal = parse_integer();

        int nb_digits = 0;
        int tmp = decimal;

        // Count the number of digits in decimal (dividing a int by 10 floors the result instead of rounding,
        // which we can use to know once tmp is under 10 (9 / 10 = 0))
        while (tmp != 0)
        {
            tmp /= 10;
            nb_digits++;
        }

        return integer + decimal / pow(10, nb_digits);
    }
    else
    {
        return integer;
    }
}

char *parse_name()
{
    skip_spaces();

    if ((get_current() >= 'a' && get_current() <= 'z') ||
        (get_current() >= 'A' && get_current() <= 'Z') ||
        get_current() == '_')
    {
        int start_position = current_char;
        int length = 1;

        update_current();

        while ((get_current() >= 'a' && get_current() <= 'z') ||
               (get_current() >= 'A' && get_current() <= 'Z') ||
               (get_current() >= '0' && get_current() <= '9') ||
               get_current() == '_')
        {
            length++;
            update_current();
        }

        char *name = malloc(sizeof(char) * (length + 1));

        for (int i = 0; i < length; i++)
        {
            name[i] = characters[start_position + i];
        }

        name[length] = '\0';

        return name;
    }
    else
    {
        printf("\033[0;31mError: Expected a-z or A-Z or '_', got %c\033[0m\n", get_current());
        error = 0;
        return NULL;
    }
}

char *parse_widget_type()
{
    return parse_name();
}

char *parse_widget_name()
{
    return parse_name();
}

char *parse_parent_name()
{
    return parse_name();
}

void parse_token(enum token token)
{
    if (error == 0)
    {
        return;
    }

    skip_spaces();

    if (get_current() != prefix[token][0])
    {
        printf("\033[0;31mError: expected %s, got %c\033[0m\n", prefix[token], get_current());
        error = 0;
        return;
    }

    update_current();
}