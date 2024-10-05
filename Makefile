CC = gcc
CFLAGS = -Wall -O2
TARGETS = hid_write hid_read
SOURCES = hid_write.c hid_read.c
OBJECTS = $(SOURCES:.c=.o)
all: $(TARGETS)

hidwrite: hid_write.o
	$(CC) $(CFLAGS) -o $@ $^

hidread: hid_read.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGETS)

.PHONY: all clean

