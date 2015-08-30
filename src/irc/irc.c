#include <irc.h>
#include <network.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int currentRoom = -1;

/**
 * Use network_connect to open a connection to IRC server.
 *
 * After, sends "PASS *" command (just a convention, because 
 * lobby doesn't have a password), then sends "NICK <name>" 
 * and "USER <name> 8 * :<name>" commands to set the player name and nick,
 * where <name> is the name of the player and "8 *" is the user mode. 
 *
 * At last, it sends "JOIN <channel>" command to join the lobby, 
 * where <channel> is the lobby channel name.
 *
 * ALL COMMANDS MUST BE TERMINATED IN "\r\n".
 */
int irc_connect(char *name)
{
    // Connect to IRC server and test for errors.
    int connectRet = network_connect(IRC_SERVER, IRC_PORT);
    if (connectRet != NO_ERROR) {
        return connectRet;
    }

    // Allocate memory for our variables.
    char *nickCommand = malloc(COMMAND_SIZE + strlen(name));
    char *userCommand = malloc(COMMAND_SIZE + strlen(name) * 2);
    char *joinCommand = malloc(COMMAND_SIZE + strlen(LOBBY_CHANNEL));

    // Prepare commands.
    sprintf(nickCommand, "NICK %s\r\n", name);
    sprintf(userCommand, "USER %s 8 * :%s\r\n", name, name);
    sprintf(joinCommand, "JOIN %s\r\n", LOBBY_CHANNEL);

    // Send commands to the server.
    network_send("PASS *\r\n", 8);
    network_send(nickCommand, strlen(nickCommand));
    network_send(userCommand, strlen(userCommand));
    network_send(joinCommand, strlen(joinCommand));

    // Free our variables.
    free(nickCommand);
    free(userCommand);
    free(joinCommand);

    return NO_ERROR;
}

/**
 * It simply sends the "QUIT" command and use network_disconnect
 * to close the connection.
 */
int irc_disconnect()
{
    // Send QUIT command and disconnect from server.
    int sendRet = network_send("QUIT\r\n", 6);
    int disconnectRet = network_disconnect();

    // Test for errors.
    if (sendRet != NO_ERROR || disconnectRet != NO_ERROR) {
        return UNINITIALIZED_CONNECTION;
    } else {
        return NO_ERROR;
    }
}

/**
 * Takes room number as argument and generates room name using the
 * following format: "#cruce-gameXXX" where XXX is the number of room.
 * After, it sends "JOIN #cruce-gameXXX" command to server to join the room.
 */
int irc_joinRoom(int roomNumber)
{
    // Prepare room name.
    char roomName[strlen(ROOM_FORMAT) + 3];
    sprintf(roomName, ROOM_FORMAT, roomNumber);

    // Prepare join command.
    char joinCommand[COMMAND_SIZE + strlen(roomName)];
    sprintf(joinCommand, "JOIN %s\r\n", roomName);

    // Send join command and test if there is any error.
    int sendRet = network_send(joinCommand, strlen(joinCommand));
    if (sendRet != NO_ERROR) {
        return sendRet;
    }

    // Set current room to room we joined a moment ago.
    currentRoom = roomNumber;

    return NO_ERROR;
}

/**
 * First test if player has joined in any room. If not, return -1, else
 * generate room name using current room number and send "PART #cruce-gameXXX"
 * command to leave the room.
 */
int irc_leaveRoom()
{
    char roomName[strlen(ROOM_FORMAT) + 3];
    // Test if player is in any room.
    if (currentRoom != -1) {
        // Prepare room name.
        sprintf(roomName, ROOM_FORMAT, currentRoom);

        // Prepare leave command.
        char partCommand[COMMAND_SIZE + strlen(roomName)];
        sprintf(partCommand, "PART %s\r\n", roomName);

        // Send leave command and test for errors.
        int sendRet = network_send(partCommand, strlen(partCommand));
        if (sendRet != NO_ERROR) {
            return sendRet;
        }
        
        // Reset current room to default value.
        currentRoom = -1;
        
        return NO_ERROR;
    } else {
        return -1;
    }
}

/**
 * Send "PRIVMSG <channel> <message>" command for sending 
 * a message to lobby, where <channel> is name of the lobby
 * channel and <message> is message to send.
 */
int irc_sendLobbyMessage(char *message)
{
    // Allocate memory for message command and prepare it.
    char *lobbyMessageCommand = malloc(COMMAND_SIZE
                                        + strlen(LOBBY_CHANNEL)
                                        + strlen(message));
    sprintf(lobbyMessageCommand,
            "PRIVMSG %s %s\r\n",
            LOBBY_CHANNEL, message);

    // Send message command.
    int sendRet = network_send(lobbyMessageCommand, strlen(lobbyMessageCommand));

    // Free memory and test for errors.
    free(lobbyMessageCommand);
    if (sendRet != NO_ERROR) {
        return sendRet;
    } else {
        return NO_ERROR;
    }
}
