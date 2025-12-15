CFLAGS=-D__LINUX__ -Wall -O2 -I../../
LIBS=-lOneBitDisplay -lgpiod 

all: hello_cpp

hello_cpp: main.o
	$(CXX) main.o $(LIBS) -o hello_cpp

main.o: main.cpp
	$(CXX) $(CFLAGS) -c main.cpp

clean:
	rm -rf *.o hello_cpp
