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
    if (team == NULL) return -1;
    if (player == NULL) return -1;
    if (team->players[0] != NULL && team->players[1] != NULL) return -1;
    if (team->players[0] == NULL)
    {
        team->players[0]->name    = player->name;
        team->players[0]->id      = player->id;
        team->players[0]->score   = player->score;
        team->players[0]->sockfd  = player->sockfd;
        team->players[0]->isHuman = player->isHuman;
        return 0;
    }
    if (team->players[1] == NULL)
    {
        team->players[1]->name    = player->name;
        team->players[1]->id      = player->id;
        team->players[1]->score   = player->score;
        team->players[1]->sockfd  = player->sockfd;
        team->players[1]->isHuman = player->isHuman;
        return 0;
    }
}