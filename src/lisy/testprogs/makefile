# for switch tester lisy mini

CC      = /usr/bin/gcc
CFLAGS  = -Wall -g -D_REENTRANT -DDEBUG
LDFLAGS = -lwiringPi

OBJ = miniswitches.o

SW_Test: $(OBJ)
	$(CC) $(CFLAGS) -o miniswitches $(OBJ) $(LDFLAGS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm *.o miniswitches
