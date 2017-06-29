XDIR=/usr/X11R6
XINCLUDE=$(XDIR)/include
XLIBS=$(XDIR)/lib
CFLAGS=-Wall -ansi -Wstrict-prototypes -I$(XINCLUDE) -g
CC=gcc

xtut11: button.o getResources.o xc2b.o xtut11.o app.o menubar.o menu.o
	gcc -L$(XLIBS) -lX11 button.o getResources.o xc2b.o xtut11.o app.o menubar.o menu.o -o xtut11

app.o: app.h type.h common.h button.h menu.h menubar.h app.c
	gcc app.c -c $(CFLAGS)
menubar.o: app.h type.h common.h button.h menu.h menubar.h menubar.c
	gcc menubar.c -c $(CFLAGS)
menu.o: app.h type.h common.h button.h menu.h menubar.h menu.c
	gcc menu.c -c $(CFLAGS)
	
button.o: button.c button.h common.h
	gcc button.c -c $(CFLAGS)
xtut11.o: xtut11.c button.h common.h
	gcc xtut11.c -c $(CFLAGS)
xc2b.o: xc2b.c common.h
	gcc xc2b.c -c $(CFLAGS)
getResources.o: getResources.c common.h
	gcc getResources.c -c $(CFLAGS)

clean:
	rm -rf *.o xtut11
