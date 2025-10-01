CFLAGS=-c -Wall -O2 -DSPI_BUS_NUMBER=0 -D_LINUX_
LIBS = -lpthread -larmbianio
all: spi_demo

spi_demo: main.o
	$(CC) main.o $(LIBS) -o spi_demo 

main.o: main.c ../../src/obd.inl ../../src/OneBitDisplay.cpp ../../src/OneBitDisplay.h
	$(CC) $(CFLAGS) main.c

clean:
	rm -rf *.o spi_demo
