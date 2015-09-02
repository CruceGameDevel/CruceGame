#include <cutter.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <irc.h>
#include <network.h>
#include <errno.h>

extern int currentRoom;

/**
 * Helper to open a local server socket.
 * Param port The port on which must be created the server.
 * Returns a sockfd.
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

/**
 * Helper to connect to a local server socket.
 * Param port The port on which runs the server.
 * Returns none.
 *
 * This function assumes the use of sockfd private variable in the networking
 * module.
 */
int connectToLocalhostSocket(int port)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
    cut_assert_operator_int(sockfd, >=, 0,
                            "Could not connect to the server thread");

    struct hostent *server = gethostbyname("localhost");
    cut_assert_not_null(server, "No such host");

    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr_list[0],
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(port);

    int conn = connect(sockfd, (struct sockaddr *)&serv_addr,
                       sizeof(serv_addr));

    cut_assert_operator_int(conn, >=, 0,
                            "Error connecting to the server process");
    return sockfd;
}

/**
 * Helper to track packages sent from this computer to the irc server.
 * 
 * Return dynamically allocated array of chars containing the packets sent from
 * this computer to the irc server. You have to free the memory after you are 
 * done with the buffer.
 */
char *sniffIrcSentPackets()
{
    FILE* fp;

    // `2>/dev/null` because we don't want to pollute the result with meaningles
    // data.
    fp = popen("sudo timeout 10 tcpflow -C -i any port 6667 2>/dev/null", "r");

    char* buffer = malloc(512); // 512, standard irc line size.
    int size = 0;
    int lines = 1;

    // Store only the line sent from this machine, ignore what the irc server
    // sends back.
    char prev_char = '\n';
    char current_char = fgetc(fp);
    int being_parsed = 0;
    while (current_char != EOF) {
        if (prev_char == '\n') {
            buffer = realloc(buffer, lines * 512);
        }

        if (!being_parsed && prev_char == '\n' && current_char != ':') {
            being_parsed = 1;
        }

        if (being_parsed) {
            buffer[size] = current_char;
            size++;
            if (current_char == '\n') {
                being_parsed = 0;
                lines++; // One more line parsed.
            }
        }

        prev_char = current_char;
        current_char = fgetc(fp);
    }

    pclose(fp);

    buffer[size] = '\0';
    return buffer;
}

bool hasLine(char* haystack, char* needle)
{
    char buffer[512];
    char *next = strchr(haystack, '\n');
    char *cur = haystack;
    while (next != NULL) {
        // +1, including the line feed.
        sprintf(buffer, "%.*s", (int)(next - cur) + 1, cur);
        if (!strcmp(buffer, needle)) {
            return true;
        }

        cur = next + 1;
        next = strchr(cur, '\n');
    }
    return false;
}

void test_irc_connect()
{
    char expected_messages[3][4][513] = {
        {
            "PASS *\r\n",
            "NICK test_user\r\n",
            "USER test_user 8 * :test_user\r\n",
            "JOIN " LOBBY_CHANNEL "\r\n"
        },
        {
            "PASS *\r\n",
            "NICK \r\n",
            "USER  8 * :\r\n",
            "JOIN " LOBBY_CHANNEL "\r\n"
        },
        {
            "PASS *\r\n",
            "NICK test_user_\r\n",
            "USER test_user_ 8 * :test_user_\r\n",
            "JOIN " LOBBY_CHANNEL "\r\n"
        }
    };

    char inputs[3][10] = {
        "test_user",
        "",
        "test_user_"
    };

    int expected_results[] = {1, 1, 0};

    for (int i = 0; i < 3; i++) {
        int pid = cut_fork();

        // Test the return type.
        if(pid == 0) {
            // The process will stop after 10 seconds.
            char *irc_messages = sniffIrcSentPackets();
            free(irc_messages);
            exit(EXIT_SUCCESS);
        }

        sleep(5);
        if (expected_results[i]) {
            cut_assert_equal_int(0, irc_connect(inputs[i]));
        } else {
            cut_assert_not_equal_int(0, irc_connect(inputs[i]));
        }

        cut_assert_equal_int(0, network_disconnect());
        
        // Now test if it works.
        pid = cut_fork();
        if (pid == 0) {
            // In order to have meaningful results, `tcpflow` should start 
            // first.
            sleep(5);
            irc_connect(inputs[i]);
            network_disconnect();
            exit(EXIT_SUCCESS);
        }

        char *irc_messages = sniffIrcSentPackets();
        int matches = 0;
        for (int j = 0; j < 4; j++) {
            char buffer[512];

            // Read the string line by line.
            char *next = strchr(irc_messages, '\n');
            char *cur = irc_messages;
            while (next != NULL) {
                // +1, including the line feed.
                sprintf(buffer, "%.*s", (int)(next - cur) + 1, cur);
                if (!strcmp(buffer, expected_messages[i][j])) {
                    matches++;
                }

                cur = next + 1;
                next = strchr(cur, '\n');
            }

        }
        free(irc_messages);

        cut_assert_equal_int(4, matches);
    }
}

void test_irc_sendLobbyMessage()
{

    char expected_messages[3][513] = {
        "PRIVMSG " LOBBY_CHANNEL " test test test test\r\n",
        "PRIVMSG " LOBBY_CHANNEL " \r\n",
        // Begins here
        "PRIVMSG " LOBBY_CHANNEL " "
        "test test test test test test test test test test test test test "
        "test test test test test test test test test test test test test "
        "test test test test test test test test test test test test test "
        "test test test test test test test test test test test test test "
        "test test test test test test test test test test test test test "
        "test test test test test test test test test test test test test "
        "test test test test test test test test test test test test test "
        "test test test test test test test\r\n"
        // and ends here.
    };

    char inputs[3][514] = {
        "test test test test",
        "",
        // Begins here
        "test test test test test test test test test test test test test "
        "test test test test test test test test test test test test test "
        "test test test test test test test test test test test test test "
        "test test test test test test test test test test test test test "
        "test test test test test test test test test test test test test "
        "test test test test test test test test test test test test test "
        "test test test test test test test test test test test test test "
        "test test test test test test test test test test test"
        // and ends here.
    };

    // If test_parametersp[i] == 1 then
    // cut_assert_equal_int(irc_sendLobbyMessage(inputs[i]), 0) will be issued
    // otherwise cut_assert_not_equal_int(irc_sendLobbyMessage(inputs[i]), 0).
    int test_parameters[3] = {1, 1, 1};

    for (int i = 0; i < 3; i++) {
        int pid = cut_fork();
        if (pid == 0) {
            int server_sock = openLocalhostSocket(8091 + i);

            char buffer[513];
            memset(buffer, 0, 513);
            cut_assert_operator_int(read(server_sock, buffer, 513), >, 0);
            cut_assert_equal_string(expected_messages[i], buffer);

            close(server_sock);
            exit(EXIT_SUCCESS);
        }

        sleep(1);

        cut_assert_equal_int(0, network_connect("localhost", 8091 + i));

        if (test_parameters[i])
            cut_assert_equal_int(0, irc_sendLobbyMessage(inputs[i]));
        else
            cut_assert_not_equal_int(0, irc_sendLobbyMessage(inputs[i]));

        cut_assert_equal_int(0, network_disconnect());
    }
}

void test_irc_disconnect()
{

    char expected_message[] = "QUIT\r\n";

    int pid = cut_fork();
    if (pid == 0) {
        int server_sock = openLocalhostSocket(8087);

        char buffer[513];
        memset(buffer, 0, 513);
        cut_assert_operator_int(read(server_sock, buffer, 513), >, 0);
        cut_assert_equal_string(expected_message, buffer);

        close(server_sock);

        server_sock = openLocalhostSocket(8088);
        close(server_sock);

        exit(EXIT_SUCCESS);
    }

    sleep(1);

    cut_assert_equal_int(0, network_connect("localhost", 8087));
    cut_assert_equal_int(0, irc_disconnect());

    sleep(1);

    // Check if connection is possible after irc_disconnect has been called;
    // It should reset the connection.
    cut_assert_equal_int(0, network_connect("localhost", 8088));

    cut_assert_equal_int(0, network_disconnect());
}

void test_irc_joinRoom()
{
    char expected_messages[4][513] = {
        "JOIN " LOBBY_CHANNEL "001\r\n",
        "JOIN " LOBBY_CHANNEL "000\r\n",
        "JOIN " LOBBY_CHANNEL "999\r\n",
        "JOIN " LOBBY_CHANNEL "1000\r\n"
    };

    int inputs[4] = {1, 0, 999, 1000};

    int test_parameters[4] = {1, 1, 1, 0};

    for (int i = 0; i < 4; i++) {
        int pid = cut_fork();
        if (pid == 0) {
            int server_sock = openLocalhostSocket(8100);

            char buffer[513];
            memset(buffer, 0, 513);

            if (test_parameters[i]) {
                cut_assert_operator_int(read(server_sock, buffer, 513), >=, 0);
            } else {
                // If we try to connect to an invalid room (`" LOBBY_CHANNEL "1000`,
                // for example) the server should not receive any message
                // from the client. Thus, `read` will return 0.
                cut_assert_equal_int(0, read(server_sock, buffer, 513));
            }

            if (test_parameters[i]) {
                cut_assert_equal_string(expected_messages[i], buffer);
            }

            close(server_sock);
            exit(EXIT_SUCCESS);
        }

        sleep(1);

        cut_assert_equal_int(0, network_connect("localhost", 8100));

        if (test_parameters[i]) {
            cut_assert_equal_int(0, irc_joinRoom(inputs[i]));
        } else {
            cut_assert_not_equal_int(0, irc_joinRoom(inputs[i]));
        }

        cut_assert_equal_int(0, network_disconnect());
    }
}

void test_irc_leaveRoom()
{
    char *expected_message[] = {"PART " LOBBY_CHANNEL "004\r\n",
                                "PART " LOBBY_CHANNEL "012\r\n",
                                "PART " LOBBY_CHANNEL "345\r\n"};

    int currentRoomValues[] = {5, 12, 345};

    for (int i = 0; i < 3; i++) {
        currentRoom = currentRoomValues[i];

        int pid = cut_fork();
        if (pid == 0) {
            int server_sock = openLocalhostSocket(8110 + i);

            char buffer[513];
            memset(buffer, 0, 513);

            read(server_sock, buffer, 513);
            cut_assert_equal_string(expected_message[i], buffer);

            close(server_sock);
            exit(EXIT_SUCCESS);
        }

        sleep(1);

        cut_assert_equal_int(0, network_connect("localhost", 8110 + i));

        cut_assert_equal_int(0, irc_leaveRoom());

        // Check if the current room has became invalid.
        cut_assert_operator_int(0, >, currentRoom);

        cut_assert_equal_int(0, network_disconnect());
    }

    cut_assert_not_equal_int(0, irc_leaveRoom());
}

void test_foo()
{
    int pid = cut_fork();
    if (pid == 0) {
        cut_assert_equal_int(1, 2000);
        exit(EXIT_SUCCESS);
    }
    // cut_assert_equal_int(1, 2000);
}