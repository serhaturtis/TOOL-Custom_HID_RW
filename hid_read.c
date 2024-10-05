#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/select.h>

#define BUFFER_SIZE 64

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s /dev/hidrawX\n", argv[0]);
        return 1;
    }

    const char *device = argv[1];
    int fd = open(device, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open device");
        return 1;
    }

    unsigned char buffer[BUFFER_SIZE];
    int res;

    fd_set readfds;
    struct timeval timeout;

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(fd, &readfds);

        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        int sel = select(fd + 1, &readfds, NULL, NULL, &timeout);
        if (sel > 0 && FD_ISSET(fd, &readfds)) {
            res = read(fd, buffer, sizeof(buffer));
            if (res < 0) {
                perror("Failed to read from device");
                close(fd);
                return 1;
            }

            printf("Received %d bytes: ", res);
            for (int i = 0; i < res; i++) {
                printf("%02X ", buffer[i]);
            }
            printf("\n");

        } else if (sel < 0) {
            perror("select() error");
            close(fd);
            return 1;
        }
    }

    close(fd);
    return 0;
}

