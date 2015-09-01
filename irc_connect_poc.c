#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include <errno.h>
#define CONNECTION_IN_USE -1
#define INVALID_HOSTNAME -2
#define CONNECTING_ERROR -3
#define UNINITIALIZED_CONNECTION -4
#define WRITING_ERROR -5
#define PARAMETER_OUT_OF_RANGE -8
#define NULL_PARAMETER -7
#define NO_ERROR 0
#define IRC_PORT 6667
#define IRC_SERVER "irc.freenode.net"
#define COMMAND_SIZE 30
#define LOBBY_CHANNEL "#cruce-devel"

int sockfd = -1;

int network_connect(char *hostname, int port)
{
    if (sockfd > 0)
        return CONNECTION_IN_USE;

    if (hostname == NULL)
        return NULL_PARAMETER;

    if (port < 0 || port > 65535)
        return PARAMETER_OUT_OF_RANGE;

    struct hostent *server = gethostbyname(hostname);
    if (server == NULL)
        return INVALID_HOSTNAME;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        return CONNECTING_ERROR;

    struct sockaddr_in serv_addr;

    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    memmove( (char *)&serv_addr.sin_addr.s_addr,
(char *)server->h_addr_list[0],
            server->h_length);

    serv_addr.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("HERE %d", errno);
        close(sockfd);
        sockfd = -1;
        return CONNECTING_ERROR;
    }

    return NO_ERROR;
}

int network_send(void *data, size_t size)
{
    if (data == NULL)
        return NULL_PARAMETER;

    if (sockfd < 0)
        return UNINITIALIZED_CONNECTION;

    if (size == 0)
        return PARAMETER_OUT_OF_RANGE;

    if (write(sockfd, data, size) <= 0)
        return WRITING_ERROR;

    return NO_ERROR;
}
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

int main() {
    printf("RET %d\n", irc_connect("test"));
    sleep(10);
    return 0;
}
