#include "team.h"
#include "constants.h"
#include "errors.h"
#include <stdlib.h>

#include <stdio.h>

struct Player *team_createPlayer(char *name, int sockfd, int isHuman)
{
    if (name == NULL)
        return NULL;

    static int id = 0; //needs rethinking
    struct Player *newPlayer = malloc(sizeof(struct Player));

    if (newPlayer == NULL)
        return NULL;

    newPlayer->name    = name;
    newPlayer->id      = id++;
    newPlayer->score   = 0;
    newPlayer->sockfd  = sockfd;
    newPlayer->isHuman = isHuman;

    return newPlayer;
}

struct Team *team_createTeam(char *name)
{
    if (name == NULL)
        return NULL;

    static int id = 0; //needs rethinking
    struct Team *newTeam = malloc(sizeof(struct Team));

    if (newTeam == NULL)
        return NULL;

    newTeam->id   = id++;
    newTeam->name = name;
    newTeam->players[0] = NULL;
    newTeam->players[1] = NULL;

    return newTeam;
}

int team_addPlayer(struct Team *team, struct Player *player)
{
    int i;
    
    if (team == NULL) 
        return TEAM_NULL;
    if (player == NULL) 
        return PLAYER_NULL;
        
    for (i = 0; i < MAX_PLAYERS; i++)
        if (team->players[i] == player) 
            return DUPLICATE;
    
    for (i = 0; i<MAX_PLAYERS; i++)
        if (team->players[i] == NULL) {
                team->players[i] = player;
                return NO_ERROR;
    }
    
    return TEAM_FULL;
}

int team_removePlayer(struct Team *team, struct Player *player)
{
    if (team == NULL)
        return TEAM_NULL;
    if (player == NULL)
        return PLAYER_NULL;
        
    for (int i = 0; i < MAX_PLAYERS; i++)
        if (team->players[i] == player) {
            team->players[i] = NULL;
            return NO_ERROR;
        }
        
    return NOT_FOUND;
}
