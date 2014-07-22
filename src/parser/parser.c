#include "parser.h"
#include <cruceGame.h>
#include <stdlib.h>
#include <string.h>

#define WRONG_COMMAND 2

#define HANDLER_NAME(name) on##name

#define BEGIN_PARSER_HANDLER(command)                                     \
    int HANDLER_NAME(command)(const char *line, struct Parser *parser) {  \
        if (line == NULL || parser == NULL)                               \
            return POINTER_NULL;                                          \
        if (strncmp((line), #command, strlen(#command)) != 0)             \
            return WRONG_COMMAND;

#define END_PARSER_HANDLER            \
        return NO_ERROR;              \
    }

int deleteParser(struct Parser **parser)
{
    if (parser == NULL || *parser == NULL)
        return POINTER_NULL;

    free(*parser);

    return NO_ERROR;
}

BEGIN_PARSER_HANDLER(CREATE_GAME)

    int gameLimit = atoi(line);
    parser->game = game_createGame(gameLimit);

    if (parser->game == NULL)
        return ALLOC_ERROR;

END_PARSER_HANDLER

const static parserHandlers handlers[] = {HANDLER_NAME(CREATE_GAME), NULL};

struct Parser *createParser()
{
    struct Parser *parser = malloc(sizeof(struct Parser));
    return parser;
}

int parse(const char *line, struct Parser *parser)
{
    if (line == NULL || parser == NULL)
        return POINTER_NULL;

    if (line[0] != COMMAND_FLAG)
        return CHAT_LINE;

    for (int i = 0; handlers[i] != NULL && !handlers[i](line, parser); i++);

    return NO_ERROR;
}

