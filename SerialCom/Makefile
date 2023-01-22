CC = gcc
CFLAGS = -Wall -ggdb -lcurses


all:
	$(CC) -o bin/main src/main.c src/uart.c src/logfile.c $(CFLAGS)

run:
	./bin/main -p /dev/ttyACM0 -b 115200

clean:
	rm bin/main
