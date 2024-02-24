#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <string.h>
#include <errno.h>

#define DEVICE_FILE "/Users/lalovelazquez/projects/personal/device-file-simulator/fifo"

int main() {
    int fd;
    int flags;
    char buffer[4096];
    fd_set readfds;
    struct timeval timeout;

    // Open the device file
    fd = open(DEVICE_FILE, O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
        perror("Failed to open the device file");
        return EXIT_FAILURE;
    }

    // Get current file descriptor flags
    flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("Failed to get file descriptor flags");
        close(fd);
        return 1;
    }

    // Set file descriptor to non-blocking mode
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("Failed to set file descriptor to non-blocking mode");
        close(fd);
        return 1;
    }

    // Main loop
    while (1) {
        FD_ZERO(&readfds);
        FD_SET(fd, &readfds);

        // Set timeout (5 seconds)
        timeout.tv_sec = 60 * 60;
        timeout.tv_usec = 0;

        // Wait for the device to be ready for reading
        int retval;
        do {
            retval = select(fd + 1, &readfds, NULL, NULL, &timeout);
        } while (retval == -1 && errno == EINTR); // Retry if interrupted by signal

        if (retval == -1) {
            // Handle other select errors
            perror("select() failed");
            break;
        } else if (retval == 0) {
            // Handle timeout
            printf("Timeout, no data available.\n");
        } else {
            // Handle ready file descriptors
            if (FD_ISSET(fd, &readfds)) {
                // The file descriptor is ready for reading
                printf("Data is available to read.\n");
                // Your code to read from the fd goes here
                // Read data from the device
                ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
                if (bytes_read < 0) {
                    perror("read failed");
                    break;
                }

                if (bytes_read > 0) {
                    buffer[bytes_read] = '\0'; // Null-terminate the string
                    printf("Received: %s\n", buffer);
                }
            }
        }
    }

    // Cleanup
    close(fd);
    return EXIT_SUCCESS;
}
