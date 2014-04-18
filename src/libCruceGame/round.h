/**
 * @file round.h
 * @brief Round and Hand structures, as well as helper functions.
 */

#ifndef ROUND_H
#define ROUND_H

#include "platform.h"
#include "deck.h"
#include "team.h"
#include "constants.h"
#include "errors.h"

/**
 * @struct Hand
 * @brief Hand structure.
 *
 * In a hand, player[i] gives cards[i] and bids bid[i].
 * The players should be added in the order of the bids.
 *
 * @var Hand::cards
 *     Pointer to the cards of the hand.
 * @var Hand::players
 *     Pointer to the players of the hand.
 */
struct Hand{
    struct Card *cards[MAX_GAME_PLAYERS];
    struct Player *players[MAX_GAME_PLAYERS];
};

/**
 * @struct Round
 * @brief Round structure.
 *
 * Round is a division of the game, it keeps the winning 
 * hands and computes the score until the winner of the
 * round is found.
 *
 * @var Round::id
 *     Identifier of the round.
 * @var Round::trump
 *     The trump of the round.
 * @var Round::hands
 *     Pointer to the hands of the round.
 * @var Round::bids
 *     The bids of the players.
 * @var Round::players
 *     Pointer to the players of the round.
 * @var Round::pointsNumber
 *     The total amount of points of the round.
 */
struct Round{
    int id;
    enum Suit trump;
    struct Hand *hands[MAX_HANDS];
    int bids[MAX_GAME_PLAYERS];
    struct Player *players[MAX_GAME_PLAYERS];
    int pointsNumber[MAX_GAME_PLAYERS];
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Finds the winner of a bid in a round.
 *
 * @param round Pointer to the round where to find the bid winner.
 *
 * @return Pointer to the bid winner player on success or NULL on failure.
 */
EXPORT struct Player *round_getBidWinner(const struct Round *round);

/**
 * @brief Places the bid of a player.
 *
 * @param player Pointer to the player who places the bid.
 * @param bid The value of the bid.
 * @param round Pointer to the round where to place the bid.
 *
 * @return \ref NO_ERROR on success, other value on failure.
 */
EXPORT int round_placeBid(const struct Player *player, const int bid, 
                          struct Round *round);

/**
 * @brief Add a player to a round.
 *
 * @param player Pointer to the player to be added.
 * @param round Pointer to the round where to add player.
 * 
 * @return \ref NO_ERROR on success, other value on failure.
 */
EXPORT int round_addPlayer(struct Player* player, struct Round *round);

/**
 * @brief Helper to find player in a round.
 *
 * @param player Player to find.
 * @param round Round to search for player.
 *
 * @return Id of the player if found, negative value otherwise.
 */
EXPORT int round_findPlayerIndexRound(const struct Player *player, 
                                      const struct Round *round);

/**
 * @brief Adds a player to a hand.
 *
 * @param player Pointer to the player to be added.
 * @param hand Pointer to the hand where the player is to be added.
 *
 * @return \ref NO_ERROR on success, other value on failure.
 */
EXPORT int round_addPlayerHand(struct Player *player, struct Hand *hand);

/**
 * @brief Places a card from a player to a hand and offers the player 20 points
 *        if has 3 and 4 of same suit or 40 points if has 3 and 4 of same suit
 *        as the trump.
 *
 * @param player Pointer to the player who places the card.
 * @param cardId Id of the card placed by the player (id from Player.cards).
 * @param handId Id of the hand in which the card is placed.
 * @param round  Pointer to the round in which is hand.
 *
 * @return \ref NO_ERROR on success, other value on failure.
 */
EXPORT int round_putCard(struct Player *player, const int cardId,
                         const int handId, struct Round *round);

/**
 * @brief Allocates memory for and initializes a round.
 *
 * @return Pointer to the new round on success or NULL on failure.
 */
EXPORT struct Round *round_createRound();

/**
 * @brief Frees the memory of a round. Makes NULL pointer.
 *
 * @param round Pointer to pointer to the round to be deleted.
 *
 * @return \ref NO_ERROR on success, other value on failure.
 */
EXPORT int round_deleteRound(struct Round **round);

/**
 * @brief Allocates memory for and initializes a hand.
 *
 * @return Pointer to the new hand on success or NULL on failure.
 */
EXPORT struct Hand *round_createHand();

/**
 * @brief Frees the memory of a hand. Makes pointer NULL.
 *
 * @param hand Pointer to pointer to the hand to be deleted.
 *
 * @return \ref NO_ERROR on success, other value on failure.
 */
EXPORT int round_deleteHand(struct Hand **hand);

/**
 * @brief Removes a player from a round.
 *
 * @param player Pointer to the player to be removed.
 * @param round Pointer to the round from which the player is removed.
 *
 * @return \ref NO_ERROR on success, other value on failure.
 */
EXPORT int round_removePlayer(const struct Player *player, struct Round *round);

/**
 * @brief Removes a player from a hand.
 * 
 * @param player Pointer to the player to be removed.
 * @param hand Pointer to the hand from where the player is removed.
 *
 * @return \ref NO_ERROR on success, other value on failure.
 */
EXPORT int round_removePlayerHand(const struct Player *player, 
                                  struct Hand *hand);

/**
 * @brief Determines the winner of a hand.
 *
 * @param hand Pointer to the hand.
 * @param trump The trump of round.
 * @param round Pointer to the round containing the hand that the player won.
 *
 * @return Pointer to the winning player or NULL on failure.
 */
EXPORT struct Player *round_handWinner(const struct Hand *hand, 
                                        const enum Suit trump, 
                                        struct Round *round);

/**
 * @brief Distributes one card to every player.
 *
 * @param deck Pointer to the deck from where cards are distributed.
 * @param round Pointer to the round containing the players that receive the cards.
 *
 * @return \ref NO_ERROR on success, other value on failure.
 */
EXPORT int round_distributeCard(struct Deck *deck, const struct Round *round);

/**
 * @brief Distributes cards to players.
 *
 * @param deck Pointer to the deck from where cards are distributed.
 * @param round Pointer to the round that deck is distributed to.
 *
 * @return \ref NO_ERROR on success, other value on failure.
 */
EXPORT int round_distributeDeck(struct Deck *deck, const struct Round *round);

/**
 * @brief The function arranges the players in a hand.
 *
 * @param round Pointer to the round from which arranges it the players.
 * @param i The position from where begin arranging.
 *
 * @return \ref NO_ERROR or 0 on success, other value on failure.
 */
EXPORT int round_arrangePlayersHand(struct Round *round, int i);

#ifdef __cplusplus
}
#endif

#endif

