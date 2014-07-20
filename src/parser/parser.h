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

/**
 * @var COMMAND_FLAG
 * @brief Flag found at the beginning of the game commands.
 */
const char COMMAND_FLAG = '&';

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
    int commandCount;
    const char **commands;
    int (**handlers)(char *line_noCommand, struct Parser *parser);
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
int parse(const char *line, struct Parser *parser);

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
