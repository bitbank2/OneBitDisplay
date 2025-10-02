CFLAGS=-c -g -Wall -O2 -D__LINUX__ -DSPI_BUS_NUMBER=0
LFLAGS= -g -lOneBitDisplay -lgpiod -pthread
all: oled_demo

oled_demo: main.o
	$(CC) main.o $(LFLAGS) -o oled_demo 

main.o: main.cpp ../../../src/obd.inl ../../../src/OneBitDisplay.cpp ../../../src/OneBitDisplay.h
	$(CXX) $(CFLAGS) main.cpp

clean:
	rm -rf *.o oled_demo
