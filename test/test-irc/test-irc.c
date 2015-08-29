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
    char expected_messages1[4][513] = {
        "PASS *\r\n",
        "NICK test_user\r\n",
        "USER test_user 8 * :test_user\r\n",
        "JOIN #cruce-devel\r\n"
    };

    int pid = cut_fork();
    if (pid == 0) {
        int server_sock = serverHelper();

        char buffer[513];
        for (int i = 0; i < 4; i++) {
            memset(buffer, 0, 513);
            cut_assert_true(read(server_sock, buffer, 513) >= 0, 
                            "Failed to read from server");
            cut_assert_equal_strings(expected_messages1[i], buffer);
        }
        close(server_sock);

        exit(EXIT_SUCCESS);
    }

    irc_connect("test_user");

    // test for user name: (empty user name)
    char expected_messages2[4][513] = {
        "PASS *\r\n",
        "NICK \r\n",
        "USER  8 * :\r\n",
        "JOIN #cruce-devel\r\n"
    };

    pid = cut_fork();
    if (pid == 0) {
        int server_sock = serverHelper();

        char buffer[513];
        for (int i = 0; i < 4; i++) {
            memset(buffer, 0, 513);
            cut_assert_true(read(server_sock, buffer, 513) >= 0, 
                            "Failed to read from server");
            cut_assert_equal_strings(expected_messages2[i], buffer);
        }
        close(server_sock);

        exit(EXIT_SUCCESS);
    }

    irc_connect("");

    // test with a nick name bigger than 9 chars: test_user_ (10 chars)
    char expected_message3[4][513] = {
        "PASS *\r\n",
        "NICK test_user_\r\n",
        "USER test_user_ 8 * :test_user_\r\n",
        "JOIN #cruce-devel\r\n"
    };

    pid = cut_fork();
    if (pid == 0) {
        int server_sock = serverHelper();

        char buffer[513];
        for (int i = 0; i < 4; i++) {
            memset(buffer, 0, 513);
            cut_assert_true(read(server_sock, buffer, 513) >= 0, 
                            "Failed to read from server");
            cut_assert_equal_strings(expected_messages3[i], buffer);
        }
        close(server_sock);

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
    char expected_message1[513] = {
        "PRIVMSG #cruce-devel test test test test\r\n";
    };

    int pid = cut_fork();
    if(pid == 0) {
        int server_sock = serverHelper();

        char buffer[513];
        memset(buffer, 0, 513);
        cut_assert_true(read(server_sock, buffer, 513) >= 0, 
                        "Failed to read from server");
        cut_assert_equal_strings(expected_message1, buffer);

        close(server_sock);
        exit(EXIT_SUCCESS);
    }

    int server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    cut_assert_true(server_sock >= 0, "Failed to create socket");

    cut_assert_true(connect(server_sock, (struct sockaddr *)&test_server, 
                            sizeof(test_server)) >= 0, "Failed to connect");

    irc_sendLobbyMessage("test test test test");
    close(server_sock);

    // test an empty message
    char expected_message2[] = {"PRIVMSG #cruce-devel \r\n"};

    pid = cut_fork();
    if(pid == 0) {
        int server_sock = serverHelper();

        char buffer[513];
        memset(buffer, 0, 513);
        cut_assert_true(read(server_sock, buffer, 513) >= 0, 
                        "Failed to read from server");
        cut_assert_equal_strings(expected_message2, buffer);

        close(server_sock);
        exit(EXIT_SUCCESS);
    }

    int server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    cut_assert_true(server_sock >= 0, "Failed to create socket");

    cut_assert_true(connect(server_sock, (struct sockaddr *)&test_server, 
                            sizeof(test_server)) >= 0, "Failed to connect");

    irc_sendLobbyMessage("");
    close(server_sock);

    // the message has exactly 512 chars 
    char expected_message3[] = {
           "PRIVMSG #cruce-devel test test test test test "
           "test test test test test test test test test test test test test "
           "test test test test test test test test test test test test test "
           "test test test test test test test test test test test test test "
           "test test test test test test test test test test test test test "
           "test test test test test test test test test test test test test "
           "test test test test test test test test test test test test test "
           "test test test test test test test test test test test test  test "
           "test tes\r\n"
    };

    pid = cut_fork();
    if (pid == 0) {
        int server_sock = serverHelper();

        char buffer[513];
        memset(buffer, 0, 513);
        cut_assert_true(read(server_sock, buffer, 513) >= 0, 
                        "Failed to read from server");
        cut_assert_equal_strings(expected_message3, buffer);

        close(server_sock);
        exit(EXIT_SUCCESS);
    }

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

void test_irc_disconnect()
{
    struct sockaddr_in test_server;

    memset(&test_server, 0, sizeof(test_server));
    test_server.sin_family = AF_INET;
    test_server.sin_addr.s_addr = inet_addr("localhost");
    test_server.sin_port = htons(8080);

    char expected_message[513] = "QUIT\r\n";

    int pid = cut_fork();
    if (pid == 0) {
        int server_sock = serverHelper();

        char buffer[513];
        memset(buffer, 0, 513);
        cut_assert_true(read(server_sock, buffer, 513) >= 0, 
                        "Failed to read message from client");
        cut_assert_equal_strings(expected_message, buffer);

        close(server_sock);
        exit(EXIT_SUCCESS);
    }

    int server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    cut_assert_true(connect(server_sock, (struct sockaddr *)&test_server, 
                            sizeof(test_server)) >= 0, 
                     "Failed o connect to the server");

    cut_assert_equal_int(irc_disconnect(), 0);
    clsoe(server_sock);
}

void test_irc_joinRoom()
{
    struct sockaddr_in test_server;

    memset(&test_server, 0, sizeof(test_server));
    test_server.sin_family = AF_INET;
    test_server.sin_addr.s_addr = inet_addr("localhost");
    test_server.sin_port = htons(8080);

    // test the room of the form #cruce-devel001
    char expected_message1[] = "JOIN #cruce-devel001\r\n";

    int pid = cut_fork();
    if (pid == 0) {
        int server_sock = serverHelper();

        char buffer[513];
        memset(buffer, 0, 513);
        cut_assert_true(read(server_sock, buffer, 513) >= 0, 
                        "Failed to read message from client");
        cut_assert_equal_strings(expected_message1, buffer);

        close(server_sock);
        exit(EXIT_SUCCESS);
    }

    int server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    cut_assert_true(connect(server_sock, (struct sockaddr *)&test_server, 
                            sizeof(test_server)) >= 0, 
                    "Failed o connect to the server");

    cut_assert_equal_int(irc_joinRoom(1), 0);

    close(server_sock);

    // test for a room of the form #cruce-devel000
    char expected_message2[] = "JOIN #cruce-devel000\r\n";

    pid = cut_fork();
    if (pid == 0) {
        int server_sock = serverHelper();

        char buffer[513];
        memset(buffer, 0, 513);
        cut_assert_true(read(server_sock, buffer, 513) >= 0, 
                        "Failed to read message from client");
        cut_assert_equal_strings(expected_message2, buffer);

        close(server_sock);
        exit(EXIT_SUCCESS);
    }

    int server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    cut_assert_true(connect(server_sock, (struct sockaddr *)&test_server, 
                            sizeof(test_server)) >= 0, 
                    "Failed o connect to the server");

    cut_assert_equal_int(irc_joinRoom(0), 0);

    close(server_sock);

    // test for a room of the form #cruce-devel999
    char expected_message3[] = "JOIN #cruce-devel999\r\n";

    pid = cut_fork();
    if (pid == 0) {
        int server_sock = serverHelper();

        char buffer[513];
        memset(buffer, 0, 513);
        cut_assert_true(read(server_sock, buffer, 513) >= 0, 
                        "Failed to read message from client");
        cut_assert_equal_strings(expected_message3, buffer);

        close(server_sock);
        exit(EXIT_SUCCESS);
    }

    int server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    cut_assert_true(connect(server_sock, (struct sockaddr *)&test_server, 
                            sizeof(test_server)) >= 0, 
                    "Failed o connect to the server");

    cut_assert_equal_int(irc_joinRoom(999), 0);

    close(server_sock);

    // test for a room of the form #cruce-devel1000
    char expected_message4[] = "JOIN #cruce-devel1000\r\n";

    pid = cut_fork();
    if (pid == 0) {
        int server_sock = serverHelper();

        char buffer[513];
        memset(buffer, 0, 513);
        cut_assert_true(read(server_sock, buffer, 513) >= 0, 
                        "Failed to read message from client");
        cut_assert_equal_strings(expected_message4, buffer);

        close(server_sock);
        exit(EXIT_SUCCESS);
    }

    int server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    cut_assert_true(connect(server_sock, (struct sockaddr *)&test_server, 
                            sizeof(test_server)) >= 0, 
                    "Failed o connect to the server");

    cut_assert_not_equal_int(irc_joinRoom(1000), 0);

    close(server_sock);
}

void test_irc_leaveRoom()
{
    struct sockaddr_in test_server;

    memset(&test_server, 0, sizeof(test_server));
    test_server.sin_family = AF_INET;
    test_server.sin_addr.s_addr = inet_addr("localhost");
    test_server.sin_port = htons(8080);

    // test for a room of the form #cruce-devel002
    currentRoom = 2;
    char expected_message[] = "PART #cruce-devel002\r\n";

    int pid = cut_fork();
    if (pid == 0) {
        int server_sock = serverHelper();

        char buffer[513];
        memset(buffer, 0, 513);
        cut_assert_true(read(server_sock, buffer, 513) >= 0,
                        "Failed to read message from client");
        cut_assert_equal_strings(expected_message2, buffer);

        close(server_sock);
        exit(EXIT_SUCCESS);
    }

    int server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    cut_assert_true(connect(server_sock, (struct sockaddr *)&test_server,
                            sizeof(test_server)) >= 0,
                    "Failed o connect to the server");

    cut_assert_equal_int(irc_leaveRoom(), 0);

    // No message should be send to the server when the room number is invalid
    currentRoom = -1000;

    int pid = cut_fork();
    if (pid == 0) {
        int server_sock = serverHelper();
        char buffer[513];
        memset(buffer, 0, 513);
        cut_assert_not_equal_int(read(server_sock, buffer, 513), 0);

        close(server_sock);
        exit(EXIT_SUCCESS);
    }

    int server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    cut_assert_true(connect(server_sock, (struct sockaddr *)&test_server,
                            sizeof(test_server)) >= 0,
                    "Failed o connect to the server");

    cut_assert_not_equal_int(irc_leaveRoom(), 0);

    close(server_sock);
}
