CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = ssdag_engine

all: $(TARGET)

$(TARGET): main.o SSDAG.o
	$(CC) $(CFLAGS) -o $(TARGET) main.o SSDAG.o

main.o: main.c SSDAG.h
	$(CC) $(CFLAGS) -c main.c

SSDAG.o: SSDAG.c SSDAG.h
	$(CC) $(CFLAGS) -c SSDAG.c

clean:
	rm -f *.o $(TARGET)
