#include <cutter.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../../src/irc/irc.h"

/**
 * @brief In order to test the irc module we have o intercept the messages sent 
 *        by the functions in this module. To accomplish this task we have 
 *        implemented this function which compares the messages sent by the
 *        tested function with the expected result.
 *
 * @param count Number of expected messages.
 * @param expected_messages An array of strings containing all the expected 
 *        messages in the order they will be compared. 
 *
 * @return none.
 */
int serverHelper(size_t count, char **expected_messages)
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

    cut_assert_true(listen(server_sock, 1) >= 0, "Test server failed "
                   "to listen");

    struct sockaddr_in test_client;
    socklen_t client_length = sizeof(test_client);
    int client_sock = accept(server_sock, (struct sockaddr *)&test_client,
                             &client_length);

    return client_sock;
}

void test_irc_connect()
{
    // test for user name: test_user 
    char expected_messages[4][513] = {
        "PASS *\r\n",
        "NICK test_user\r\n",
        "USER test_user 8 * :test_user\r\n",
        "JOIN #cruce-devel\r\n"
    };

    int pid = cut_fork();
    if (pid == 0) {
        serverHelper(4, expected_messages); // 4 messages are expected;
        close(EXIT_SUCCESS);
    }

    irc_connect("test_user");

    // test for user name: (empty user name)
    memset(expected_messages[0], 0, 513);
    strcpy(expected_messages[0], "PASS *\r\n");
    memset(expected_messages[1], 0, 513);
    strcpy(expected_messages[1], "NICK \r\n");
    memset(expected_messages[2], 0, 513);
    strcpy(expected_messages[2], "USER  8 * :\r\n");
    memset(expected_messages[3], 0, 513);
    strcpy(expected_messages[3], "JOIN #cruce-devel\r\n");

    pid = cut_fork();
    if (pid == 0) {
        serverHelper(4, expected_messages);
        exit(EXIT_SUCCESS);
    }

    irc_connect("");

    // test with a nick name bigger than 9 chars: test_user_ (10 chars)
    memset(expected_messages[0], 0, 513);
    strcpy(expected_messages[0], "PASS *\r\n");
    memset(expected_messages[1], 0, 513);
    strcpy(expected_messages[1], "NICK test_user_\r\n");
    memset(expected_messages[2], 0, 513);
    strcpy(expected_messages[2], "USER test_user_ 8 * :test_user_\r\n");
    memset(expected_messages[3], 0, 513);
    strcpy(expected_messages[3], "JOIN #cruce-devel\r\n");

    pid = cut_fork();
    if (pid == 0) {
        serverHelper(4, expected_messages);
        exit(EXIT_SUCCESS);
    }

    irc_connect("test_user_");
}

void test_irc_sendLobbyMessage()
{
    struct sockaddr_in test_server;

    memset(&test_server, 0, sizeof(test_server));
    test_server.sin_family = AF_INET;
    test_server.sin_addr.s_addr = inet_addr("localhost");
    test_server.sin_port = htons(8080);


    // test a message of average length: 39 chars
    char expected_messages[4][513] = {
        "PRVMSG #cruce-devel test test test test\r\n";
    };


    int pid = cut_fork();
    if(pid == 0) {
        serverHelper(1, expected_messages);
        exit(EXIT_SUCCESS);
    }

    int server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    cut_assert_true(server_sock >= 0, "Failed to create socket");

    cut_assert_true(connect(server_sock, (struct sockaddr *)&test_server, 
                            sizeof(test_server)) >= 0, "Failed to connect");

    irc_sendLobbyMessage("test test test test");
    close(server_sock);

    // test an empty message
    memset(expected_messages, 0, 513);
    strcpy(expected_messages, "PRVMSG #cruce-devel \r\n");

    pid = cut_fork();
    if(pid == 0) {
        serverHelper(1, expected_messages);
        exit(EXIT_SUCCESS);
    }

    int server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    cut_assert_true(server_sock >= 0, "Failed to create socket");

    cut_assert_true(connect(server_sock, (struct sockaddr *)&test_server, 
                            sizeof(test_server)) >= 0, "Failed to connect");

    irc_sendLobbyMessage("");
    close(server_sock);

    // test a message that is longer than 512 (the standard irc line size)
    memset(expected_messages, 0, 514);
    // the message has exactly 512 chars 
    strcpy(expected_messages, "PRVMSG #cruce-devel test test test test test "
           "test test test test test test test test test test test test test "
           "test test test test test test test test test test test test test "
           "test test test test test test test test test test test test test "
           "test test test test test test test test test test test test test "
           "test test test test test test test test test test test test test "
           "test test test test test test test test test test test test test "
           "test test test test test test test test test test test test  test "
           "test test\r\n");

    int server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    cut_assert_true(server_sock >= 0, "Failed to create socket");

    cut_assert_true(connect(server_sock, (struct sockaddr *)&test_server, 
                            sizeof(test_server)) >= 0, "Failed to connect");

    irc_sendLobbyMessage("test test test test test "
                         "test test test test test test test test test test "
                         "test test test test test test test test test test "
                         "test test test test test test test test test test "
                         "test test test test test test test test test test "
                         "test test test test test test test test test test "
                         "test test test test test test test test test test "
                         "test test test test test test test test test test "
                         "test test test test test test test test test test "
                         "test test test test test test test test test test "
                         "test test test test test test test");

    close(server_sock);

}