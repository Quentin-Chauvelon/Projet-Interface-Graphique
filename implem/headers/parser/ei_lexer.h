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

typedef enum
{
    OB,
    CB,
    EQUAL,
    DOT,
    END_LINE,
    IGNORE_END_LINE,
    END_OF_FILE,
    PLACE,
    COMMENT,
    NAME,
    INTEGER,
    REAL,
    WS
} token;

static const char *prefix[] = {
    "{",
    "}",
    "=",
    ".",
    "\n",
    "\\n",
    "\0",
    "place",
    "#",
    "a-z or A-Z or '_'",
    "0-9",
    "0-9 or '.'",
    "' ' or '\\t'"};

void init_current();

void update_current();

char get_current();

int parse_token_integer();

double parse_token_real();

char *parse_token_name();

bool is_current_token(token token);

void *parse_token(token token);

#endif