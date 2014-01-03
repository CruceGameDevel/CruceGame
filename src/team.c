#include "team.h"
#include <stdlib.h>

struct Player *createPlayer(char *name, int sockfd, int isHuman)
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

struct Team *createTeam(char *name)
{
    static int id = 0; //needs rethinking
    struct Team *newTeam = malloc(sizeof(struct Team));

    newTeam->id   = id++;
    newTeam->name = name;

    return newTeam;
}

