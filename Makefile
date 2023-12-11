CC = gcc
CFLAGS = -Wall

SRC = src/

# First URL
# URL = ftp://anonymous:anonymous@ftp.up.pt/pub/kodi/timestamp.txt
URL = ftp://ftp.up.pt/pub/kodi/timestamp.txt

# Second URL
# URL = ftp://rcom:rcom@netlab1.fe.up.pt/pipe.txt
# URL = ftp://netlab1.fe.up.pt/pipe.txt

# URL = ftp://rcom:rcom@netlab1.fe.up.pt/files/crab.mp4
# URL = ftp://netlab1.fe.up.pt/files/crab.mp4


.PHONY: download
download: $(SRC)/download.c
	$(CC) $(CFLAGS) -o download $^

.PHONY: clean
clean:
	rm -rf download
	rm -rf timestamp.txt
	rm -rf pipe.txt

.PHONY: run
	# clean, compile and run the program with this url ./download ftp://anonymous:anonymous@ftp.up.pt/pub/kodi/timestamp.txt
run: clean download
	./download $(URL)
