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
