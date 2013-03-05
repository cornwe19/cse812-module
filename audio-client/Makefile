CC = gcc

CFILES = pa_fuzz.c
CFLAGS = -Wall -o testaudio -I ./portaudio/include -arch x86_64

SHELL = /bin/sh
LIBTOOL = $(SHELL) ./portaudio/libtool

LIBS = ./portaudio/lib/libportaudio.la

all:
	$(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(CFILES) $(LIBS)

clean:
	rm -rf *.o testaudio
