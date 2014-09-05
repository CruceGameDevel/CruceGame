/**
 * @file parser.h
 * @brief The parser module parses all the commands recieved and performs
 *        the required actions.
 */

#ifndef PARSER_H
#define PARSER_H

/**
 * @brief Return code for char lines.
 */
#define CHAT_LINE 1

struct Parser;

/**
 * \typedef Represents a function pointer data type to parser handlers.
 */
typedef int (*parserHandlers)(const char *line);

/**
 * @struct Parser
 * @brief Holds all the data needed to parse commands and perform required
 *        actions.
 *
 * @var Parser::game Pointer to a game structure that holds information about
 *                   the current game.
 */
struct Parser{
    struct Game *game;
    parserHandlers *handlers;
};

/**
 * The main parser function. This is the function that parses the string and
 * performs the operations needed.
 *
 * The lines that begin with & are considered game commands.
 *
 * @param line The line to be parsed.
 *
 * @return negative value for failure, \ref NO_ERROR if the string was parsed
 *         successfully and it's a game command and positive value if the
 *         string is a chat line.
 */
int parse(const char *line);

/**
 * Function to allocate memory for a Parser structure. The allocated memory
 * must be freed with deleteParser().
 *
 * @return void.
 */
struct Parser *createParser();

/**
 * Function to free memory for a Parser.
 *
 * @return \ref NO_ERROR on success, negative value on failure.
 */
int deleteParser(struct Parser **parser);

#endif
