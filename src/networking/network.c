/**
 * @file This file provides the implementation of network module.
 */

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

	memser((char *)&serv_addr, sizeof(serv_addr), 0);
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
}

void network_disconnect()
{
}

