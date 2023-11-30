CC = gcc
CFLAGS = -Wall

SRC = src/

URL = ftp://anonymous:anonymous@ftp.up.pt/pub/kodi/timestamp.txt

.PHONY: downloader
downloader: $(SRC)/download.c
	$(CC) $(CFLAGS) -o download $^

.PHONY: clean
clean:
	rm -rf download
	rm -rf timestamp.txt

.PHONY: run
	# clean, compile and run the progam with this url ./download ftp://anonymous:anonymous@ftp.up.pt/pub/kodi/timestamp.txt
run: clean downloader
	./download $(URL)
