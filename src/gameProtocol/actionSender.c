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

/**
 * Send "$UPDATE_SCORE <N1> <N2> <S1> , <N3> <N4> <S2>" command to room using
 * irc_sendRoomMessage() when playing with teams.
 * Send "$UPDATE_SCORE <N1> <S1> , <N2> <S2> , <N3> <S3> , <N4> <S4>" command
 * to room using irc_sendRoomMessage() when palying without teams.
 *
 * N - represents the player name.
 * S - represents the team score.
 */
int actionSender_updateScore(struct Game *game)
{
    if (game == NULL)
        return NULL_PARAMETER;

    int commandSize = 15;
    for (int i = 0; i < MAX_GAME_TEAMS; i++) {
        if (game->teams[i] != NULL) {
            commandSize += 7;
            for (int j = 0; j < MAX_TEAM_PLAYERS; j++) {
                if (game->teams[i]->players[j] != NULL) {
                    commandSize += strlen(game->teams[i]->players[j]->name);
                }
            }
        }
    }

    char *command = malloc(commandSize);
    char score[4];

    sprintf(command, "$UPDATE_SCORE ");

    for (int i = 0; i < MAX_GAME_TEAMS; i++) {
        if (game->teams[i] != NULL) {
            for (int j = 0; j < MAX_TEAM_PLAYERS; j++) {
                if (game->teams[i]->players[j] != NULL) {
                    strcat(command, game->teams[i]->players[j]->name);
                    strcat(command, " ");
                }
            }
            sprintf(score, "%d", game->teams[i]->score);
            strcat(command, score);
            strcat(command, " , ");
        }
    }

    int sendMessageRet = irc_sendRoomMessage(command);

    free(command);

    return sendMessageRet;
}
