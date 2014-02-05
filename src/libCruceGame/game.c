#include "game.h"
#include "errors.h"
#include "constants.h"

#include <stdio.h>
#include <stdlib.h>

struct Game *game_createGame(int numberPoints)
{
    struct Game *newGame = malloc(sizeof(struct Game));
    if (newGame == NULL)
        return NULL;

    for (int i = 0; i < MAX_GAME_PLAYERS; i++)
        newGame->players[i] = NULL;

    for (int i = 0; i < MAX_GAME_TEAMS; i++)
        newGame->teams[i] = NULL;

    if (numberPoints > 0)
        newGame->numberPoints = numberPoints;
    else
        return NULL;

    newGame->numberPlayers = 0;
    newGame->round = NULL;
    newGame->deck = NULL;

    return newGame;
}

int game_deleteGame(struct Game **game)
{
    if (game == NULL)
        return POINTER_NULL;
    if (*game == NULL)
        return GAME_NULL;

    free(*game);
    *game = NULL;

    return NO_ERROR;
}

int game_addPlayer(struct Player *player, struct Game *game)
{
    if (player == NULL)
        return PLAYER_NULL;
    if (game == NULL)
        return GAME_NULL;

    for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
        if (game->players[i] == player)
            return DUPLICATE;
    }

    for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
        if (game->players[i] == NULL) {
            game->players[i] = player;
            game->numberPlayers++;
            return NO_ERROR;
        }
    }

    return FULL;
}

int game_removePlayer(struct Player *player, struct Game *game)
{
    if (player == NULL)
        return PLAYER_NULL;
    if (game == NULL)
        return GAME_NULL;

    int i = 0;
    while (i < MAX_GAME_PLAYERS && game->players[i] != player)
        i++;

    if (i == MAX_GAME_PLAYERS)
        return NOT_FOUND;

    game->players[i] = NULL;

    return NO_ERROR;
}

int game_addTeam(struct Team *team, struct Game *game)
{
    if (team == NULL)
        return TEAM_NULL;
    if (game == NULL)
        return GAME_NULL;

    for (int i = 0; i < MAX_GAME_TEAMS; i++) {
        if (game->teams[i] == team)
            return DUPLICATE;
    }

    for (int i = 0; i < MAX_GAME_TEAMS; i++) {
        if (game->teams[i] == NULL) {
            game->teams[i] = team;
            return NO_ERROR;
        }
    }

    return FULL;
}

int game_removeTeam(struct Team *team, struct Game *game)
{
    if (team == NULL)
        return TEAM_NULL;
    if (game == NULL)
        return GAME_NULL;

    int i = 0;
    while (i < MAX_GAME_TEAMS && game->teams[i] != team)
        i++;

    if (i == MAX_GAME_TEAMS)
        return NOT_FOUND;

    game->teams[i] = NULL;

    return NO_ERROR;
}

struct Team *game_winningTeam(struct Game *game)
{
    if (game == NULL)
        return NULL;

    int score[MAX_GAME_TEAMS];
    int checkTeams = 0;
    for (int i = 0; i < MAX_GAME_TEAMS; i++) {
        score[i] = 0;
        if (game->teams[i] != NULL) {
            for (int j = 0; j < MAX_TEAM_PLAYERS; j++) {
                if (game->teams[i]->players[j] != NULL) {
                    score[i] += game->teams[i]->players[j]->score;
                }
            }
            if (score[i] >= game->numberPoints)
                checkTeams++;
        }
    }

    if (checkTeams == 0)
        return NULL;
    if (checkTeams == 1) {
        for (int i = 0; i < MAX_GAME_TEAMS; i++)
            if (score[i] >= game->numberPoints)
                return game->teams[i];
    }
    if (checkTeams > 1) {
        game->numberPoints += 10;
        return NULL;
    }

    return NULL;
}


