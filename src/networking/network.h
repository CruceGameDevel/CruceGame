/**
 * @file Module that implements the network operations.
 */

#ifndef NETWORK_H
#define NETWORK_H

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

