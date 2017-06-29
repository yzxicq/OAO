#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DefGC(dpy) DefaultGC(dpy, DefaultScreen(dpy))

XrmDatabase setupDB(Display *dpy,
			XrmOptionDescRec *xrmTable, int nCommandLineResources,
			const char *progname, int *argc, char **argv);
char *getResource(Display *dpy, XrmDatabase db, char *name, char *cl, char *def);
unsigned long getColour(Display *dpy,  XrmDatabase db, char *name,
			char *cl, char *def);
XFontStruct *getFont(Display *dpy, XrmDatabase db, char *name,
		char *cl, char *def);
int XChar2bLen(XChar2b *string);
int utf8toXChar2b(XChar2b *output_r, int outsize, const char *input, int inlen);

