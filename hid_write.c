#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/select.h>
#include <time.h>

#define BUFFER_SIZE 20

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s /dev/hidrawX\n", argv[0]);
        return 1;
    }

    const char *device = argv[1];
    int fd = open(device, O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return 1;
    }

    unsigned char buffer[BUFFER_SIZE] = {
        0x11, 0xFF, 0x0B, 0x2A, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00,
        0xE0, 0x53, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    fd_set writefds;
    struct timeval timeout;

    while (1) {
        FD_ZERO(&writefds);
        FD_SET(fd, &writefds);

        timeout.tv_sec = 0;
        timeout.tv_usec = 250000;  // 250 ms

        int sel = select(fd + 1, NULL, &writefds, NULL, &timeout);
        if (sel > 0 && FD_ISSET(fd, &writefds)) {
             int res = write(fd, buffer, sizeof(buffer));
            if (res < 0) {
                perror("Failed to write to device");
                close(fd);
                return 1;
            }

            printf("Wrote %d bytes to the HID device\n", res);
        } else if (sel < 0) {
            perror("select() error");
            close(fd);
            return 1;
        }
    }

    close(fd);
    return 0;
}

