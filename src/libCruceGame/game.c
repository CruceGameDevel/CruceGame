#include "game.h"
#include "errors.h"
#include "constants.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Game *game_createGame(int pointsNumber)
{
#ifndef DEBUG
    if (pointsNumber != 11 && pointsNumber != 15 && pointsNumber != 21)
        return NULL;
#endif

    struct Game *newGame = malloc(sizeof(struct Game));
    if (newGame == NULL)
        return NULL;

    for (int i = 0; i < MAX_GAME_PLAYERS; i++)
        newGame->players[i] = NULL;

    for (int i = 0; i < MAX_GAME_TEAMS; i++)
        newGame->teams[i] = NULL;

    newGame->pointsNumber = pointsNumber;
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
        if (game->players[i] != NULL &&
            !strcmp(game->players[i]->name, player->name))
            return DUPLICATE_NAME;
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
    game->numberPlayers--;
    
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
        if (game->teams[i] != NULL) {
            score[i] = team_computeScore(game->teams[i]);
            if (score[i] >= game->pointsNumber)
                checkTeams++;
        } else {
            score[i] = 0;
        }
    }

    if (checkTeams == 0)
        return NULL;
    if (checkTeams == 1) {
        for (int i = 0; i < MAX_GAME_TEAMS; i++)
            if (score[i] >= game->pointsNumber)
                return game->teams[i];
    }
    if (checkTeams > 1) {
        game->pointsNumber += 10;
        return NULL;
    }

    return NULL;
}

int game_maximumValue(struct Card *cards[], int length, enum Suit suit)
{
    int maxValue = -1;
    for (int i = 0; i < length; i++)
        if (cards[i] != NULL && cards[i]->suit == suit &&
            cards[i]->value > maxValue)
        maxValue = cards[i]->value;

    return maxValue;
}

int game_checkCard(struct Player *player, struct Game *game,
                   struct Hand *hand, int idCard)
{
    if (player == NULL)
        return PLAYER_NULL;
    if (game == NULL)
        return GAME_NULL;
    if (hand == NULL)
        return HAND_NULL;
    if (game->numberPlayers == 0)
        return GAME_EMPTY;
    if (game->numberPlayers == 1)
        return INSUFFICIENT_PLAYERS;
    if (game->numberPlayers * MAX_CARDS > DECK_SIZE &&
       (idCard < 0 || idCard > DECK_SIZE / game->numberPlayers - 1))
        return ILLEGAL_VALUE;
    if (idCard < 0 || idCard > MAX_CARDS - 1)
        return ILLEGAL_VALUE;
    if (player->hand[idCard] == NULL)
        return CARD_NULL;
    if (hand->cards[0] == NULL)
        return 1;

    int maxFirstCardValuePlayer = game_maximumValue(player->hand, MAX_CARDS,
                                                    hand->cards[0]->suit);
    int maxTrumpValuePlayer     = game_maximumValue(player->hand, MAX_CARDS,
                                                    game->round->trump);
    int maxFirstCardValue       = game_maximumValue(hand->cards,
                                                    MAX_GAME_PLAYERS,
                                                    hand->cards[0]->suit);
    int maxTrumpValue           = game_maximumValue(hand->cards,
                                                    MAX_GAME_PLAYERS,
                                                    game->round->trump);

    struct Card *chosenCard = player->hand[idCard];
    struct Card *firstCard  = hand->cards[0];
    enum Suit trump         = game->round->trump;

    if ((maxFirstCardValuePlayer == -1 && maxTrumpValuePlayer == -1) ||
        (chosenCard->suit == firstCard->suit &&
        (chosenCard->value > maxFirstCardValue ||
        maxFirstCardValuePlayer < maxFirstCardValue || (maxTrumpValue > -1 &&
        (firstCard->suit != trump || chosenCard->value > maxTrumpValue)))) ||
        (maxFirstCardValuePlayer == -1 && chosenCard->suit == trump &&
        (chosenCard->value > maxTrumpValue ||
        maxTrumpValue > maxTrumpValuePlayer))) 
        return 1;

    return 0;
}

int game_findNextAllowedCard(struct Player *player, struct Game *game,
                             struct Hand *hand, int currentCard)
{
    if (player == NULL)
        return PLAYER_NULL;
    if (game == NULL)
        return GAME_NULL;
    if (hand == NULL)
        return HAND_NULL;
    if (game->numberPlayers * MAX_CARDS > DECK_SIZE &&
       (currentCard < 0 || currentCard > DECK_SIZE / game->numberPlayers - 1))
        return ILLEGAL_VALUE;

    currentCard++;
    while (player->hand[currentCard] == NULL ||
        game_checkCard(player, game, hand, currentCard % MAX_CARDS) != 1)
            currentCard++;
    currentCard = currentCard % MAX_CARDS;

    return currentCard;
}

int game_findPreviousAllowedCard(struct Player *player, struct Game *game,
                                 struct Hand *hand, int currentCard)
{
    if (player == NULL)
        return PLAYER_NULL;
    if (game == NULL)
        return GAME_NULL;
    if (hand == NULL)
        return HAND_NULL;
    if (game->numberPlayers * MAX_CARDS > DECK_SIZE &&
       (currentCard < 0 || currentCard > DECK_SIZE / game->numberPlayers - 1))
        return ILLEGAL_VALUE;

    currentCard--;
    while (player->hand[currentCard] == NULL ||
        game_checkCard(player, game, hand, currentCard % MAX_CARDS) != 1) {
            currentCard--;
        if (currentCard < 0)
            currentCard = MAX_CARDS - 1;
    }
    currentCard = currentCard % MAX_CARDS;

    return currentCard;
}

