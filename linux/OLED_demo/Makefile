CFLAGS=-c -Wall -O3 -D_LINUX_

all: oled_demo

oled_demo: main.o
	$(CC) -lpthread main.o -o oled_demo 

main.o: main.c ../../src/obd.inl ../../src/OneBitDisplay.cpp ../../src/OneBitDisplay.h
	$(CC) $(CFLAGS) main.c

clean:
	rm -rf *.o oled_demo
