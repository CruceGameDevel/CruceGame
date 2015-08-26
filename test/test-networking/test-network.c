#include <cutter.h>
#include <unistd.h>
#include <fnctl.h>
#include "../../src/networking/network.h"

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
        int serverSockfd = socket(AF_INET, SOCK_STREAM, 0);
        cut_assert_true(serverSockfd >= 0, "Server socket opening failed");

        struct sockaddr_in serv_addr, cli_addr;
        bzero((char *)&serv_addr, sizeof(serv_addr));

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(8080);
        cut_assert_true(bind(sockfd, (struct sockaddr *)&serv_addr,
                        sizeof(serv_addr)) >= 0, "Server bind failed")

        listen(sockfd,5);
        socklen_t clilen = sizeof(cli_addr);

        int newsockfd = cut_assert_true(accept(sockfd,
                                               (struct sockaddr *)&cli_addr,
                                               &clilen) >= 0,
                                        "Server accept failed");

        write(newsockfd, "test", 5);

        char buffer[10];
        read(newsockfd, buffer, 10);
        cut_assert_equal_string("check", buffer, "First data transfer failed");

        exit(EXIT_SUCCESS);
    }

    network_connect("localhost", 8080);
    cut_assert_true(sockfd >= 0,
                    "Network connect failed; negative socket");

    char buffer[10];
    read(newsockfd, buffer, 10);
    cut_assert_equal_string("test", buffer, "Second data transfer failed");

    write(newsockfd, "check", 6);

    read(sockfd, buffer, 10);

    cut_assert_not_equal_int(0, network_connect("localhost", 8080),
                             "Reconnection attempt succedeed without "
                             "previous disconnect");
}

void test_network_disconnect() {
    cut_assert_equal_int(0, 0);
}

void test_network_read() {
    cut_assert_equal_int(0, 0);
}

void test_network_send() {
    cut_assert_equal_int(0, 0);
}

