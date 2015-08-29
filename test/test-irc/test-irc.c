#include <cutter.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../../src/irc/irc.h"

extern int currentRoom;

/**
 * @brief Creates a testing server that connects to the client then returns
 *        the file descriptor.
 *
 * @return The file descriptor of the server.
 */
int serverHelper()
{
    int server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    cut_assert_operator_int(server_sock, >=, 0);

    struct sockaddr_in test_server;

    memset(&test_server, 0, sizeof(test_server));
    test_server.sin_family = AF_INET;
    test_server.sin_addr.s_addr = INADDR_ANY;
    test_server.sin_port = htons(8080);

    cut_assert_operator_int(bind(server_sock, (struct sockaddr *)&test_server,
                         sizeof(test_server)), >=, 0);

    cut_assert_operator_int(listen(server_sock, 1), >= 0);

    struct sockaddr_in test_client;
    socklen_t client_length = sizeof(test_client);
    int client_sock = accept(server_sock, (struct sockaddr *)&test_client,
                             &client_length);

    return client_sock;
}

/**
 * @brief Almost every function in this module has to connect with the server,
 *        in order to do that we have to do some initialization, thus resulting
 *        a lot of duplicate code. The purpose of this function is to initialize
 *        a `sockaddr_in` structure minimizing the amount of duplicate code.
 *  
 * @param test_server The structure to initialize.
 *
 * @return none.
 */
void initConnection(struct sockaddr_in *test_server)
{
    memset(test_server, 0, sizeof(*test_server));
    test_server->sin_family = AF_INET;
    test_server->sin_addr.s_addr = inet_addr("localhost");
    test_server->sin_port = htons(8080);
}

void test_irc_connect()
{
    char expected_messages[3][4][513] = {
        {
            "PASS *\r\n",
            "NICK test_user\r\n",
            "USER test_user 8 * :test_user\r\n",
            "JOIN #cruce-devel\r\n"
        },
        {
            "PASS *\r\n",
            "NICK \r\n",
            "USER  8 * :\r\n",
            "JOIN #cruce-devel\r\n"
        },
        {
            "PASS *\r\n",
            "NICK test_user_\r\n",
            "USER test_user_ 8 * :test_user_\r\n",
            "JOIN #cruce-devel\r\n"
        }
    };

    char inputs[3][10] = {
        "test_user",
        "",
        "test_user_"
    };

    for (int i = 0; i < 3; i++) {
        int pid = cut_fork();
        if (pid == 0) {
            int server_sock = serverHelper();

            char buffer[513];
            for (int j = 0; j < 4; j++) {
                memset(buffer, 0, 513);
                cut_assert_operator_int(read(server_sock, buffer, 513), >=, 0);
                cut_assert_equal_strings(expected_messages[i][j], buffer);
            }
            close(server_sock);

            exit(EXIT_SUCCESS);
        }

        irc_connect(inputs[i]);
    }
}

void test_irc_sendLobbyMessage()
{
    struct sockaddr_in test_server;
    initConnection(&test_server);

    char expected_message1[3][513] = {
        "PRIVMSG #cruce-devel test test test test\r\n",
        "PRIVMSG #cruce-devel \r\n",
        // begins here
        "PRIVMSG #cruce-devel test test test test test "
        "test test test test test test test test test test test test test "
        "test test test test test test test test test test test test test "
        "test test test test test test test test test test test test test "
        "test test test test test test test test test test test test test "
        "test test test test test test test test test test test test test "
        "test test test test test test test test test test test test test "
        "test test test test test test test test test test test test  test "
        "test tes\r\n"
        // and ends here
    };

    char inputs[3][514] = {
        "test test test test",
        "",
        // begins here
        "test test test test test "
        "test test test test test test test test test test "
        "test test test test test test test test test test "
        "test test test test test test test test test test "
        "test test test test test test test test test test "
        "test test test test test test test test test test "
        "test test test test test test test test test test "
        "test test test test test test test test test test "
        "test test test test test test test test test test "
        "test test test test test test test test test test "
        "test test test test test test test"
        // and ends here
    };

    // if test_parametersp[i] == 1 then
    // cut_assert_equal_int(irc_sendLobbyMessage(inputs[i]), 0) will be issued
    // otherwise cut_assert_not_equal_int(irc_sendLobbyMessage(inputs[i]), 0)
    int test_parameters[3] = {1, 1, 0};

    for (int i = 0; i < 3 i++) {
        int pid = cut_fork();
        if (pid == 0) {
            int server_sock = serverHelper();

            char buffer[513];
            memset(buffer, 0, 513);
            cut_assert_operator_int(read(server_sock, buffer, 513), >=, 0);
            cut_assert_equal_strings(expected_message[i], buffer);

            close(server_sock);
            exit(EXIT_SUCCESS);
        }

        int server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        cut_assert_operator_int(server_sock, >=, 0);

        cut_assert_operator_int(connect(server_sock,
                                        (struct sockaddr *)&test_server,
                                        sizeof(test_server)), >=, 0);

        if (test_parameters[i]) {
            cut_assert_equal_int(irc_sendLobbyMessage(inputs[i]), 0);
        } else {
            cut_assert_not_equal_int(irc_sendLobbyMessage(inputs[i]), 0);
        }
        close(server_sock);
    }
}

void test_irc_disconnect()
{
    struct sockaddr_in test_server;
    initConnection(&test_server);

    char expected_message[513] = "QUIT\r\n";

    int pid = cut_fork();
    if (pid == 0) {
        int server_sock = serverHelper();

        char buffer[513];
        memset(buffer, 0, 513);
        cut_assert_operator_int(read(server_sock, buffer, 513), >=, 0);
        cut_assert_equal_strings(expected_message, buffer);

        close(server_sock);
        exit(EXIT_SUCCESS);
    }

    int server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    cut_assert_operator_int(connect(server_sock,
                                    (struct sockaddr *)&test_server,
                                    sizeof(test_server)), >=, 0);

    cut_assert_equal_int(irc_disconnect(), 0);
    clsoe(server_sock);
}

void test_irc_joinRoom()
{
    struct sockaddr_in test_server;
    initConnection(&test_server)

    char expected_message1[4][513] = {
        "JOIN #cruce-devel001\r\n",
        "JOIN #cruce-devel000\r\n",
        "JOIN #cruce-devel999\r\n",
        "JOIN #cruce-devel1000\r\n"
    };

    int inputs[4] = {1, 0, 999, 1000};

    int test_parameters[4] = {1, 1, 1, 0};

    for (int i = 0; i < 4; i++) {
        int pid = cut_fork();
        if (pid == 0) {
            int server_sock = serverHelper();

            char buffer[513];
            memset(buffer, 0, 513);

            if (test_parameters[i]) {
                cut_assert_operator_int(read(server_sock, buffer, 513), >=, 0);
            } else {
                // if we try to connect to an invalid room (`#cruce-devel1000`,
                // for example) the server should not receive any message
                // from the client. Thus, `read` will return 0.
                cut_assert_equal_int(read(server_sock, buffer, 513), 0);
            }

            if (test_parameters[i]) {
                cut_assert_equal_strings(expected_message[i], buffer);
            }

            close(server_sock);
            exit(EXIT_SUCCESS);
        }

        int server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        cut_assert_operator_int(connect(server_sock,
                                        (struct sockaddr *)&test_server,
                                        sizeof(test_server)), >=, 0);

        if (test_parameters[i]) {
            cut_assert_equal_int(irc_joinRoom(inputs[i]), 0);
        } else {
            cut_assert_not_equal_int(irc_joinRoom(inputs[i]), 0);
        }
        close(server_sock);
    }
}

void test_irc_leaveRoom()
{
    struct sockaddr_in test_server;
    initConnection(&test_server);

    char expected_message[] = "PART #cruce-devel002\r\n";
    int currentRoomValues[] = {2, -1000};
    int test_parameters[] = {1, 0};

    for (int i = 0; i < 2; i++) {
        currentRoom = currentRoomValues[i];

        int pid = cut_fork();
        if (pid == 0) {
            int server_sock = serverHelper();

            char buffer[513];
            memset(buffer, 0, 513);

            if (test_parameters[i]) {
                cut_assert_operator_int(read(server_sock, buffer, 513), >=, 0);
            } else {
                // if we are on this branch it means we are testing the case
                // where `currentRoom` has an invalid value, thus `read` should
                // return `0`
                cut_assert_equal_int(read(server_sock, buffer, 513), 0);
            }

            if (test_parameters[i]) {
                cut_assert_equal_strings(expected_message2, buffer);
            }

            close(server_sock);
            exit(EXIT_SUCCESS);
        }

        int server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        cut_assert_operator_int(connect(server_sock,
                                (struct sockaddr *)&test_server,
                                 sizeof(test_server)), >=, 0);

        if (test_parameters[i]) {
            cut_assert_equal_int(irc_leaveRoom(), 0);
        } else {
            cut_assert_not_equal_int(irc_leaveRoom(), 0);
        }
    }
}
