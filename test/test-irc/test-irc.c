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

int fd[2];
char buffer[1024];

// Open a pipe to test the result.
void cut_setup() {
    cut_assert_equal_int(0, socketpair(AF_LOCAL, SOCK_STREAM, 0, fd));
    network_setSockfd(fd[1]);
    memset(buffer, 0, 1024);
}

// Check if after the tests ran, the pipe is emtpy.
void cut_teardown() {
    fd_set set;

    FD_ZERO(&set);
    FD_SET(fd[0], &set);

    struct timeval timeout;
    timeout.tv_sec  = 1;
    timeout.tv_usec = 0;

    cut_assert_equal_int(0, select(fd[0] + 1, &set, NULL, NULL, &timeout));
    close(fd[0]);
    close(fd[1]);
}

void test_irc_connect()
{
    char inputs[4][10] = {
        "testUser",
        "testUser",
        "",
        "testsUser_"
    };

    char expectedMessages[513] = {"localhost 6667\r\n"
                                  "PASS *\r\n"
                                  "NICK testUser\r\n"
                                  "USER testUser 8 * :testUser\r\n"
                                  "JOIN " LOBBY_CHANNEL "\r\n" };

    // Reset network's iternal sockfd.
    network_setSockfd(-1);

    // Check if the send fails (no previous socket has been setup, since
    // network connect is dummy).
    cut_assert_not_equal_int(0, irc_connect(inputs[0]));

    network_setSockfd(fd[1]);

    cut_assert_equal_int(0,     irc_connect(inputs[1]));
    // Check user length errors.
    cut_assert_not_equal_int(0, irc_connect(inputs[2]));
    cut_assert_not_equal_int(0, irc_connect(inputs[3]));

    // Make the network module use the pipe for sending results.
    read(fd[0], buffer, strlen(expectedMessages));
    cut_assert_equal_string(expectedMessages, buffer);

}

void test_irc_sendLobbyMessage()
{

    char expectedMessages[1024] = {
        "PRIVMSG " LOBBY_CHANNEL " test test test test\r\n"
        "PRIVMSG " LOBBY_CHANNEL " \r\n"
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
        "test test test test test test test"
        // and ends here.
    };

    cut_assert_equal_int(0, irc_sendLobbyMessage(inputs[0]));
    cut_assert_equal_int(0, irc_sendLobbyMessage(inputs[1]));
    cut_assert_equal_int(0, irc_sendLobbyMessage(inputs[2]));

    cut_assert_operator_int(read(fd[0], buffer, 1024), >, 0);
    cut_assert_equal_string(expectedMessages, buffer);
}

void test_irc_disconnect()
{
    char expectedMessage[] = "QUIT\r\n";

    cut_assert_equal_int(0, irc_disconnect());
    cut_assert_operator_int(read(fd[0], buffer, 1024), >, 0);
    cut_assert_equal_string(expectedMessage, buffer);
    cut_assert_not_equal_int(0, irc_disconnect());
}

void test_irc_joinRoom()
{
    char expectedMessages[1024];

    sprintf(buffer, "JOIN "ROOM_FORMAT"\r\n", 1);
    strcpy(expectedMessages, buffer);

    sprintf(buffer, "JOIN "ROOM_FORMAT"\r\n", 0);
    strcat(expectedMessages, buffer);

    sprintf(buffer, "JOIN "ROOM_FORMAT"\r\n", 999);
    strcat(expectedMessages, buffer);

    cut_assert_equal_int(0,     irc_joinRoom(1));
    cut_assert_equal_int(0,     irc_joinRoom(0));
    cut_assert_equal_int(0,     irc_joinRoom(999));
    cut_assert_not_equal_int(0, irc_joinRoom(1000));

    cut_assert_operator_int(read(fd[0], buffer, 1024), >, 0);
    cut_assert_equal_string(expectedMessages, buffer);
}


void test_irc_leaveRoom()
{
    extern int currentRoom;

    currentRoom = -1;
    cut_assert_not_equal_int(0, irc_leaveRoom());
    cut_assert_operator_int(0, >, currentRoom);

    currentRoom = 4;
    cut_assert_equal_int(0, irc_leaveRoom());
    cut_assert_operator_int(0, >, currentRoom);

    currentRoom = 12;
    cut_assert_equal_int(0, irc_leaveRoom());
    cut_assert_operator_int(0, >, currentRoom);

    currentRoom = 345;
    cut_assert_equal_int(0, irc_leaveRoom());
    cut_assert_operator_int(0, >, currentRoom);

    char expectedMessages[1024];

    sprintf(buffer, "PART "ROOM_FORMAT"\r\n", 4);
    strcpy(expectedMessages, buffer);

    sprintf(buffer, "PART "ROOM_FORMAT"\r\n", 12);
    strcat(expectedMessages, buffer);

    sprintf(buffer, "PART "ROOM_FORMAT"\r\n", 345);
    strcat(expectedMessages, buffer);

    cut_assert_operator_int(read(fd[0], buffer, 1024), >, 0);
    cut_assert_equal_string(expectedMessages, buffer);
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
        printf("Begun test %d\n", test);
        write(fd[0], serverMessages[test], strlen(serverMessages[test]));
        cut_assert_equal_int(outputs[test],
                             irc_toggleRoomStatus(roomNumbers[test]));

//
//        // First, we test the correctness of the return type.
//        int pid = cut_fork();
//        if (pid == 0) {
//            // It doesn't make sense to do assertions in the child process
//            // since they are not executed.
//            int sockfd = openLocalhostSocket(8008);
//            write(sockfd, serverMessages[test], sizeof(serverMessages[test]));
//            close(sockfd);
//            exit(EXIT_SUCCESS);
//        }
//
//        sleep(1); // Wait a little bit before connecting to the server.
//        cut_assert_equal_int(0, network_connect("localhost", 8008));
//        cut_assert_equal_int(outputs[test],
//                             irc_toggleRoomStatus(roomNumbers[test]));
//        cut_assert_equal_int(0, network_disconnect());
//
//        // Then we test the behavior of the function (if it sends the right
//        // thing to the server).
//        pid = cut_fork();
//
//        if (pid == 0) {
//            network_connect("localhost", 8099);
//            irc_toggleRoomStatus(roomNumbers[test]);
//            network_disconnect();
//            exit(EXIT_SUCCESS);
//        }
//
//        int sockfd = openLocalhostSocket(8099);
//
//        write(sockfd, serverMessages[test], sizeof(serverMessages[test]));
//
//        char receivedMessage[512];
//        char expectedMessage[512];
//
//        for (int subtest = 0; subtest < 2; subtest++) {
//            memset(expectedMessage, 0, sizeof(expectedMessage));
//            memset(receivedMessage, 0, sizeof(receivedMessage));
//
//            cut_assert_operator_int(read(sockfd, receivedMessage, 512), >=, 0);
//            if (strlen(expectedMessagesFormat[test][subtest]))
//                sprintf(expectedMessage,
//                        expectedMessagesFormat[test][subtest],
//                        roomNumbers[test]);
//
//            cut_assert_equal_string(expectedMessage, receivedMessage);
        }

 //       close(sockfd);
   // }
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
//void test_irc_sendRoomMessage()
//{
//    cut_assert_operator_int(NO_ERROR, >, irc_sendRoomMessage("message"),
//                            "Send data to non-existent room succeeded");
//
//    currentRoom = 1;
//    int pid = cut_fork();
//    if (pid == 0) {
//        int serverSockfd = openLocalhostSocket(8200), returnedValue = 0;
//
//        char buffer[513];
//        memset(buffer, 0, 513);
//
//        read(serverSockfd, buffer, 513);
//        if (strcmp(buffer, "PRIVMSG #cruce-game001 :message") != 0)
//            returnedValue++;
//
//        close(serverSockfd);
//
//        sleep(1);
//
//        exit(returnedValue);
//    }
//
//    sleep(1);
//
//    cut_assert_equal_int(NO_ERROR, network_connect("localhost", 8200));
//    int value;
//
//    cut_assert_equal_int(NO_ERROR, irc_sendRoomMessage("message"),
//                         "Send data failed");
//    cut_assert_operator_int(NO_ERROR, >, irc_sendRoomMessage(NULL),
//                            "Send wrong data succeeded");
//
//    wait(&value);
//
//    cut_assert_equal_int(0, value);
//
//    cut_assert_equal_int(NO_ERROR, network_disconnect());
//
//    currentRoom = -1;
//}
//

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
//void test_irc_invite()
//{
//    cut_assert_not_equal_int(NO_ERROR, irc_invite("user"));
//
//    int pid = cut_fork();
//    if (pid == 0) {
//        cut_assert_equal_int(2, 3);
//        int serverSockfd = openLocalhostSocket(8201);
//        char userList[] =
//          ":weber.freenode.net 353 Paul1234 = #cruce-devel :user1 user2 user3\n"
//          ":weber.freenode.net 366 Paul1234 #cruce-devel :End of /NAMES list.";
//
//        char buffer[513];
//        int returnedValue = 0;
//
//        read(serverSockfd, buffer, 513);
//        if (strcmp(buffer, "INVITE user #cruce-game001") != 0)
//            returnedValue++;
//
//        close(serverSockfd);
//
//        sleep(1);
//
//        exit(returnedValue);
//    exit(1);
//    }

//    sleep(1);
//
//    cut_assert_equal_int(NO_ERROR, network_connect("localhost", 8201));
//    sleep(1);
//    currentRoom = 1;
//
//    int value;
//
//    cut_assert_equal_int(NO_ERROR, irc_invite("user"), "Invite user failed");
 //   cut_assert_operator_int(NO_ERROR, >, irc_invite(NULL));

 //   wait(&value);

 //   cut_assert_equal_int(0, value);

 //   cut_assert_equal_int(NO_ERROR, network_disconnect());

 //   currentRoom = -1;
//}

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
//void test_irc_getAvailableRooms()
//{
//    cut_assert_equal_pointer(NULL, irc_getAvailableRooms());
//
//    int pid = cut_fork();
//    if (pid == 0) {
//        int serverSockfd = openLocalhostSocket(8202), returnedValue = 0;
//
//        char buffer[513];
//        memset(buffer, 0, 513);
//
//        read(serverSockfd, buffer, 513);
//        if (strcmp(buffer, "LIST") != 0)
//            returnedValue++;
//        else
//            write(serverSockfd, "#cruce-game001 #cruce-game512", 29);
//
//        close(serverSockfd);
//
//        sleep(1);
//
//        exit(returnedValue);
//    }
//
//    sleep(1);
//
//    cut_assert_equal_int(NO_ERROR, network_connect("localhost", 8202));
//
//    int value;
//    char *rooms = irc_getAvailableRooms();
//
//    currentRoom = 2;
//    cut_assert_equal_pointer(NULL, irc_getAvailableRooms());
//    currentRoom = -1;
//
//    cut_assert_equal_string("#cruce-game001 #cruce-game512", rooms);
//
//    wait(&value);
//
//    cut_assert_equal_int(0, value);
//
//    cut_assert_equal_int(NO_ERROR, network_disconnect());
//}

