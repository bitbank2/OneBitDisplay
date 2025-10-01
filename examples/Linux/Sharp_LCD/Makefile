CFLAGS=-c -Wall -O2 -DSPI_BUS_NUMBER=0 -D_LINUX_
LIBS = -lpthread -larmbianio
all: sharp_lcd

sharp_lcd: main.o
	$(CC) main.o $(LIBS) -o sharp_lcd

main.o: main.c ../../src/obd.inl ../../src/OneBitDisplay.cpp ../../src/OneBitDisplay.h
	$(CC) $(CFLAGS) main.c

clean:
	rm -rf *.o sharp_lcd
