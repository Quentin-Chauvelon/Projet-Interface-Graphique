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

int parse_digit()
{
    if (get_current() >= '0' && get_current() <= '9')
    {
        // Convert the character to an integer
        int result = get_current() - '0';

        update_current();

        return result;
    }
}

int parse_token_integer()
{
    skip_spaces();

    int result = 0;

    while (get_current() >= '0' && get_current() <= '9')
    {
        result = result * 10 + parse_digit();
    }

    skip_spaces();

    return result;
}

double parse_token_real()
{
    skip_spaces();

    int integer = 0;

    if (is_current_token(INTEGER))
    {
        integer = parse_token_integer();
    }

    parse_token(DOT);

    int decimal = parse_token_integer();

    int nb_digits = 0;
    int tmp = decimal;

    // Count the number of digits in decimal (dividing a int by 10 floors the result instead of rounding,
    // which we can use to know once tmp is under 10 (9 / 10 = 0))
    while (tmp != 0)
    {
        tmp /= 10;
        nb_digits++;
    }

    skip_spaces();

    return integer + (double)decimal / pow(10, nb_digits);
}

void parse_token_comment()
{
    while (get_current() != '\n')
    {
        update_current();
    }
}

char *parse_token_name()
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
               get_current() == '_' ||
               get_current() == '/' || // The '/' and '.' characters are not part of the grammar but were added to handle image paths
               get_current() == '.')
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

        skip_spaces();

        return name;
    }
    else
    {
        printf("\033[0;31mError: Expected a-z or A-Z or '_', got %c\033[0m\n", get_current());
        error = 0;
        return NULL;
    }
}

bool is_current_token(token token)
{
    skip_spaces();

    if (error == 0)
    {
        return false;
    }

    if (token == WS)
    {
        return get_current() == ' ' || get_current() == '\t';
    }
    else if (token == NAME)
    {
        return (get_current() >= 'a' && get_current() <= 'z') ||
               (get_current() >= 'A' && get_current() <= 'Z') ||
               get_current() == '_';
    }
    else if (token == PLACE)
    {
        return get_current() == 'p';
    }
    else if (token == INTEGER)
    {
        return get_current() >= '0' && get_current() <= '9';
    }
    else if (token == REAL)
    {
        return get_current() >= '0' && get_current() <= '9' || get_current() == '.';
    }
    else if (token == COMMENT)
    {
        return get_current() == '#';
    }
    else
    {
        return get_current() == prefix[token][0];
    }
}

void *parse_token(token token)
{
    if (error == 0)
    {
        return NULL;
    }

    void *value = NULL;

    if (!is_current_token(token))
    {
        printf("\033[0;31mError: Expected %s, got %c\033[0m\n", prefix[token], get_current());
        error = 0;
    }

    skip_spaces();

    if (token == NAME)
    {
        char *name = parse_token_name();
        value = &name;
    }
    else if (token == PLACE)
    {
        char *name = parse_token_name();
        value = &name;
    }
    else if (token == INTEGER)
    {
        int integer = parse_token_integer();
        value = &integer;
    }
    else if (token == REAL)
    {
        double real = parse_token_real();
        value = &real;
    }
    else if (token == COMMENT)
    {
        parse_token_comment();
        return NULL;
    }
    else if (token == END_OF_FILE)
    {
        return NULL;
    }
    else
    {
        update_current();
    }

    skip_spaces();

    return value;
}