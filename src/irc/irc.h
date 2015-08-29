/**
 * @file This module implements all the irc operations.
 *       It is the only module which has access to the network module.
 *
 *       The concepts used by this module are:
 *          - lobby       : an irc channel that is joined automatically when
 *                          the game starts, used as a chat.
 *          - room        : an irc channel where a game is played.
 *                          Any user can join at most one room.
 *          - room status : it is represented by the channel's topic,
 *                          that can be PLAYING or WAITING
 *          - invitation  : any user can be invited to join a room.
 *
 *       This module implements an event driven interface for the events
 *       JOIN, QUIT, NOTICE, INVITE, PRIVMSG.
 *
 * TODO: add handler register/unregister methods.
 */

#ifndef IRC_H
#define IRC_H

/**
 * @brief Name of the IRC server for the game.
 */
#define IRC_SERVER "irc.freenode.net"

/**
 * @brief Port number for connection to IRC.
 */
#define IRC_PORT 6667

/**
 * @brief Name of lobby channel.
 */
#define LOBBY_CHANNEL "#cruce-devel"

/**
 * @brief String format for room names.
 */
#define ROOM_FORMAT "#cruce-game%03d"

/**
 * @brief Minimum size for strings with IRC commands.
 */
#define COMMAND_SIZE 30

/**
 * @brief Connect to the IRC server using 'network_connect' and
 *        send necessary data for connection respecting IRC protocol:
 *        1. PASS *  (the lobby doesn't have a password, just a convention).
 *        2. NICK <name> (set the nick of the player).
 *        3. USER <name> <mode> :<name> (set the user of the player).
 *        4. JOIN <channel> (join to the lobby channel).
 *        ALL COMMAND STRINGS MUST BE TERMINATED IN "\r\n".
 *
 * @param name The name of the user that wants to connect.
 *
 * @return 0 on success, other value on failure.
 */
int irc_connect(char *name);

/**
 * @brief Join a room. AT MOST ONE ROOM CAN BE JOINED AT ANY TIME.
 *
 * @return 0 on success, other value on failure.
 */
void irc_joinRoom();

/**
 * @brief Leave the room.
 *
 * @return 0 on success, other value on failure.
 */
void irc_leaveRoom();

/**
 * @brief Send QUIT command to the IRC server and close the connection.
 *
 * @return 0 on success, other value on failure.
 */
int irc_disconnect();

/**
 * @brief Create a new room and join it.
 *
 * @return Room id on success, negative value on failure.
 */
int irc_createRoom(char *channelName);

/**
 * @brief Toggle the status of the current room.
 *
 * @return 0 on success, other value on failure.
 */
int irc_toggleRoomStatus();

/**
 * @brief Get all users from the lobby or from room.
 *
 * @param isRoom If 1, the users from the current room are returned.
 *               Otherwise, the users from the lobby are returned.
 *
 * @return Pointer to a string containing the nicknames of all users in the
 *         lobby or in the room. The nicknames are separated by a space.
 *         MUST BE FREED.
 *         On failure, NULL is returned.
 */
char *irc_getNames(int isRoom);

/**
 * @brief List all available rooms. Cannot be used if already joined in a room.
 *
 * @return A string containing all the available rooms on success,
 *         NULL on failure.
 */
char *irc_getAvailableRooms();

/**
 * @brief Invite to room. Cannot be used if not joined in a room.
 *
 * @param nickname The nickname of the user to be invited.
 *
 * @return 0 on success, other value on failure.
 */
int irc_invite(char *nickname);

/**
 * @brief Send an IRC message to the lobby.
 *
 * @param message The message to be sent. Must be NUL terminated.
 *
 * @return 0 on success, other value on failure.
 */
int irc_sendLobbyMessage(char *message);

/**
 * @brief Send an IRC message to the room.
 *
 * @param message The message to be sent. Must be NUL terminated.
 *
 * @return 0 on success, other value on failure.
 */
int irc_sendRoomMessage(char *message);

/**
 * @brief Read a new message and handle it.
 */
void irc_handleNextMessage();

#endif
