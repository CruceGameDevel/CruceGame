/**
 * @file game.c
 * @brief This file contains implementations of the functions used 
 *        for game-related operations.
 */

#include "game.h"
#include "errors.h"
#include "constants.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Game *game_createGame(const int pointsNumber)
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

int game_removePlayer(const struct Player *player, struct Game *game)
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

int game_removeTeam(const struct Team *team, struct Game *game)
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

    int checkTeams = 0;
    for (int i = 0; i < MAX_GAME_TEAMS; i++) {
        if (game->teams[i] != NULL) {
            if (game->teams[i]->score >= game->pointsNumber)
                checkTeams++;
        }
    }

    if (checkTeams == 0)
        return NULL;
    else if (checkTeams == 1) {
        for (int i = 0; i < MAX_GAME_TEAMS; i++)
            if (game->teams[i] != NULL && 
                game->teams[i]->score >= game->pointsNumber)
                return game->teams[i];
    } else {
        game->pointsNumber += 10;
        return NULL;
    }

    return NULL;
}

int game_maximumValue(struct Card *cards[], const int length, 
                        const enum Suit suit)
{
    int maxValue = -1;
    for (int i = 0; i < length; i++)
        if (cards[i] != NULL && cards[i]->suit == suit &&
            cards[i]->value > maxValue)
        maxValue = cards[i]->value;

    return maxValue;
}

int game_checkCard(struct Player *player, const struct Game *game,
                   struct Hand *hand, const int idCard)
{
    if (player == NULL)
        return PLAYER_NULL;
    if (game == NULL)
        return GAME_NULL;
    if (hand == NULL)
        return HAND_NULL;
    if (game->numberPlayers == 0)
        return GAME_EMPTY;
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

/**
 * @brief Helper for function that search for an allowed card.
 *
 * @param player The player who's hand is searched.
 * @param game The game in which the player and the hand belongs.
 * @param hand The hand where is supposed to put the card.
 * @param currentCard The cardId to look for.
 * @param searchPattern Function searches the player's hand using a pattern
 *                      provided by this argument. Currently, there are allowed
 *                      only 1 and -1 as search patterns, altough the function
 *                      may be extended to use other patterns as well.
 *
 * @return The first allowed card found.
 */
int findAllowedCard(const struct Player *player, const struct Game *game, 
                    struct Hand *hand, int currentCard, const int searchPattern)
{
    if (player == NULL)
        return PLAYER_NULL;
    if (game == NULL)
        return GAME_NULL;
    if (hand == NULL)
        return HAND_NULL;
    if (searchPattern != 1 && searchPattern != -1)
        return ILLEGAL_VALUE;
    if (game->numberPlayers * MAX_CARDS > DECK_SIZE &&
       (currentCard < 0 || currentCard > DECK_SIZE / game->numberPlayers - 1))
        return ILLEGAL_VALUE;

    while (1) {
        currentCard += searchPattern;
        if (currentCard < 0)
            currentCard += MAX_CARDS;
        while (player->hand[currentCard % MAX_CARDS] == NULL && abs(currentCard) <= 15)
            currentCard += searchPattern;
        if (game_checkCard(player, game, hand, currentCard % MAX_CARDS) == 1)
            return currentCard % MAX_CARDS;
        if (abs(currentCard) > 15) 
             return NOT_FOUND;
    }
}

int game_findNextAllowedCard(struct Player *player, struct Game *game,
                             struct Hand *hand, int currentCard)
{
    return findAllowedCard(player, game, hand, currentCard, 1);
}

int game_findPreviousAllowedCard(struct Player *player, struct Game *game,
                                 struct Hand *hand, int currentCard)
{
    return findAllowedCard(player, game, hand, currentCard, -1);
}

struct Team *game_findTeam(const struct Game *game, struct Player *player)
{
    if (player == NULL)
        return NULL;
    if (game == NULL)
        return NULL;

    for (int i = 0; i < MAX_GAME_TEAMS; i++) {
        if (game->teams[i] != NULL) {
            for (int j = 0; j < MAX_TEAM_PLAYERS; j++)
                if (game->teams[i]->players[j] == player)
                    return game->teams[i];
        }
    }

    return NULL;
}

int game_updateScore(const struct Game *game, struct Player *bidWinner)
{
    if (game == NULL)
        return GAME_NULL;
    if (bidWinner == NULL)
        return PLAYER_NULL;

    struct Team *bidWinnerTeam = game_findTeam(game, bidWinner);
    int bidWinnerTeamId = -1;
    int teamScores[MAX_GAME_TEAMS];

    for (int i = 0; i < MAX_GAME_TEAMS; i++) {
        teamScores[i] = 0;
        if (game->teams[i] == bidWinnerTeam)
            bidWinnerTeamId = i;
    }

    if (bidWinnerTeamId == -1 || bidWinnerTeam == NULL)
        return NOT_FOUND;

    for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
        if (game->round->players[i] != NULL) {
            struct Team *team = game_findTeam(game, game->round->players[i]);
            if (team == NULL)
                return NOT_FOUND;
            for (int j = 0; j < MAX_GAME_TEAMS; j++)
                if (game->teams[j] == team)
                    teamScores[j] += game->round->pointsNumber[i];
        }
    }

    int bidWinnerId = round_findPlayerIndexRound(bidWinner, game->round);
    for (int i = 0; i < MAX_GAME_TEAMS; i++) {
        if (game->teams[i] != NULL) {
            if (game->teams[i] != bidWinnerTeam)
                game->teams[i]->score += teamScores[i] / 33;
            else if (game->round->bids[bidWinnerId] <=
                     teamScores[bidWinnerTeamId] / 33)
                bidWinnerTeam->score += teamScores[bidWinnerTeamId] / 33;
            else
                bidWinnerTeam->score -= game->round->bids[bidWinnerId];
        }
        team_updatePlayersScore(game->teams[i]);
    }

    return NO_ERROR;
}

int game_arrangePlayersRound(struct Game *game, const int i)
{
    if (game == NULL)
        return GAME_NULL;
    if (i < 0 || i >= MAX_GAME_PLAYERS)
        return ILLEGAL_VALUE;
    if (game->round != NULL)
        return FULL;

    struct Round *round = round_createRound();
    for (int j = i; j < i + MAX_GAME_PLAYERS; j++)
        if (game->players[j % MAX_GAME_PLAYERS] != NULL)
            round_addPlayer(game->players[j % MAX_GAME_PLAYERS], round);

    game->round = round;

    return NO_ERROR;
}

