#include <cutter.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "../../src/irc/irc.h"

void serverHelper()
{
    int server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    cut_assert_true(server_sock >= 0, "Failed to create the server socket");

    struct sockaddr_in test_server;

    memset(&test_server, 0, sizeof(test_server));
    test_server.sin_family = AF_INET;
    test_server.sin_addr.s_addr = INADDR_ANY;
    test_server.sin_port = htons(8080);

    cut_assert_true(bind(server_sock, (struct sockaddr *)&test_server, 
                sizeof(test_server)) >= 0, "Failed to bind the test server to socket");

    cut_assert_true(listen(server_sock, 1) >= 0, "Test server failed to lisen");

    struct sockaddr_in test_client;
    socklen_t client_length = sizeof(test_client);
    int client_sock = accept(server_sock, (struct sockaddr *)&test_client, 
            &client_length);

    cut_assert_true(client_sock >= 0, "Failed to accept connection");

    char expected_messages[4][513] = {
        "PASS *\r\n",
        "NICK test_user\r\n",
        "USER test_user 8 * :test_user\r\n",
        "JOIN #cruce-devel\r\n"
    };

    char received_messages[4][513]; // Every irc line has 512 chars + '\0'

    for (int i = 0; i < 4; ++i)
    {
        memset(received_messages, 0, 513);
        cut_assert_true(read(client_sock, received_messages[i], 513) >= 0,
                "Failed to read message from client");
        cut_assert_equal_string(expected_messages[i], received_messages[i]);
    }

    close(client_sock);
    close(server_sock);
}

void test_irc_connect()
{
    int pid = cut_fork();
    if (pid == 0) {
        serverHelper(); // 4 messages are expected;
        close(EXIT_SUCCESS);
    }

    irc_connect("test_user");
}

