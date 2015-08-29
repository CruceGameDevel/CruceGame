#include <irc.h>
#include <network.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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
