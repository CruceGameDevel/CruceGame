#include <actionSender.h>
#include <irc.h>
#include <network.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * Send "$REQUEST_BID <user>" command to room using irc_sendRoomMessage().
 */
int actionSender_requestBid(char *user)
{
    if (user == NULL)
        return NULL_PARAMETER;

    char *command = malloc(14 + strlen(user));
    sprintf(command, "$REQUEST_BID %s", user);

    int sendMessageRet = irc_sendRoomMessage(command);

    free(command);

    return sendMessageRet;
}

/**
 * Send "$BID <value>" command to room using irc_sendRoomMessage().
 */
int actionSender_sendBid(int value)
{
    char *command = malloc(7);
    sprintf(command, "$BID %c", value + 48); // value belongs [0,6]

    int sendMessageRet = irc_sendRoomMessage(command);

    free(command);

    return sendMessageRet;
}

/**
 * Send "$REQUEST_CARD <user>" command to room using irc_sendRoomMessage().
 */
int actionSender_requestCard(char *user)
{
    if (user == NULL)
        return NULL_PARAMETER;

    char *command = malloc(15 + strlen(user));
    sprintf(command, "$REQUEST_CARD %s", user);

    int sendMessageRet = irc_sendRoomMessage(command);

    free(command);

    return sendMessageRet;
}

/**
 * Send "$CARD <value> <suit>" command to room using irc_sendRoomMessage().
 */
int actionSender_sendCard(struct Card *card)
{
    if (card == NULL)
        return NULL_PARAMETER;

    char *command = malloc(12);
    sprintf(command, "$CARD %d %d", card->value, card->suit);

    int sendMessageRet = irc_sendRoomMessage(command);

    free(command);

    return sendMessageRet;
}

/**
 * Send "$DISTRIBUTE_CARD <recipient> <value> <suit>" command to room using
 * irc_sendRoomMessage().
 */
int actionSender_distributeCard(char *recipient, struct Card *card)
{
    if (recipient == NULL || card == NULL)
        return NULL_PARAMETER;

    char *command = malloc(24 + strlen(recipient));
    sprintf(command, "$DISTRIBUTE_CARD %s %d %d", recipient, card->value,
                                                  card->suit);

    int sendMessageRet = irc_sendRoomMessage(command);

    free(command);

    return sendMessageRet;
}
