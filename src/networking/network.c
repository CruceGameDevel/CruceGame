/**
 * @file This file provides the implementation of network module.
 */

#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "network.h"

int sockfd = -1;

int network_connect(char *hostname, int port)
{
    if (sockfd > 0)
        return CONNECTION_IN_USE;

    if (hostname == NULL)
        return NULL_PARAMETER;

    struct hostent *server = gethostbyname(hostname);
    if (server == NULL)
        return INVALID_HOSTNAME;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        return CONNECTING_ERROR;

    struct sockaddr_in serv_addr;

    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    memmove((char *)server->h_addr_list[0], (char *)&serv_addr.sin_addr.s_addr,
            server->h_length);

    serv_addr.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
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

    if (write(sockfd, data, size) <= 0)
        return WRITING_ERROR;

    return NO_ERROR;
}

int network_read(void *buffer, size_t size)
{
    if (buffer == NULL)
        return NULL_PARAMETER;

    if (sockfd < 0)
        return UNINITIALIZED_CONNECTION;

    int bytes_read = read(sockfd, buffer, size);

    if (bytes_read < 1)
        return READING_ERROR;

    return bytes_read;
}

int network_disconnect()
{
    if (sockfd < 0)
        return UNINITIALIZED_CONNECTION;
    else {
        close(sockfd);
        sockfd = -1;
        return NO_ERROR;
    }
}

