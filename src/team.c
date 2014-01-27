#include "team.h"
#include "errors.h"
#include <stdlib.h>

struct Player *team_createPlayer(char *name, int sockfd, int isHuman)
{
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
        return -1;
    if (player == NULL) 
        return -1;
        
    for (i = 0; i < MAX_PLAYERS; i++)
        if (team->players[i] == player) 
            return -1;
    
    for (i = 0; i<MAX_PLAYERS; i++)
        if (team->players[i] == NULL) {
                team->players[i] = player;
                return 0;
            }
    
    return -1;
}

int team_removePlayer(struct Team *team, struct Player *player)
{
    if (team == NULL)
        return -1;
    if (player == NULL)
        return -1;
        
    for (int i = 0; i < MAX_PLAYERS; i++)
        if (team->players[i] == player) {
            free(team->players[i]);
            team->players[i] = NULL;
            return 0;
        }
        
    return -1;
}
