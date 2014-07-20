#include "parser.h"
#include <cruceGame.h>
#include <stdlib.h>
#include <string.h>

struct Parser *createParser()
{
    struct Parser *parser = malloc(sizeof(struct Parser));
    return parser;
}

int deleteParser(struct Parser *parser)
{
    if (parser == NULL)
        return POINTER_NULL;

    free(parser);

    return NO_ERROR;
}

int parser(char *line, struct Parser *parser)
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
