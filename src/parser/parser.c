#include "parser.h"
#include <cruceGame.h>
#include <stdlib.h>
#include <string.h>

static struct Parser *parser;

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

int initParser()
{
    parser = malloc(sizeof(struct Parser));
    if (parser == NULL)
        return ALLOC_ERROR;
    return NO_ERROR;
}

int teardownParser()
{
    if (parser == NULL)
        return POINTER_NULL;

    free(parser);
    parser = NULL;

    return NO_ERROR;
}

BEGIN_PARSER_HANDLER(CREATE_GAME)

    int gameLimit = atoi(line);
    parser->game = game_createGame(gameLimit);

    if (parser->game == NULL)
        return ALLOC_ERROR;

END_PARSER_HANDLER

const static parserHandlers handlers[] = {HANDLER_NAME(CREATE_GAME), NULL};

int parse(const char *line, struct Parser *parser)
{
    if (line == NULL || parser == NULL)
        return POINTER_NULL;

    if (line[0] != COMMAND_FLAG)
        return CHAT_LINE;

    for (int i = 0; handlers[i] != NULL && !handlers[i](line, parser); i++);

    return NO_ERROR;
}

