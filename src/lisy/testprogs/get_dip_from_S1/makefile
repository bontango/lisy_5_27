# lisy Makefile for mpfserver
# RTH 06.2019

CC      = /usr/bin/gcc
CFLAGS  = -g -Wunused-variable
LDFLAGS = -lwiringPi -lSDL2 -lSDL2_mixer
LOP = /home/pi//lisy/xpinmame.obj/lisy/

OBJ = $(LOP)lisy80.o \
      $(LOP)lisy1.o \
      $(LOP)lisy35.o \
      $(LOP)lisy.o \
      $(LOP)fileio.o \
      $(LOP)utils.o \
      $(LOP)hw_lib.o \
      $(LOP)switches.o \
      $(LOP)displays.o \
      $(LOP)coils.o \
      $(LOP)eeprom.o \
      $(LOP)sound.o \
      $(LOP)opc.o \
      $(LOP)fadecandy.o \
      $(LOP)usbserial.o \
      $(LOP)lisy_home.o \
      $(LOP)lisy_w.o \
      get_dip_from_S1.o

get_dip_from_S1: $(OBJ)
	$(CC) $(CFLAGS) -o get_dip_from_S1 $(OBJ) $(LDFLAGS)

%.o: ../%.c ../%.h
	$(CC) $(CFLAGS) -c $<

