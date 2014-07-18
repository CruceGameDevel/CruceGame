#include "parser.h"
#include <cruceGame.h>
#include <stdlib.h>

int parser(char *line, struct Parser *parser)
{
    if (line == NULL || parser == NULL)
        return POINTER_NULL;

    if (line[0] != COMMAND_FLAG)
        return CHAT_LINE;

    return NO_ERROR;
}
