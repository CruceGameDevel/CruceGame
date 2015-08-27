/**
 * @file Module that implements the network operations.
 */

#ifndef NETWORK_H
#define NETWORK_H

/**
 * @brief Issued when you are trying to establish a connection when there is
 *        already an active one.
 */
#define SOCKET_IN_USE -1

/**
 * @brief Issued when you are trying to establish a connection but the hostname
 *        is invalid.
 */
#define INVALID_HOSTNAME -2

/**
 * @brief Issued when the internal call to `connect` returned an error status.
 */
#define CONNECTING_ERROR -3

/**
 * @brief Issued when the function finished the job with no error.
 */
#define NO_ERROR 0

/**
 * @brief You are trying to send/read from an uninitialized socket.
 */
#define UNINITIALIZED_SOCKET -4

/**
 * @brief Issued when the internal call to `write` in `network_send` returns
 *        an error status.
 */
#define WRITING_ERROR -5

/**
 * @brief Issued when the internal call to `read` in the function 'network_read'
 *        returns an error status.
 */
#define READING_ERROR -6

/**
 * @brief Initialize a network connection.
 *
 * @param hostname The hostname to connect to.
 * @param port The port to use.
 *
 * @return NO_ERROR, if the connection succeeded;
 *         INVALID_HOSTNAME, if the hostname is invalid;
 *         CONNECTING_ERROR, if the call to api connect failed.
 */
int network_connect(char *hostname, int port);

/**
 * @brief Send a message over the network.
 *        No assumption is made about the message's format or content.
 *
 * @param data Pointer to the data to be sent.
 * @param size The number of bytes to be sent.
 *
 * @return NO_ERROR, if the connection succeeded;
 *         UNINITIALIZED_SOCKET, if the connection has not been established;
 *         WRITING_ERROR, if the internal write call failed.
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
 * @return The number of bytes read (positive value),
 *              if the connection succeeded;
 *         UNINITIALIZED_SOCKET, if the connection has not been established;
 *         READING_ERROR, if the internal read call failed.
 */
int network_read(void *buffer, size_t size);

/**
 * @brief End a network connection.
 *
 * @return NO_ERROR, if the operation succeeded;
 *         UNINITIALIZED_SOCKET, if there is no connection to close.
 */
int network_disconnect();

#endif // end of include guard: NETWORK_H

