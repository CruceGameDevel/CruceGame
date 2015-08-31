#include <cutter.h>
#include <unistd.h>
#include <fcntl.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Open a localhost socket and waits a client to connect.
 */
int openLocalhostSocket(int port) {
    int serverSockfd = socket(AF_INET, SOCK_STREAM, 0);
    int optval = 1;
    setsockopt(serverSockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
    cut_assert_true(serverSockfd >= 0, "Server socket opening failed");

    struct sockaddr_in serv_addr, cli_addr;
    bzero((char *)&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    int bnd = bind(serverSockfd, (struct sockaddr *)&serv_addr,
                   sizeof(serv_addr));
    cut_assert_operator_int(bnd, >=, 0, "Server bind failed");

    listen(serverSockfd, 5);
    socklen_t clilen = sizeof(cli_addr);

    int newsockfd = accept(serverSockfd, (struct sockaddr *)&cli_addr, &clilen);

    close(serverSockfd);

    cut_assert_operator_int(newsockfd, >=, 0, "Server accept failed");

    return newsockfd;
}
