CFLAGS=-c -Wall -O3 -D_LINUX_

all: obd_test 

obd_test: main.o
	$(CC) main.o -o obd_test 

main.o: main.c ../../src/obd.inl ../../src/OneBitDisplay.h
	$(CC) $(CFLAGS) main.c

clean:
	rm -rf *.o obd_test
