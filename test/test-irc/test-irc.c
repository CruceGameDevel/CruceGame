#include <cutter.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netdb.h>
#include <irc.h>
#include <network.h>
#include <errno.h>
#include <errors.h>
#include <helperFunctions.h>

extern int currentRoom;

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
    char prevChar = '\n';
    char currentChar = fgetc(fp);
    int beingParsed = 0;
    while (currentChar != EOF) {
        if (prevChar == '\n') {
            buffer = realloc(buffer, lines * 512);
        }

        if (!beingParsed && prevChar == '\n' && currentChar != ':') {
            beingParsed = 1;
        }

        if (beingParsed) {
            buffer[size] = currentChar;
            size++;
            if (currentChar == '\n') {
                beingParsed = 0;
                lines++; // One more line parsed.
            }
        }

        prevChar = currentChar;
        currentChar = fgetc(fp);
    }

    pclose(fp);

    buffer[size] = '\0';
    return buffer;
}


void test_irc_connect()
{
    char expectedMessages[3][4][513] = {
        {
            "PASS *\r\n",
            "NICK testUser\r\n",
            "USER testUser 8 * :testUser\r\n",
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
            "NICK testUser_\r\n",
            "USER testUser_ 8 * :testUser_\r\n",
            "JOIN " LOBBY_CHANNEL "\r\n"
        }
    };

    char inputs[3][10] = {
        "testUser",
        "",
        "testsUser_"
    };

    int expectedResults[] = {1, 0, 0};

    for (int test = 0; test < 3; test++) {
        // Test the return value.
        if (expectedResults[test]) {
            cut_assert_equal_int(0, irc_connect(inputs[test]));
            cut_assert_equal_int(0, network_disconnect());
        } else
            cut_assert_not_equal_int(0, irc_connect(inputs[test]));

        // Now test if it works.
        int pid = cut_fork();
        if (pid == 0) {
            // In order to have meaningful results, `tcpflow` should start 
            // first.
            sleep(1);
            irc_connect(inputs[test]);
            network_disconnect();
            exit(EXIT_SUCCESS);
        }

        if (expectedResults[test]) {
            char *ircMessages = sniffIrcSentPackets();
            for (int subtest = 0; subtest < 4; subtest++) {
                char *lineStart = strstr(ircMessages,
                                         expectedMessages[test][subtest]);
                cut_assert_not_null(lineStart);
            }

            free(ircMessages);
        }
    }
}

void test_irc_sendLobbyMessage()
{

    char expectedMessages[3][513] = {
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

    // If testParametersp[i] == 1 then
    // cut_assert_equal_int(irc_sendLobbyMessage(inputs[i]), 0) will be issued
    // otherwise cut_assert_not_equal_int(irc_sendLobbyMessage(inputs[i]), 0).
    int testParameters[3] = {1, 1, 1};

    for (int i = 0; i < 3; i++) {
        int pid = cut_fork();
        if (pid == 0) {
            int serverSock = openLocalhostSocket(8091 + i);

            char buffer[513];
            memset(buffer, 0, 513);
            cut_assert_operator_int(read(serverSock, buffer, 513), >, 0);
            cut_assert_equal_string(expectedMessages[i], buffer);

            close(serverSock);
            exit(EXIT_SUCCESS);
        }

        sleep(1);

        cut_assert_equal_int(0, network_connect("localhost", 8091 + i));

        if (testParameters[i])
            cut_assert_equal_int(0, irc_sendLobbyMessage(inputs[i]));
        else
            cut_assert_not_equal_int(0, irc_sendLobbyMessage(inputs[i]));

        cut_assert_equal_int(0, network_disconnect());
    }
}

void test_irc_disconnect()
{

    char expectedMessage[] = "QUIT\r\n";

    int pid = cut_fork();
    if (pid == 0) {
        int serverSock = openLocalhostSocket(8087);

        char buffer[513];
        memset(buffer, 0, 513);
        cut_assert_operator_int(read(serverSock, buffer, 513), >, 0);
        cut_assert_equal_string(expectedMessage, buffer);

        close(serverSock);

        serverSock = openLocalhostSocket(8088);
        close(serverSock);

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
    char expectedMessages[4][513] = {
        "JOIN " LOBBY_CHANNEL "001\r\n",
        "JOIN " LOBBY_CHANNEL "000\r\n",
        "JOIN " LOBBY_CHANNEL "999\r\n",
        "JOIN " LOBBY_CHANNEL "1000\r\n"
    };

    int inputs[4] = {1, 0, 999, 1000};

    int testParameters[4] = {1, 1, 1, 0};

    for (int test = 0; test < 4; test++) {
        int pid = cut_fork();
        if (pid == 0) {
            int serverSock = openLocalhostSocket(8100);

            char buffer[513];

            if (testParameters[test]) {
                cut_assert_operator_int(read(serverSock, buffer, 513), >, 0);
            } else {
                // If we try to connect to an invalid room (with id 1000,
                // for example) the server should not receive any message
                // from the client. Thus, read() will return 0.
                cut_assert_equal_int(0, read(serverSock, buffer, 513));
            }

            if (testParameters[test])
                cut_assert_equal_string(expectedMessages[test], buffer);

            close(serverSock);
            exit(EXIT_SUCCESS);
        }

        sleep(1);

        cut_assert_equal_int(0, network_connect("localhost", 8100));

        if (testParameters[test]) {
            cut_assert_equal_int(0, irc_joinRoom(inputs[test]));
        } else {
            cut_assert_not_equal_int(0, irc_joinRoom(inputs[test]));
        }

        cut_assert_equal_int(0, network_disconnect());
    }
}

void test_irc_leaveRoom()
{
    char *expectedMessage[] = {"PART " LOBBY_CHANNEL "004\r\n",
                                "PART " LOBBY_CHANNEL "012\r\n",
                                "PART " LOBBY_CHANNEL "345\r\n"};

    int currentRoomValues[] = {5, 12, 345};

    for (int i = 0; i < 3; i++) {
        currentRoom = currentRoomValues[i];

        int pid = cut_fork();
        if (pid == 0) {
            int serverSock = openLocalhostSocket(8110 + i);

            char buffer[513];
            memset(buffer, 0, 513);

            read(serverSock, buffer, 513);
            cut_assert_equal_string(expectedMessage[i], buffer);

            close(serverSock);
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

void test_irc_toggleRoomStatus()
{
    char serverMessages[5][512] = {
        "",
        "WAITING\r\n",
        "PLAYING\r\n",
        ":No topic is set\r\n",
        ""
    };
    int roomNumbers[] = {-1, 50, 125, 999, 1000};
    int outputs[] = {PARAMETER_OUT_OF_RANGE, NO_ERROR, NO_ERROR,
                     TOGGLE_ROOM_STATUS_ERROR, PARAMETER_OUT_OF_RANGE};

    char expectedMessagesFormat[5][2][512] = {
        {"", ""},
        {"TOPIC " ROOM_FORMAT "\r\n", "TOPIC " ROOM_FORMAT " PLAYING\r\n"},
        {"TOPIC " ROOM_FORMAT "\r\n", "TOPIC " ROOM_FORMAT " WAITING\r\n"},
        {"TOPIC " ROOM_FORMAT "\r\n", ""},
        {"", ""}
    };

    for (int test = 0; test < 5; test++) {
        // First, we test the correctness of the return type.
        int pid = cut_fork();
        if (pid == 0) {
            // It doesn't make sense to do assertions in the child process
            // since they are not executed.
            int sockfd = openLocalhostSocket(8008);
            write(sockfd, serverMessages[test], sizeof(serverMessages[test]));
            close(sockfd);
            exit(EXIT_SUCCESS);
        }

        sleep(1); // Wait a little bit before connecting to the server.
        cut_assert_equal_int(0, network_connect("localhost", 8008));
        cut_assert_equal_int(outputs[test],
                             irc_toggleRoomStatus(roomNumbers[test]));
        cut_assert_equal_int(0, network_disconnect());

        // Then we test the behavior of the function (if it sends the right
        // thing to the server).
        pid = cut_fork();

        if (pid == 0) {
            network_connect("localhost", 8099);
            irc_toggleRoomStatus(roomNumbers[test]);
            network_disconnect();
            exit(EXIT_SUCCESS);
        }

        int sockfd = openLocalhostSocket(8099);

        write(sockfd, serverMessages[test], sizeof(serverMessages[test]));

        char receivedMessage[512];
        char expectedMessage[512];

        for (int subtest = 0; subtest < 2; subtest++) {
            memset(expectedMessage, 0, sizeof(expectedMessage));
            memset(receivedMessage, 0, sizeof(receivedMessage));

            cut_assert_operator_int(read(sockfd, receivedMessage, 512), >=, 0);
            if (strlen(expectedMessagesFormat[test][subtest]))
                sprintf(expectedMessage,
                        expectedMessagesFormat[test][subtest],
                        roomNumbers[test]);

            cut_assert_equal_string(expectedMessage, receivedMessage);
        }

        close(sockfd);
    }
}

/**
 * Test for irc_sendRoomMessage.
 * It works by testing a exceptional case, then is created a new process that
 * opens a socket, connecting to it and to a room. Some data is transfered to
 * server using irc_sendRoomMessage and the server checks if the data has been
 * transfered correctly. Then is tested another one exceptional case.
 * Then deconnecting from the server and the room.
 *
 * This function assumes the use of currentRoom private variable in the irc
 * module.
 */
void test_irc_sendRoomMessage()
{
    cut_assert_operator_int(NO_ERROR, >, irc_sendRoomMessage("message"),
                            "Send data to non-existent room succeeded");

    currentRoom = 1;
    int pid = cut_fork();
    if (pid == 0) {
        int serverSockfd = openLocalhostSocket(8200), returnedValue = 0;

        char buffer[513];
        memset(buffer, 0, 513);

        read(serverSockfd, buffer, 513);
        if (strcmp(buffer, "PRIVMSG #cruce-game001 :message") != 0)
            returnedValue++;

        close(serverSockfd);

        sleep(1);

        exit(returnedValue);
    }

    sleep(1);

    cut_assert_equal_int(NO_ERROR, network_connect("localhost", 8200));
    int value;

    cut_assert_equal_int(NO_ERROR, irc_sendRoomMessage("message"),
                         "Send data failed");
    cut_assert_operator_int(NO_ERROR, >, irc_sendRoomMessage(NULL),
                            "Send wrong data succeeded");

    wait(&value);

    cut_assert_equal_int(0, value);

    cut_assert_equal_int(NO_ERROR, network_disconnect());

    currentRoom = -1;
}

/**
 * Test for irc_invite.
 * It works by testing a exceptional case, then is created a new process that
 * opens a socket, connecting to it and to a room. Some data is transfered to
 * server using irc_invite and the server checks if the data has been transfered
 * correctly. Then is tested another one exceptional case.
 * Then deconnecting from the server and the room.
 *
 * This function assumes the use of currentRoom private variable in the irc
 * module.
 */
void test_irc_invite()
{
    cut_assert_not_equal_int(NO_ERROR, irc_invite("user"));

    int pid = cut_fork();
    if (pid == 0) {
        int serverSockfd = openLocalhostSocket(8201), returnedValue = 0;

        char buffer[513];
        memset(buffer, 0, 513);

        read(serverSockfd, buffer, 513);
        if (strcmp(buffer, "INVITE user #cruce-game001") != 0)
            returnedValue++;

        close(serverSockfd);

        sleep(1);

        exit(returnedValue);
    }

    sleep(1);

    cut_assert_equal_int(NO_ERROR, network_connect("localhost", 8201));
    currentRoom = 1;

    int value;

    cut_assert_equal_int(NO_ERROR, irc_invite("user"), "Invite user failed");
    cut_assert_operator_int(NO_ERROR, >, irc_invite(NULL));

    wait(&value);

    cut_assert_equal_int(0, value);

    cut_assert_equal_int(NO_ERROR, network_disconnect());

    currentRoom = -1;
}

/**
 * Test for irc_getAvailableRooms.
 * It works by testing a exceptional case, then is created a new process that
 * opens a socket, connecting to it and to a room. Some data is transfered to
 * server using irc_getAvailableRooms and the server checks if the data has been
 * transfered correctly and if the data bas been trasfered correctly the server
 * send two available rooms to client. Then is tested a exceptional case and
 * if the server sent two available rooms.
 * Then deconnecting from the server and the room.
 *
 * This function assumes the use of currentRoom private variable in the irc
 * module.
 */
void test_irc_getAvailableRooms()
{
    cut_assert_equal_pointer(NULL, irc_getAvailableRooms());

    int pid = cut_fork();
    if (pid == 0) {
        int serverSockfd = openLocalhostSocket(8202), returnedValue = 0;

        char buffer[513];
        memset(buffer, 0, 513);

        read(serverSockfd, buffer, 513);
        if (strcmp(buffer, "LIST") != 0)
            returnedValue++;
        else
            write(serverSockfd, "#cruce-game001 #cruce-game512", 29);

        close(serverSockfd);

        sleep(1);

        exit(returnedValue);
    }

    sleep(1);

    cut_assert_equal_int(NO_ERROR, network_connect("localhost", 8202));

    int value;
    char *rooms = irc_getAvailableRooms();

    currentRoom = 2;
    cut_assert_equal_pointer(NULL, irc_getAvailableRooms());
    currentRoom = -1;

    cut_assert_equal_string("#cruce-game001 #cruce-game512", rooms);

    wait(&value);

    cut_assert_equal_int(0, value);

    cut_assert_equal_int(NO_ERROR, network_disconnect());
}

