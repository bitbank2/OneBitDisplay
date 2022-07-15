CFLAGS=-c -Wall -O3 -D__LINUX__ -D_LINUX_

all: gifplay 

gifplay: main.o
	$(CC) main.o -o gifplay 

main.o: main.c ../../src/obd.inl ../../src/OneBitDisplay.h
	$(CC) $(CFLAGS) main.c

clean:
	rm -rf *.o gifplay
