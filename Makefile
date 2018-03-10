CC=gcc
CFLAGS=-Wall

all : clock

clock : led_clock.o 
	gcc -o clock arduino-serial-lib.c led_clock.o

led_clock.o : arduino-serial-lib.c led_clock.c

clean : 
	rm led_clock.o	
