all: imgconvert

CC     = gcc
CFLAGS = -Wall

imgconvert: main.c ../src/Group5.h ../src/g5enc.inl
	$(CC) $(CFLAGS) $< -o $@
	strip $@

clean:
	rm -f imgconvert
