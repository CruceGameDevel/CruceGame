#include "parser.h"
#include <cruceGame.h>
#include <stdlib.h>
#include <string.h>

#define WRONG_COMMAND 2

#define BEGIN_PARSER_HANDLER(command)                                     \
    int on ##command(const char *line_noCommand, struct Parser *parser) { \
        handlers[handlersEnd++] = on ##command;                           \
        handlers[handlersEnd]   = NULL;                                   \
        if (line_noCommand == NULL || parser == NULL)                     \
            return POINTER_NULL;                                          \
        if (strncmp(line_noCommand), #command, strlen(#command) != 0)     \
            return WRONG_COMMAND;

#define END_PARSER_HANDLER }

int deleteParser(struct Parser **parser)
{
    if (parser == NULL || *parser == NULL)
        return POINTER_NULL;

    free(*parser);

    return NO_ERROR;
}

int onCreateGame(const char *line_noCommand, struct Parser *parser)
{
    if (line_noCommand == NULL || parser == NULL)
        return POINTER_NULL;

    int gameLimit = atoi(line_noCommand);
    parser->game = game_createGame(gameLimit);

    if (parser->game == NULL)
        return ALLOC_ERROR;

    return NO_ERROR;
}

parserHandlers handlers[] = {onCreateGame};
const char *command[] = {"CREATE_GAME"};

struct Parser *createParser()
{
    struct Parser *parser = malloc(sizeof(struct Parser));
    parser->commandCount = sizeof(handlers) / sizeof(void*);
    return parser;
}

int parser(const char *line, struct Parser *parser)
{
    if (line == NULL || parser == NULL)
        return POINTER_NULL;

    if (line[0] != COMMAND_FLAG)
        return CHAT_LINE;

    for (int i = 0; i < parser->commandCount; i++) {
        if (strncmp(line, command[i], strlen(command[i])) == 0)
            return handlers[0](line + strlen(command[i]), parser);
    }
    return NO_ERROR;
}
