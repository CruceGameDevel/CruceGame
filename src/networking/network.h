/**
 * @file Module that implements the network operations.
 */

#ifndef NETWORK_H
#define NETWORK_H

#include <stddef.h>

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
 * @brief End a network connection.
 *
 * @return NO_ERROR, if the operation succeeded;
 *         UNINITIALIZED_SOCKET, if there is no connection to close.
 */
int network_disconnect();

#endif // end of include guard: NETWORK_H

