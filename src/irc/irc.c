#include <irc.h>
#include <network.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int currentRoom = -1;

int irc_connect(char *name)
{
    int connectRet = network_connect(IRC_SERVER, IRC_PORT);
    if (connectRet != NO_ERROR) {
        return connectRet;
    }
    
    char *nickCommand = malloc(COMMAND_SIZE + strlen(name));
    char *userCommand = malloc(COMMAND_SIZE + strlen(name) * 2);
    char *joinCommand = malloc(COMMAND_SIZE + strlen(LOBBY_CHANNEL));

    sprintf(nickCommand, "NICK %s\r\n", name);
    sprintf(userCommand, "USER %s 8 * :%s\r\n", name, name);
    sprintf(joinCommand, "JOIN %s\r\n", LOBBY_CHANNEL);

    network_send("PASS *\r\n", 8);
    network_send(nickCommand, strlen(nickCommand));
    network_send(userCommand, strlen(userCommand));
    network_send(joinCommand, strlen(joinCommand));

    free(nickCommand);
    free(userCommand);
    free(joinCommand);

    return NO_ERROR;
}

int irc_disconnect()
{
    int sendRet = network_send("QUIT\r\n", 6);
    int disconnectRet = network_disconnect();

    if (sendRet != NO_ERROR || disconnectRet != NO_ERROR) {
        return UNINITIALIZED_CONNECTION;
    } else {
        return NO_ERROR;
    }
}

int irc_joinRoom(int roomNumber)
{
    char roomName[strlen(ROOM_FORMAT) + 3];
    sprintf(roomName, ROOM_FORMAT, roomNumber);

    char joinCommand[COMMAND_SIZE + strlen(roomName)];
    sprintf(joinCommand, "JOIN %s\r\n", roomName);

    int sendRet = network_send(joinCommand, strlen(joinCommand));
    if (sendRet != NO_ERROR) {
        return sendRet;
    }

    currentRoom = roomNumber;
    return NO_ERROR;
}

int irc_leaveRoom()
{
    char roomName[strlen(ROOM_FORMAT) + 3];
    if (currentRoom != -1) {
        sprintf(roomName, ROOM_FORMAT, currentRoom);

        char partCommand[COMMAND_SIZE + strlen(roomName)];
        sprintf(partCommand, "PART %s\r\n", roomName);

        int sendRet = network_send(partCommand, strlen(partCommand));
        if (sendRet != NO_ERROR) {
            return sendRet;
        }
        return NO_ERROR;
    } else {
        return -1;
    }
}

int irc_sendLobbyMessage(char *message)
{
    char *lobbyMessageCommand = malloc(COMMAND_SIZE 
                                        + strlen(LOBBY_CHANNEL)
                                        + strlen(message));
    sprintf(lobbyMessageCommand, 
            "PRIVMSG %s %s\r\n", 
            LOBBY_CHANNEL, message);
    int sendRet = network_send(lobbyMessageCommand, strlen(lobbyMessageCommand));
    
    free(lobbyMessageCommand);
    if (sendRet != NO_ERROR) {
        return sendRet;
    } else {
        return NO_ERROR;
    }
}
