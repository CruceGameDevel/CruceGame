/**
 * @file Module that implements the network operations.
 */

#ifndef NETWORK_H
#define NETWORK_H

/**
 * @brief Issued when you are trying to establish a connection when there is
 *        already an active one.
 */
#define ERROR_SOCKET_IN_USE -1

/**
 * @brief Issued when you are trying to establish a connection but the hostname
 *        is invalid.
 */
#define ERROR_INVALID_HOST_NAME -2

/**
 * @brief Issued when the internal call to `connect` returned an error status.
 */
#define ERROR_CONNECTING_TO_SERVER -3

/**
 * @brief Issued when the function finished the job with no error.
 */
#define INFO_ACTION_COMPLETED_SUCCESSFULLY 0

/**
 * @brief You are trying to send/read from an uninitialized socket.
 */
#define ERROR_UNINITIALIZED_SOCKET -4

/**
 * @brief Issued when the internal call to `write` in `network_send` returns
 *        an error status.
 */
#define ERROR_WRITING_TO_SERVER -5

/**
 * @brief Issued when the internal call to `read` in the function 'network_read'
 *        returns an error status.
 */
#define ERROR_READING_FROM_SERVER -6

/**
 * @brief Initialize a network connection.
 *
 * @param hostname The hostname to connect to.
 * @param port The port to use.
 *
 * @return none
 */
void network_connect(char *hostname, int port);

/**
 * @brief Send a message over the network.
 *        No assumption is made about the message's format or content.
 *
 * @param data Pointer to the data to be sent.
 * @param size The number of bytes to be sent.
 *
 * @return none
 */
int network_send(void *data, size_t size);

/**
 * @brief Read a message from the network.
 *
 * @param buffer Pointer to a memory region where the message read is going to
 *               be stored. The content of this memory region may be
 *               overwritten.
 * @param size The maximum number of bytes to read.
 *
 * @return The actual number of bytes read.
 */
int network_read(void *buffer, size_t size);

/**
 * @brief End a network connection.
 *
 * @return none
 */
void network_disconnect();

#endif // end of include guard: NETWORK_H

