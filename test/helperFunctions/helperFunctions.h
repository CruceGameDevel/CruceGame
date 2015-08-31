/**
 * This file implements helper functions for tests.
 */

#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

/**
 * @brief Open a local server socket and waits a client to connect.
 *
 * @param port The port on which must be created the server.
 *
 * @return A sockfd which represents a connection with a client..
 */
int openLocalhostSocket(int port);

#endif
