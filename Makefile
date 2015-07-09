#
# Makefile --- offline tape (c) PMC 1999 - 2015
#

CC	= gcc
LIBDIR  = /code/include/
MAINDIR = /code/include/

CFLAGS = -I$(MAINDIR) -I$(LIBDIR) 

LIBS	= 

all:		tape single new-stuff

clean:          rm tape single new-stuff

tape:        	tape.c
	        $(CC) $(CFLAGS) -o $@ $<

single:		single.c
	        $(CC) $(CFLAGS) -o $@ $<

new-stuff:	new-stuff.c
		$(CC) $(CFLAGS) -o $@ $<
