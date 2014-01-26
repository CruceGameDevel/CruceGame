#include "team.h"
#include <stdlib.h>

struct Player *team_createPlayer(char *name, int sockfd, int isHuman)
{
    static int id = 0; //needs rethinking
    struct Player *newPlayer = malloc(sizeof(struct Player));

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

    newTeam->id   = id++;
    newTeam->name = name;
    newTeam->players[0] = NULL;
    newTeam->players[1] = NULL;

    return newTeam;
}

int team_addPlayer(struct Team *team, struct Player *player)
{
    int i;
    int freePlace=0; //0 If not more is free place in team. Otherwise non-zero. 
    
    if (team == NULL) return -1;
    if (player == NULL) return -1;
    
    for(i=0; i<MAX_PLAYERS; i++)
    {
        if (team->players[i] == NULL) 
            {
                freePlace++;
                break;
            }
    }
    
    if (freePlace == 0) return -1;
    else 
    {
        team->players[i] = player;
        return 0;
    }
}