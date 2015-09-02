/**
 * @file Module that implements the network operations.
 */

#ifndef NETWORK_H
#define NETWORK_H

#include <stddef.h>

/**
 * @brief Issued when the function finished the job with no error.
 */
#define NO_ERROR 0

/**
 * @brief Issued when you are trying to establish a connection when there is
 *        already an active one.
 */
#define CONNECTION_IN_USE -1

/**
 * @brief Issued when you are trying to establish a connection but the hostname
 *        is invalid.
 */
#define INVALID_HOSTNAME -2

/**
 * @brief Issued when the connection attempt failed.
 */
#define CONNECTING_ERROR -3

/**
 * @brief You are trying to send/read from an uninitialized socket.
 */
#define UNINITIALIZED_CONNECTION -4

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
 * @brief Issued when a parameter is null.
 */
#define NULL_PARAMETER -7

/**
 * @brief Issued when a parameter is out of range.
 */
#define PARAMETER_OUT_OF_RANGE -8

/**
 * @brief Initialize a network connection.
 *
 * @param hostname The hostname to connect to.
 * @param port The port to use.
 *
 * @return NO_ERROR, if the connection succeeded;
 *         CONNECTION_IN_USE, if there is already a connection initialised;
 *         NULL_PARAMETER, if the hostname is NULL;
 *         INVALID_HOSTNAME, if the hostname is invalid;
 *         PARAMETER_OUT_OF_RANGE, if the port value is out of range 0-65535;
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
 * @return NO_ERROR, if the network send succeeded;
 *         UNINITIALIZED_CONNECTION, if the connection has not been established;
 *         PARAMETER_OUT_OF_RANGE, if the size is 0;
 *         NULL_PARAMETER, if the data pointer is NULL;
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
 *         UNINITIALIZED_CONNECTION, if the connection has not been established;
 *         PARAMETER_OUT_OF_RANGE, if the size is 0;
 *         NULL_PARAMETER, if the buffer pointer is NULL;
 *         READING_ERROR, if the internal read call failed.
 */
int network_read(void *buffer, size_t size);

/**
 * @brief Read one line (a string of characters up to the first occurence of
 *        /n) from the network connection. It is meant to be used for line
 *        based protocols, such as IRC or HTTP. The newline character is
 *        replaced with a NUL character in the returned buffer.
 *
 * @param buffer Pointer to a memory region where the line will be stored.
 *               The buffer is NUL terminated and includes the newline
 *               character, if one has been found.
 * @param size The maximum number of characters to read, including the NUL
 *             character.
 */
int network_readLine(char *buffer, size_t size);

/**
 * @brief End a network connection.
 *
 * @return NO_ERROR, if the operation succeeded;
 *         UNINITIALIZED_SOCKET, if there is no connection to close.
 */
int network_disconnect();

#endif // end of include guard: NETWORK_H

