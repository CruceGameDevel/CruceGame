#include <irc.h>
#include <network.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int current_room = -1;

int irc_connect(char *name)
{
    int connect_ret = network_connect(IRC_SERVER, IRC_PORT);
    if (connect_ret != NO_ERROR) {
        return connect_ret;
    }
    
    char *nick_command = malloc(COMMAND_SIZE + strlen(name));
    char *user_command = malloc(COMMAND_SIZE + strlen(name) * 2);
    char *join_command = malloc(COMMAND_SIZE + strlen(LOBBY_CHANNEL));

    sprintf(nick_command, "NICK %s\r\n", name);
    sprintf(user_command, "USER %s 8 * :%s\r\n", name, name);
    sprintf(join_command, "JOIN %s\r\n", LOBBY_CHANNEL);

    network_send("PASS *\r\n", 8);
    network_send(nick_command, strlen(nick_command));
    network_send(user_command, strlen(user_command));
    network_send(join_command, strlen(join_command));

    free(nick_command);
    free(user_command);
    free(join_command);

    return NO_ERROR;
}

int irc_disconnect()
{
    int send_ret = network_send("QUIT\r\n", 6);
    int disconnect_ret = network_disconnect();

    if (send_ret != NO_ERROR || disconnect_ret != NO_ERROR) {
        return UNINITIALIZED_CONNECTION;
    } else {
        return NO_ERROR;
    }
}

int irc_joinRoom(int room_number)
{
    char room_name[strlen(ROOM_FORMAT) + 3];
    sprintf(room_name, ROOM_FORMAT, room_number);

    char join_command[COMMAND_SIZE + strlen(room_name)];
    sprintf(join_command, "JOIN %s\r\n", room_name);

    int send_ret = network_send(join_command, strlen(join_command));
    if (send_ret != NO_ERROR) {
        return send_ret;
    }

    current_room = room_number;
    return NO_ERROR;
}

int irc_leaveRoom()
{
    char room_name[strlen(ROOM_FORMAT) + 3];
    if (current_room != -1) {
        sprintf(room_name, ROOM_FORMAT, current_room);

        char part_command[COMMAND_SIZE + strlen(room_name)];
        sprintf(part_command, "PART %s\r\n", room_name);

        int send_ret = network_send(part_command, strlen(part_command));
        if (send_ret != NO_ERROR) {
            return send_ret;
        }
        return NO_ERROR;
    } else {
        return -1;
    }
}

int irc_sendLobbyMessage(char *message)
{
    char *lobby_message_command = malloc(COMMAND_SIZE 
                                        + strlen(LOBBY_CHANNEL)
                                        + strlen(message));
    sprintf(lobby_message_command, 
            "PRIVMSG %s %s\r\n", 
            LOBBY_CHANNEL, message);
    int send_ret = network_send(lobby_message_command, strlen(lobby_message_command));
    
    free(lobby_message_command);
    if (send_ret != NO_ERROR) {
        return send_ret;
    } else {
        return NO_ERROR;
    }
}
