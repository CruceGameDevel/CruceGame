#include <cutter.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "../../src/irc/irc.h"

void serverHelper(size_t count, char **expected_messages)
{
    int server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    cut_assert_true(server_sock >= 0, "Failed to create the server socket");

    struct sockaddr_in test_server;

    memset(&test_server, 0, sizeof(test_server));
    test_server.sin_family = AF_INET;
    test_server.sin_addr.s_addr = INADDR_ANY;
    test_server.sin_port = htons(8080);

    cut_assert_true(bind(server_sock, (struct sockaddr *)&test_server, 
        sizeof(test_server)) >= 0, "Failed to bind the "
        "test server to socket");

    cut_assert_true(listen(server_sock, 1) >= 0, "Test server failed to lisen");

    struct sockaddr_in test_client;
    socklen_t client_length = sizeof(test_client);
    int client_sock = accept(server_sock, (struct sockaddr *)&test_client, 
            &client_length);

    cut_assert_true(client_sock >= 0, "Failed to accept connection");

    char **received_messages = malloc(count)
    cut_assert_true(recived_messages != NULL, "Malloc failed");
    for (int i = 0; i < count; ++i) {
        // every irc line has at most 513 chars
        received_messages[i] = malloc(513);
        memset(received_messages, 0, 513);
        cut_assert_true(received_messages[i] != NULL, "Inner malloc failed");
    }

    for (int i = 0; i < count; ++i) {
        cut_assert_true(read(client_sock, received_messages[i], 513) >= 0,
                "Failed to read message from client");
        cut_assert_equal_string(expected_messages[i], received_messages[i]);
    }

    for (int i = 0; i < count; ++i ) {
        free(received_messages[i])
    }
    free(received_messages);

    close(client_sock);
    close(server_sock);
}

void test_irc_connect()
{
    char **expected_messages = malloc(4);
    cut_assert_true(expected_messages != NULL, "Maloc Failed");
    for (int i = 0; i < 4; ++i) {
        expected_messages[i] = malloc(513);
        memset(expected_messages[i], 0, 513);
        cut_assert_true(expected_messages != NULL, "Inner malloc failed");
    }
        
    strcpy(expected_messages[0], "PASS *\r\n");
    strcpy(expected_messages[1], "NICK test_user\r\n");
    strcpy(expected_messages[2], "USER test_user 8 * :test_user\r\n");
    strcpy(expected_messages[3], "JOIN #cruce-devel\r\n");

    int pid = cut_fork();
    if (pid == 0) {
        serverHelper(4, expected_messages); // 4 messages are expected;
        close(EXIT_SUCCESS);
    }

    irc_connect();

    for (int i = 0; i < 4; ++i) {
        free(expected_messages[i]);
    }
    free(expected_messages);
}

