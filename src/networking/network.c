/**
 * @file This file provides the implementation of network module.
 */

#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errors.h>
#include "network.h"

int sockfd = -1;

int network_connect(char *hostname, int port)
{
    if (sockfd > 0)
        return CONNECTION_IN_USE;

    if (hostname == NULL)
        return POINTER_NULL;

    if (port < 0 || port > 65535)
        return PARAMETER_OUT_OF_RANGE;

    struct hostent *server = gethostbyname(hostname);
    if (server == NULL)
        return INVALID_HOSTNAME;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        return CONNECTING_ERROR;

    struct sockaddr_in serv_addr;

    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    memmove((char *)&serv_addr.sin_addr.s_addr, (char *)server->h_addr_list[0],
            server->h_length);

    serv_addr.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        close(sockfd);
        sockfd = -1;
        return CONNECTING_ERROR;
    }

    return NO_ERROR;
}

int network_send(void *data, size_t size)
{
    if (data == NULL)
        return POINTER_NULL;

    if (sockfd < 0)
        return UNINITIALIZED_CONNECTION;

    if (size == 0)
        return PARAMETER_OUT_OF_RANGE;

    if (write(sockfd, data, size) <= 0)
        return WRITING_ERROR;

    return NO_ERROR;
}

int network_read(void *buffer, size_t size)
{
    if (buffer == NULL)
        return POINTER_NULL;

    if (sockfd < 0)
        return UNINITIALIZED_CONNECTION;

    if (size == 0)
        return PARAMETER_OUT_OF_RANGE;

    int bytes_read = read(sockfd, buffer, size);

    if (bytes_read < 1)
        return READING_ERROR;

    return bytes_read;
}

int network_disconnect()
{
    if (sockfd < 0)
        return UNINITIALIZED_CONNECTION;
    else {
        close(sockfd);
        sockfd = -1;
        return NO_ERROR;
    }
}

int network_readLine(char *buffer, size_t size)
{
    if (sockfd < 0)
        return UNINITIALIZED_CONNECTION;

    if (buffer == NULL)
        return POINTER_NULL;

    // Cannot read a line longer than the buffer size.
    if (size == 0 || size > BUFFER_SIZE)
        return PARAMETER_OUT_OF_RANGE;

    // Create a temporary buffer and a buffer count.
    static char internalBuffer[BUFFER_SIZE];
    static int bufferedBytes = 0;

    // Use this to check if the internal buffer already contains a line.
    char *newline = strchr(internalBuffer, '\n');

    if (size > bufferedBytes + 1 || newline == NULL ||
        newline - buffer >= size ) {

        int bytesRead = network_read(internalBuffer + bufferedBytes,
                                     size - 1 - bufferedBytes);

        // Even if the read failed, there may be buffered bytes that can be
        // used (even an entire line that can be returned).
        if (bytesRead < 1 && bufferedBytes == 0)
            return READING_ERROR;

        if (bytesRead > 0)
            bufferedBytes += bytesRead;
    }

    // Iterate over the internalBuffer elements and copy them to the parameter
    // buffer until current character is a new line or the size of the
    // parameter buffer has been exceeded.
    int bufferIndex = 0;
    while (internalBuffer[bufferIndex] != '\n' && bufferIndex < size - 1) {
        buffer[bufferIndex] = internalBuffer[bufferIndex];
        bufferIndex++;
    }

    // Copy the newline if a full line does fit in the external buffer.
    // Return an error code if there is not a full line in the internalBuffer
    // and neither enough data to fill up the external buffer.
    if (bufferedBytes > bufferIndex && internalBuffer[bufferIndex] == '\n') {
        buffer[bufferIndex] = internalBuffer[bufferIndex];
        bufferIndex++;
    } else if (bufferIndex < size - 1)
        return DATA_NOT_READY;

    bufferedBytes -= bufferIndex;

    // Set last character to be a NUL.
    buffer[bufferIndex] = '\0';

    if (bufferedBytes > 0)
        strncpy(internalBuffer, internalBuffer + bufferIndex, bufferedBytes);

    return bufferIndex;
}

/**
 * This function is a wrapper for select function from C library,
 * it checks if there is any data incoming and return 1 if it is,
 * else return 0.
 */
int network_checkForData()
{
    // Prepare structures.
    fd_set rfds;
    struct timeval tv;

    // Set to 0.
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);

    // Set waiting time.
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    // If socket is not initialized, return error;
    if (sockfd < 0) {
        return UNINITIALIZED_CONNECTION;
    }

    // Wait and see if there is any data.
    int retval = select(sockfd, &rfds, NULL, NULL, &tv);

    if (retval) {
        return 1;
    } else {
        return 0;
    }
}
