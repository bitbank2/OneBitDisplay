CFLAGS=-c -g -Wall -O2 -I../src -D__LINUX__ -DSPI_BUS_NUMBER=0
LIBS = -g -lm -lpthread

all: libOneBitDisplay.a

libOneBitDisplay.a: OneBitDisplay.o ../src/OneBitDisplay.h ../src/linux_io.inl ../src/obd.inl
	ar -rc libOneBitDisplay.a OneBitDisplay.o ;\
	sudo cp libOneBitDisplay.a /usr/local/lib ;\
	sudo cp ../src/OneBitDisplay.h /usr/local/include

OneBitDisplay.o: ../src/obd.inl ../src/linux_io.inl ../src/OneBitDisplay.h ../src/OneBitDisplay.cpp
	$(CXX) $(CFLAGS) ../src/OneBitDisplay.cpp

clean:
	rm *.o libOneBitDisplay.a
