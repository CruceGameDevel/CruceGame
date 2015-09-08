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


void test_irc_connect()
{
    char expected_messages[3][4][513] = {
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

    int expected_results[] = {1, 0, 0};

    for (int test = 0; test < 3; test++) {
        // Test the return value.
        if (expected_results[test]) {
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

        if (expected_results[test]) {
            char *irc_messages = sniffIrcSentPackets();
            for (int subtest = 0; subtest < 4; subtest++) {
                char *lineStart = strstr(irc_messages,
                                         expected_messages[test][subtest]);
                cut_assert_not_null(lineStart);
            }

            free(irc_messages);
        }
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

void test_irc_toggleRoomStatus()
{
    char serverMessages[4][512] = {
        "WAITING\r\n",
        "PLAYING\r\n",
        ":No topic is set\r\n",
        ":No topic is set\r\n"
    };
    int room_numbers[] = {0, 50, 999, 1000};
    int outputs[] = {1, 2, 0, 0};

    for (int i = 0; i < 4; i++) {
        // First, we test the correctness of the return type.
        int pid = cut_fork();
        if (pid == 0) {
            // It doesn't make sense to do assertions in the child process
            // since they are not executed.
            int sockfd = openLocalhostSocket(8008 + i);
            write(sockfd, serverMessages[i], 512);
            close(sockfd);
            exit(EXIT_SUCCESS);
        }

        sleep(1); // Wait a little bit before connecting to the server.
        cut_assert_equal_int(0, network_connect("localhost", 8008 + i));
        cut_assert_equal_int(outputs[i], irc_toggleRoomStatus(room_numbers[i]));
        cut_assert_equal_int(0, network_disconnect());

        // Then we test the behavior of the function (if it sends the right
        // thing to the server).

        int sockfd = openLocalhostSocket(8099 + i);
        cut_assert_operator_int(sockfd, >=, 0);

        pid = cut_fork();

        if (pid == 0) {
            sleep(1);
            network_connect("localhost", 8099 + i);
            irc_toggleRoomStatus(room_numbers[i]);
            network_disconnect();
            exit(EXIT_SUCCESS);
        }

        char received_message[512];
        cut_assert_operator_int(read(sockfd, received_message, 512), >=, 0);

        close(sockfd);

        char expected_message[512];
        sprintf(expected_message, "TOPIC " ROOM_FORMAT "\r\n", room_numbers[i]);

        cut_assert_equal_string(expected_message, received_message);
    }
}

void test_irc_getNames()
{
    currentRoom = 122; // Assign some random-chosen value.

    char server_output[2][2][512];
    // Build the server's output.
    sprintf(server_output[0][0], ":test.freenode.net 353 dummy = "
            ROOM_FORMAT " :user1 user2\r\n", currentRoom);
    sprintf(server_output[0][1], ":test.freenode.net 366 dummy "
            ROOM_FORMAT " :End of /NAMES list.\r\n", currentRoom);
    sprintf(server_output[1][0], ":test.freenode.net 353 dummy = "
            LOBBY_CHANNEL " :user3 user4\r\n");
    sprintf(server_output[1][1], ":test.freenode.net 366 dummy "
            LOBBY_CHANNEL " :End of /NAMES list.\r\n");

    char user_list[2][512] = {
        "user1 user2",
        "user3 user4"
    };

    int inputs[] = {1, 200};

    for (int i = 0; i < 2; i++) {
        // Test the correctness of the return type.
        int pid = cut_fork();

        if (pid == 0) {
            int sockfd = openLocalhostSocket(8018 + i);

            write(sockfd, server_output[i][0], 512);
            write(sockfd, server_output[i][1], 512);

            close(sockfd);
            exit(EXIT_SUCCESS);
        }

        sleep(1);
        cut_assert_equal_int(0, network_connect("localhost", 8018 + i));
        char *names = irc_getNames(inputs[i]);

        cut_assert_equal_string(user_list[i], names);

        cut_assert_equal_int(0, network_disconnect());
        free(names);

        // Then test if the behavior is the right one.
        int sockfd = openLocalhostSocket(8029 + i);
        cut_assert_operator_int(sockfd, >=, 0);

        pid = cut_fork();
        if (pid == 0) {
            sleep(1);
            network_connect("localhost", 8029 + i);

            // In this case we shouldn't care about what it returns, but we
            // risk a memory leak if we don't free that memory.
            char *names = irc_getNames(inputs[i]);
            free(names);

            network_disconnect();
            exit(EXIT_SUCCESS);
        }

        char received_message[512];
        cut_assert_operator_int(read(sockfd, received_message, 512), >=, 0);

        close(sockfd);

        char expected_message[512];
        if (i == 1) {
            sprintf(expected_message, "NAMES " ROOM_FORMAT "\r\n", currentRoom);
        } else {
            sprintf(expected_message, "NAMES " LOBBY_CHANNEL "\r\n");
        }

        cut_assert_equal_string(expected_message, received_message);
    }
}

void test_irc_createRoom()
{
    int free_rooms[] = {38, 1000}; // 1000 means that all rooms are in use.
    char server_output[2][512];

    for (int i = 0; i < 2; i++) {
        // Test the correctness of the return value first.
        int pid = cut_fork();
        if (pid == 0) {
            int sockfd = openLocalhostSocket(8038 + i);

            for (int j = 0; j <= 999; j++) {
                // We are generating the server response as all the options are
                // checked. This way it will contain meaningful data about the
                // room name.
                if (j == free_rooms[i]) {
                    sprintf(server_output[0], ":test.freenode.net 353 dummy = "
                        ROOM_FORMAT " :user1 user2\r\n", currentRoom);

                    write(sockfd, server_output[0], 512);
                }

                sprintf(server_output[1], ":test.freenode.net 366 dummy "
                    ROOM_FORMAT " :End of /NAMES list.\r\n", currentRoom);

                write(sockfd, server_output[1], 512);

                if (j == free_rooms[i]) {
                    break; // We have reached the first empty channel.
                }
            }

            close(sockfd);

            exit(EXIT_SUCCESS);
        }

        sleep(1);
        cut_assert_equal_int(0, network_connect("localhost", 8038 + i));

        if (free_rooms[i] <= 999) {
            cut_assert_operator_int(irc_createRoom(), >=, 0);
        } else {
            cut_assert_operator_int(irc_createRoom(), <, 0);
        }

        cut_assert_equal_int(0, network_disconnect());

        // Now we are testing the behavior of the function.

        int sockfd = openLocalhostSocket(8049 + i);
        cut_assert_operator_int(sockfd, >=, 0);

        pid = cut_fork();
        if (pid == 0) {
            sleep(1);
            network_connect("localhost", 8049 + i);

            irc_createRoom();

            network_disconnect();

            exit(EXIT_SUCCESS);
        }

        for (int j = 0; j <= 999; j++) {
            char received_message[512];
            cut_assert_operator_int(read(sockfd, received_message, 512), >=, 0);

            char expected_message[512];
            sprintf(expected_message, "NAME " ROOM_FORMAT "\r\n", j);

            cut_assert_equal_string(expected_message, received_message);

            if (j == free_rooms[i]) {
                sprintf(server_output[0], ":test.freenode.net 353 dummy = "
                    ROOM_FORMAT " :user1 user2\r\n", currentRoom);

                write(sockfd, server_output[0], 512);
            }

            sprintf(server_output[1], ":test.freenode.net 366 dummy "
                ROOM_FORMAT " :End of /NAMES list.\r\n", currentRoom);

            write(sockfd, server_output[1], 512);

            if (j == free_rooms[i]) {
                cut_assert_operator_int(read(sockfd, received_message, 512),
                                        >=, 0);
                sprintf(expected_message, "JOIN " ROOM_FORMAT "\r\n", j);

                cut_assert_equal_string(expected_message, received_message);

                break;
            }
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

