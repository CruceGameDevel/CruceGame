/**
 * This file is part of the gameProtocol module, which represents the only way
 * to communicate with the external world. It provides methods for all game
 * events, such as bidding, putting a card on the table or talking to another
 * player.
 * 
 * The sender submodule broadcasts the actions of a player by writing game
 * commands and user messages to the irc channel.
 */

#include <cruceGame.h>

/**
 * @brief Function to send a REQUEST_BID game command.
 *
 * @param user The user to request the bid from.
 *
 * @return NO_ERROR on success, other value on failure.
 */
int actionSender_requestBid(char *user);


/**
 * @brief Function to send a bid in reply to a REQUEST_BID command. Be sure that
 *        bidValue is a valid bid.
 *
 * @param bidValue The value to bid.
 *
 * @return NO_ERROR on success, other value on failure.
 */
int actionSender_sendBid(int bidValue);

/**
 * @brief Function to request a card from a user.
 *
 * @param user The user to request the card from.
 *
 * @return NO_ERROR on success, other value on failure.
 */
int actionSender_requestCard(char *user);

/**
 * @brief Function to send a card in reply to a REQUEST_CARD command. Be sure
 *        that card is a valid card.
 *
 * @param card The card to send.
 *
 * @return NO_ERROR on success, other value on failure.
 */
int actionSender_sendCard(struct Card *card);

/**
 * @brief Function to distribute a card. Used by the dealer (host). Be sure that
 *        card is a valid card.
 *
 * @param recipient The name of the recipient of the card.
 * @param card The card to send.
 *
 * @return NO_ERROR on succes, other value on failure.
 */
int actionSender_distributeCard(char *recipient, struct Card *card);

/**
 * @brief Function to update the score. Used by the dealer (host).
 *
 * @param score???
 *
 * @return NO_ERROR on success, other value on failure.
 */
int actionSender_updateScore();

