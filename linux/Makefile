CFLAGS=-c -Wall -O2 -I../src -DSPI_BUS_NUMBER=0
LIBS = -lm -lpthread

all: libOneBitDisplay.a

libOneBitDisplay.a: obd.o ../src/OneBitDisplay.h
	ar -rc libOneBitDisplay.a obd.o ;\
	sudo cp libOneBitDisplay.a /usr/local/lib ;\
	sudo cp ../src/OneBitDisplay.h /usr/local/include

obd.o: obd.c ../src/obd.inl ../src/OneBitDisplay.h
	$(CC) $(CFLAGS) -D_LINUX_ obd.c

clean:
	rm *.o libOneBitDisplay.a
