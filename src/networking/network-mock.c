/**
 * @file This file provides a mock implementation of the network module, used
 *       to test other modules.
 */

#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errors.h>
#include <stdio.h>
#include "network-mock.h"

#define BUFFER_SIZE 1024
#define min(a,b) (a < b)?(a):(b)

int sockfd = -1;

int network_connect(char *hostname, int port)
{
    network_send(hostname, strlen(hostname));
    char port_string[10];
    sprintf(port_string, " %d\r\n", port);
    network_send(port_string, strlen(port_string));
    return NO_ERROR;
}

void network_setSockfd(int fileDescriptor)
{
    sockfd = fileDescriptor;
}

int network_getSockfd()
{
    return sockfd;
}

int network_send(void *data, size_t size)
{
    if (data == NULL)
        return POINTER_NULL;

    if (sockfd < 0)
        return UNINITIALIZED_CONNECTION;

    if (size == 0)
        return PARAMETER_OUT_OF_RANGE;

    if (write(sockfd, data, size) <= 0)
        return WRITING_ERROR;

    return NO_ERROR;
}

int network_read(void *buffer, size_t size)
{
    return read(sockfd, buffer, size);
}

int network_disconnect()
{
    if (sockfd == -1) {
        return UNINITIALIZED_CONNECTION;
    } else {
        sockfd = -1;
        return NO_ERROR;
    }
}

