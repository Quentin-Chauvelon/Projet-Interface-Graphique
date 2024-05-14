/**
 * @file    ei_lexer.h
 *
 * @brief   Defines functions for the lexical analysis
 *
 * @authors Seydina BADIANE
 * @authors Christian BOSSE
 * @authors Quentin CHAUVELON
 */

#ifndef EI_LEXER_H
#define EI_LEXER_H

extern char *characters;
static int current_char;

enum token
{
    OB,
    CB,
    EQUAL,
    DOT,
    END_LINE,
    IGNORE_END_LINE,
    END_OF_FILE
};

static const char *prefix[] = {
    "{",
    "}",
    "=",
    ".",
    "\n",
    "\\n",
    "\0"};

void init_current();

char get_current();

void update_current();

void skip_spaces();

bool is_digit_token();

bool is_name_token();

bool is_place_token();

int parse_digit();

int parse_integer();

double parse_number();

char *parse_name();

char *parse_widget_type();

char *parse_widget_name();

char *parse_parent_name();

void parse_token(enum token token);

#endif