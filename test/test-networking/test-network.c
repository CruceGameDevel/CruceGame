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
#include <errno.h>
#include "../../src/networking/network.h"

extern int sockfd;

/**
 * Helper to open a local server socket.
 * Returns a sockfd.
 */
int openLocalhostSocket(int port) {
    int serverSockfd = socket(AF_INET, SOCK_STREAM, 0);
    cut_assert_true(serverSockfd >= 0, "Server socket opening failed");

    struct sockaddr_in serv_addr, cli_addr;
    bzero((char *)&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    cut_assert_true(bind(serverSockfd, (struct sockaddr *)&serv_addr,
                    sizeof(serv_addr)) >= 0, "Server bind failed");

    listen(serverSockfd, 5);
    socklen_t clilen = sizeof(cli_addr);

    int newsockfd = accept(serverSockfd, (struct sockaddr *)&cli_addr, &clilen);

    cut_assert_true(newsockfd >= 0, "Server accept failed");

    return newsockfd;
}

/**
 * Test for network_connect.
 * It works by testing some exceptional cases, then by creating a new process
 * that opens a socket and connecting to it. Some data is transfered back and
 * forth between the 2 processes using the opened socket.
 *
 * This function assumes the use of sockfd private variable in the networking
 * module.
 */
void test_network_connect() {
    cut_assert_not_equal_int(0, network_connect("", 8080),
                             "Connection to empty hostname succeeded");
    cut_assert_not_equal_int(0, network_connect("abcd", 8080),
                             "Connection to non-existent host succeeded");
    cut_assert_not_equal_int(0, network_connect("localhost", -1),
                             "Connection to port -1 succeeded");
    cut_assert_not_equal_int(0, network_connect("localhost", 70000),
                             "Connection to port 70000 succeeded");

    int pid = cut_fork();
    if (pid == 0) {
        int newsockfd = openLocalhostSocket(8080);

        write(newsockfd, "test", 5);

        char buffer[10];
        read(newsockfd, buffer, 10);
        cut_assert_equal_string("check", buffer, "First data transfer failed");

        exit(EXIT_SUCCESS);
    }

    sleep(1);
    network_connect("localhost", 8080);
    cut_assert_true(sockfd >= 0,
                    "Network connect failed; negative socket");

    char buffer[10] = {'\0'};
    read(sockfd, buffer, 10);
    cut_assert_equal_string("test", buffer, "Second data transfer failed");

    write(sockfd, "check", 6);

    read(sockfd, buffer, 10);

    cut_assert_not_equal_int(0, network_connect("localhost", 8080),
                             "Reconnection attempt succedeed without "
                             "previous disconnect");
}

/**
 * Check id a file descriptor is valid (i.e. the resource is still open).
 * It is used to test the disconnect function.
 */
int fdIsValid(int fd)
{
    return fcntl(fd, F_GETFD) != -1 || errno != EBADF;
}

/**
 * Test for network_disconnect.
 * It works by opening a socket in another process, opening it and calling
 * network_disconnect on it.
 *
 * This function assumes the use of sockfd private variable in the networking
 * module.
 */
void test_network_disconnect() {

    int pid = cut_fork();
    if (pid == 0) {
        int newsockfd = openLocalhostSocket(8079);
        exit(EXIT_SUCCESS);
    }

    sleep(1);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    cut_assert_true(sockfd >= 0, "Could not connect to the server thread");

    struct hostent *server = gethostbyname("localhost");
    cut_assert_true(server != NULL, "No such host");

    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr_list[0],
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(8079);

    cut_assert_false(connect(sockfd,
                             (struct sockaddr *)&serv_addr,
                             sizeof(serv_addr)) < 0,
                     "Error connecting to the server process");

    network_disconnect();

    cut_assert_false(fdIsValid(sockfd),
                     "Network disconnect failed to disable socket "
                     "file descriptor");

}

void test_network_read() {
    char buffer[10] = {'\0'};

    cut_assert_operator_int(0, >, network_read(buffer, 10));

    int pid = cut_fork();
    if (pid == 0) {
        int newsockfd = openLocalhostSocket(8078);
        write(newsockfd, "test", 5);

        exit(EXIT_SUCCESS);
    }

    sleep(1);
    network_connect("localhost", 8078);

    cut_assert_equal_int(5, network_read(buffer, 10),
                         "Not have been read all bytes");
    cut_assert_equal_string("test", buffer, "Data transfer failed");

    network_disconnect();

    cut_assert_operator_int(0, >, network_read(buffer, 10));
}

void test_network_send() {
    cut_assert_not_equal_int(0, network_send("test", 5),
                             "Send data to non-existent server succeeded");

    int pid = cut_fork();
    if (pid == 0) {
        int newsockfd = openLocalhostSocket(8077);
        char buffer[10] = {'\0'};
        read(newsockfd, buffer, 10);

        cut_assert_equal_string("test", buffer, "Data transfer failed");

        exit(EXIT_SUCCESS);
    }

    sleep(1);
    network_connect("localhost", 8077);

    cut_assert_equal_int(NO_ERROR, network_send("test", 5), "Send data failed");

    network_disconnect();

    cut_assert_not_equal_int(0, network_send("test", 5),
                             "Send data to non-existent server succeeded");
}

