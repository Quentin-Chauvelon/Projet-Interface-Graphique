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

extern char *characters; // A char array holding the content of the file to parse
static int current_char; // The current character to parse

// Enum of the tokens from the lexical analysis
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

// Array to associate a token with a string
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

/**
 * @brief   Initialize the current character to the beggining of the characters array
 */
void init_current();

/**
 * @brief   Move the current character to the next one
 */
void update_current();

/**
 * @brief   Return the current character
 *
 * @return  The current character
 */
char get_current();

/**
 * @brief   Parse an integer token and return its value
 *
 * @return  The integer value of the token
 */
int parse_token_integer();

/**
 * @brief   Parse a real token and return its value
 *
 * @return  The real value of the token
 */
double parse_token_real();

/**
 * @brief   Parse a name token and return its value
 *
 * @return  The name value of the token
 */
char *parse_token_name();

/**
 * @brief   Check and return if the current token matches the given token
 *
 * @param   token   The token to check
 *
 * @return  True if the current token matches the given token, false otherwise
 */
bool is_current_token(token token);

/**
 * @brief   Parse the current token and return its value
 *
 * @param   token   The token to parse
 *
 * @return  The value of the token
 */
void *parse_token(token token);

#endif