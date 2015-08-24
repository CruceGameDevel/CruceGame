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

int sockfd;

int network_connect(char *hostname, int port)
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd != 0) {
        return SOCKET_IN_USE;
    }

    struct hostent *server = gethostbyname(hostname);
    if(server == NULL) {
        return INVALID_HOSTNAME;
    } 

    struct sockaddr_in serv_addr;

    memset((char *)&serv_addr, sizeof(serv_addr), 0);
    serv_addr.sin_family = AF_INET;

    memmove((char *)server->h_addr_list[0], (char *)&serv_addr.sin_addr.s_addr, 
        server->h_length);

    serv_addr.sin_port = htons(port);
 	
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        return CONNECTING_ERROR;
    }

    return NO_ERROR;
}

int network_send(void *data, size_t size)
{
    if(sockfd == 0) {
        return UNINITIALIZED_SOCKET;
    }

    if(write(sockfd, data, size) <= 0) {
        return WRITING_ERROR;
    }

    return NO_ERROR;
}

int network_read(void *buffer, size_t size)
{
	if(sockfd == 0) {
        return UNINITIALIZED_SOCKET;
    }

    int bytes_read = read(sockfd, buffer, size);

    if(bytes_read < 1) {
        return READING_ERROR;
    }

    return bytes_read;
}

void network_disconnect()
{
    if(sockfd != 0) {
        close(sockfd);
    }
}

