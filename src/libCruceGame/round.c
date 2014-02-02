#include "round.h"
#include "errors.h"

#include <stdlib.h>
#include <stdio.h>

struct Round *round_createRound(enum Suit trump)
{
    struct Round *round = malloc(sizeof(struct Round));

    if (round == NULL)
        return NULL;

    round->trump = trump;

    for (int i = 0; i < MAX_HANDS; i++)
        round->hands[i] = NULL;

    return round;
}

int round_deleteRound(struct Round **round)
{
    if (round == NULL)
        return POINTER_NULL;
    if (*round == NULL)
        return ROUND_NULL;

    free(*round);
    *round = NULL;

    return NO_ERROR;
}

struct Hand *round_createHand()
{
    struct Hand *hand = malloc(sizeof(struct Hand));
    if (hand == NULL)
        return NULL;

    for (int i = 0; i < MAX_GAME_PLAYERS + 1; i++) {
        hand->cards[i]   = NULL;
        hand->players[i] = NULL;
    }

    for (int i = 0; i < MAX_GAME_PLAYERS + 1; i++) {
        hand->bids[i] = 0;
    }

    return hand;
}

int round_deleteHand(struct Hand **hand)
{
    if (hand == NULL)
        return POINTER_NULL;
    if (*hand == NULL)
        return HAND_NULL;

    free(*hand);
    *hand = NULL;

    return NO_ERROR;
}

struct Player *round_getBidWinner(struct Hand *hand)
{
    if (hand == NULL)
        return NULL;

    int maxBidIndex = 0;
    for (int i = 0; hand->players[i] != NULL; i++) {
        if (hand->bids[i] > hand->bids[maxBidIndex]) {
            maxBidIndex = i;
        }
    }
    return hand->players[maxBidIndex];
}

/**
 * @brief Helper to find player in a hand.
 *
 * @param player Player to find.
 * @param hand Hand to search for player.
 * @return 0 on success, non-zero on failure.
 */
int findPlayerIndexHand(struct Player *player, struct Hand *hand)
{
    if (player == NULL)
        return PLAYER_NULL;
    if (hand == NULL)
        return HAND_NULL;

    int i = 0;
    while (hand->players[i] != player && 
           i < MAX_GAME_PLAYERS)
        i++;

    if (i == MAX_GAME_PLAYERS)
        return NOT_FOUND;

    return i;
}

int round_placeBid(struct Player *player, int bid, struct Hand *hand)
{
    if (player == NULL)
        return PLAYER_NULL;
    if (hand == NULL)
        return HAND_NULL;
    if (bid < 0)
        return ILLEGAL_VALUE;

    int index = findPlayerIndexHand(player, hand);;

    if(index < 0)
        return NOT_FOUND;

    hand->bids[index] = bid;

    return NO_ERROR;
}

int round_addPlayer(struct Player *player, struct Hand *hand)
{
    if (player == NULL)
        return PLAYER_NULL;
    if (hand == NULL)
        return HAND_NULL;

    int index = findPlayerIndexHand(player, hand);
    if(index >= 0) //impossible to add same player multimple time
        return DUPLICATE;

    int i = 0;
    while(hand->players[i] != NULL && i < MAX_GAME_PLAYERS)
        i++;

    if (i == MAX_GAME_PLAYERS)
        return FULL;

    hand->players[i] = player;

    return 0;
}

int round_removePlayer(struct Player *player, struct Hand *hand)
{
    if (player == NULL)
        return PLAYER_NULL;
    if (hand == NULL)
        return HAND_NULL;

    int index = findPlayerIndexHand(player, hand);

    if (index < 0)
        return NOT_FOUND;

    hand->players[index] = NULL;

    return NO_ERROR;
}

int round_putCard(struct Player *player, int cardId, struct Hand *hand)
{
    if (player == NULL)
        return PLAYER_NULL;
    if (player->hand[cardId] == NULL)
        return CARD_NULL;
    if (hand == NULL)
        return HAND_NULL;

    int index = findPlayerIndexHand(player, hand);

    if(index < 0)
        return NOT_FOUND;

    hand->cards[index] = player->hand[cardId];
    player->hand[cardId] = NULL;

    return 0;
}

int round_computeScore(struct Hand *hand)
{
    if (hand == NULL)
        return HAND_NULL;

    int cardsScore = 0;
    for (int i = 0; hand->players[i] != NULL; i++) {
        if (hand->cards[i] == NULL)
            return CARD_NULL;
        cardsScore += hand->cards[i]->value;
    }

    int gameScore = cardsScore / 33;

    return gameScore;
}

struct Player *round_handWinner(struct Hand *hand, enum Suit trump)
{
    if (hand == NULL || trump == SuitEnd)
        return NULL;

    int playerWinner = -1;
    int numberPlayers = 0;
    for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
        if ((hand->players[i] != NULL && hand->cards[i] == NULL) ||
            (hand->players[i] == NULL && hand->cards[i] != NULL))
            return NULL;
        if (hand->players[i] != NULL && hand->cards[i] != NULL) {
            numberPlayers++;
            if (playerWinner == -1)
                playerWinner = i;
        }
    }
    if (playerWinner == -1 || numberPlayers == 1)
        return NULL;

    int cardWinner;
    for (int i = playerWinner + 1; i < MAX_GAME_PLAYERS; i++) {
        if (hand->players[i] != NULL && hand->cards[i] != NULL) {
            cardWinner = deck_compareCards(hand->cards[playerWinner],
                                               hand->cards[i], trump);
            if (cardWinner == 2)
                playerWinner = i;
            if (cardWinner == 0)
                return NULL;
        }
    }

    return hand->players[playerWinner];
}

int round_distributeCard(struct Deck *deck, struct Hand *hand)
{
    if (deck == NULL)
        return DECK_NULL;
    if (hand == NULL)
        return HAND_NULL;

    int i, j;
    int distributedCards = 0;
    for (i = 0, j = 0; i < MAX_GAME_PLAYERS && j < DECK_SIZE; i++, j++) {
        //do not change while's order
        while (deck->cards[j] == NULL && j < DECK_SIZE)
            j++;
        while (hand->players[i] == NULL && i < MAX_GAME_PLAYERS)
            i++;
        if (i < MAX_GAME_PLAYERS && j < DECK_SIZE) {
            int checkError = team_addCard(hand->players[i], deck->cards[j]);
            if (checkError != NO_ERROR)
                return checkError;
            deck->cards[j] = NULL;
            distributedCards++;
        }
    }

    if (distributedCards == 0 && j == DECK_SIZE + 1)
        return DECK_EMPTY;
    if (distributedCards == 0 && i == MAX_GAME_PLAYERS + 1)
        return HAND_EMPTY;
    if (distributedCards == 1 && i == MAX_GAME_PLAYERS + 1)
        return LESS_PLAYERS;
    if (distributedCards == 1 && j == DECK_SIZE + 1)
        return LESS_CARDS;

    return NO_ERROR;
}

int round_distributeDeck(struct Deck *deck, struct Hand *hand)
{
    if (deck == NULL)
        return DECK_NULL;
    if (hand == NULL)
        return HAND_NULL;

    int numberPlayers = 0;
    for (int i = 0; i < MAX_GAME_PLAYERS; i++)
        if (hand->players[i] != NULL)
            numberPlayers++;

    if (numberPlayers == 1)
        return LESS_PLAYERS;
    if (numberPlayers == 0)
        return HAND_EMPTY;

    for (int i = 0; i < MAX_HANDS && i < DECK_SIZE / numberPlayers; i++) {
        int distributeCard = round_distributeCard(deck, hand);
        if (distributeCard != NO_ERROR)
            return distributeCard;
    }

    return NO_ERROR;
}

