XDIR=/usr/X11R6
XINCLUDE=$(XDIR)/include
XLIBS=$(XDIR)/lib

xtut10: button.o getResources.o xc2b.o xtut10.o
	gcc -L$(XLIBS) -lX11 button.o getResources.o xc2b.o xtut10.o -o xtut10 

button.o: button.c button.h common.h
	gcc button.c -c -Wall -ansi -Wstrict-prototypes -I$(XINCLUDE)
xtut10.o: xtut10.c button.h common.h
	gcc xtut10.c -c -Wall -ansi -Wstrict-prototypes -I$(XINCLUDE)
xc2b.o: xc2b.c common.h
	gcc xc2b.c -c -Wall -ansi -Wstrict-prototypes -I$(XINCLUDE)
getResources.o: getResources.c common.h
	gcc getResources.c -c -Wall -Wstrict-prototypes -I$(XINCLUDE)

clean:
	rm -rf *.o xtut10
