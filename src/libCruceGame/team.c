#include "team.h"
#include "constants.h"
#include "errors.h"
#include <stdlib.h>

#include <string.h>
#include <stdio.h>

struct Player *team_createPlayer(const char *name, int isHuman)
{
    if (name == NULL)
        return NULL;

    static int id = 0; //needs rethinking
    struct Player *newPlayer = malloc(sizeof(struct Player));

    if (newPlayer == NULL)
        return NULL;

    newPlayer->name    = malloc((strlen(name)+1) * sizeof(char));

    if(newPlayer->name != NULL) 
        strcpy(newPlayer->name, name);
    else 
        return NULL;

    newPlayer->id      = id++;
    newPlayer->score   = 0;
    newPlayer->isHuman = isHuman;

    for (int i = 0; i < MAX_CARDS; i++)
        newPlayer->hand[i] = NULL;

    return newPlayer;
}

struct Team *team_createTeam(const char *name)
{
    if (name == NULL)
        return NULL;

    static int id = 0; //needs rethinking
    struct Team *newTeam = malloc(sizeof(struct Team));

    if (newTeam == NULL)
        return NULL;

    newTeam->id   = id++;
    newTeam->name = malloc((strlen(name) + 1) * sizeof(char));
    
    if(newTeam->name == NULL)
        return NULL;
    strcpy(newTeam->name, name);

    newTeam->players[0] = NULL;
    newTeam->players[1] = NULL;

    return newTeam;
}

int team_addPlayer(struct Team *team, struct Player *player)
{
    if (team == NULL)
        return TEAM_NULL;
    if (player == NULL)
        return PLAYER_NULL;

    for (int i = 0; i < MAX_TEAM_PLAYERS; i++)
        if (team->players[i] == player)
            return DUPLICATE;

    for (int i = 0; i < MAX_TEAM_PLAYERS; i++)
        if (team->players[i] == NULL) {
            team->players[i] = player;
            return NO_ERROR;
    }

    return TEAM_FULL;
}

int team_removePlayer(struct Team *team,const struct Player *player)
{
    if (team == NULL)
        return TEAM_NULL;
    if (player == NULL)
        return PLAYER_NULL;

    for (int i = 0; i < MAX_TEAM_PLAYERS; i++)
        if (team->players[i] == player) {
            team->players[i] = NULL;
            return NO_ERROR;
        }

    return NOT_FOUND;
}

int team_deleteTeam(struct Team **team)
{
    if (team == NULL)
        return POINTER_NULL;
    if (*team == NULL)
        return TEAM_NULL;

    free((*team)->name);
    free(*team);
    *team = NULL;

    return NO_ERROR;
}

int team_deletePlayer(struct Player **player)
{
    if (player == NULL)
        return POINTER_NULL;
    if (*player == NULL)
        return PLAYER_NULL;

    free((*player)->name);
    free(*player);
    *player = NULL;

    return NO_ERROR;
}


int team_computeScore(const struct Team *team)
{
    if(team == NULL)
        return TEAM_NULL;

    int bool_atLeastOnePlayerInTeam = 0;
    for (int i = 0; i < MAX_TEAM_PLAYERS; i++) {
        if(team->players[i] != NULL) {
            bool_atLeastOnePlayerInTeam = 1;
            break;
        }
            
    }
    if(!bool_atLeastOnePlayerInTeam)
        return TEAM_EMPTY;

    int returnScore = 0;
    for (int i = 0; i < MAX_TEAM_PLAYERS; i++)
        if(team->players[i] != NULL)
            returnScore += (team->players[i])->score;

    return returnScore;
}

int team_addCard(struct Player *player,struct Card *card)
{
    if (player == NULL)
        return PLAYER_NULL;
    if (card == NULL)
        return CARD_NULL;

    for (int i = 0; i < MAX_CARDS; i++)
        if (player->hand[i] == card)
            return DUPLICATE;        

    for (int i = 0; i < MAX_CARDS; i++) {
        if (player->hand[i] == NULL) {
            player->hand[i] = card;
            return NO_ERROR;
        }
    }

    return FULL;
}

int team_hasCards(struct Player *player)
{
    if(player == NULL)
        return PLAYER_NULL;
    
    for(int i = 0; i < MAX_CARDS; i++)
        if(player->hand[i] != NULL)
            return 1;
    
    return 0;
}
