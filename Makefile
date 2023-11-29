CC = gcc
CFLAGS = -Wall

SRC = src/

.PHONY: downloader
downloader: $(SRC)/download.c
	$(CC) $(CFLAGS) -o download $^

.PHONY: clean
clean:
	rm -rf download