CFLAGS=-c -Wall -O2 -DSPI_BUS_NUMBER=0 -D_LINUX_
LIBS = -larmbianio -lOneBitDisplay -pthread
all: epd_demo

epd_demo: main.o
	$(CC) main.o $(LIBS) -o epd_demo 

main.o: main.c
	$(CC) $(CFLAGS) main.c

clean:
	rm -rf *.o epd_demo
