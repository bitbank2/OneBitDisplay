CFLAGS=-c -Wall -O2 -DSPI_BUS_NUMBER=0 -D_LINUX_
LIBS = -lpthread -larmbianio
all: epd_demo

epd_demo: main.o OneBitDisplay.o
	g++ main.o OneBitDisplay.o $(LIBS) -o epd_demo 

main.o: main.cpp ../../src/obd.inl ../../src/OneBitDisplay.cpp ../../src/OneBitDisplay.h
	g++ $(CFLAGS) main.cpp

OneBitDisplay.o: ../../src/OneBitDisplay.cpp ../../src/OneBitDisplay.h ../../src/obd.inl
	g++ $(CFLAGS) ../../src/OneBitDisplay.cpp

clean:
	rm -rf *.o epd_demo
