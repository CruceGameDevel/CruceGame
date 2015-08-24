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

int sockfd;

int network_connect(char *hostname, int port)
{
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0) {
		return -1;
	}

	struct hostent *server = gethostbyname(hostname);
	if(server == NULL) {
		return -2;
	} 

	struct sockaddr_in serv_addr;

	memset((char *)&serv_addr, sizeof(serv_addr), 0);
	serv_addr.sin_family = AF_INET;

	memmove((char *)server->h_addr_list[0], (char *)&serv_addr.sin_addr.s_addr, 
		server->h_length);

	serv_addr.sin_port = htons(port);
 	
 	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
 		return -3;
    }

    return 0;
}

int network_send(void *data, int size)
{
}

int network_read(void *buffer, int size)
{
	if(sockfd == 0) {
		return -4;
	}

	if(size <= 0) {
		return -5;
	}

	int bytes_read = read(sockfd, buffer, size);

	if(bytes_read < 1) {
		return -6;
	}

	return bytes_read;
}

void network_disconnect()
{
	if(sockfd != 0) {
		close(sockfd);
	}
}

