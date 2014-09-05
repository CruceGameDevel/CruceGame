#include "parser.h"
#include "../multiplayerCurses/cli.h"
#include <ncurses.h>
#include <cruceGame.h>
#include <network.h>
#include <stdlib.h>
#include <string.h>

static struct Parser *parser;

#define WRONG_COMMAND 2
#define ROOM_NAME "#cruce-game"
#define NUMBER_LEN 3

#define HANDLER_NAME(name) on##name

#define BEGIN_PARSER_HANDLER(command)                                     \
    int HANDLER_NAME(command)(const char *line) {  \
        if (line == NULL)                               \
            return POINTER_NULL;                                          \
        if (strncmp((line + 1), #command, strlen(#command)) != 0)             \
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

char *issueRoomName(int channelNo)
{
    char *roomName = malloc(strlen(ROOM_NAME) + NUMBER_LEN + 100);
    char format[5 + NUMBER_LEN];
    sprintf(format, "%%s%%.%dd", NUMBER_LEN);
    sprintf(roomName, format, ROOM_NAME, channelNo);
    return roomName;
}

BEGIN_PARSER_HANDLER(CREATE_GAME)

    wprintw(getPrintWin(), "CALLED\n");
    int roomCount = 0;
    int foundRoom = 0;
    while (!foundRoom) {
        roomCount++;
        char *roomName = issueRoomName(roomCount);
        foundRoom = !(channelExists(roomName));
        wprintw(getPrintWin(), "%s %d\n", roomName, foundRoom);
        
        free(roomName);
    }
    wprintw(getPrintWin(), "SUCCESSSSSS%d\n", roomCount);
    wrefresh(getPrintWin());

//    int gameLimit = atoi(line);
//    parser->game = game_createGame(gameLimit);
//
//    if (parser->game == NULL)
//        return ALLOC_ERROR;

END_PARSER_HANDLER

const static parserHandlers handlers[] = {HANDLER_NAME(CREATE_GAME), NULL};

int parse(const char *line)
{
    wprintw(getPrintWin(), "PARSING\n");
//    if (line == NULL || parser == NULL)
//        return POINTER_NULL;
//
//    if (line[0] != GAME_COMMAND_FLAG)
//        return CHAT_LINE;

//    for (int i = 0; handlers[i] != NULL && !handlers[i](line); i++);
    wprintw(getPrintWin(), "%d\n", channelExists("#linudfdx"));
//    wprintw(getPrintWin(), "%d\n", onCREATE_GAME(line));

    return NO_ERROR;
}

