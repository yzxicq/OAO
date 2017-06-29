#include "funcs.h"
#include "app.h"

Window newApp(char *progname, Display *dpy, XrmDatabase db, XContext ctxt,
	int	width, int height, int argc, char ** argv){
	App *mainwindow;
	char resourcename[256];
	char * temp;
	Window win;

	mainwindow = calloc(sizeof(*mainwindow), 1);
	if (!mainwindow){
		fprintf(stderr, "can't allocate space for the main window\n");
		exit(3);
	}
	mainwindow->funcs = AppFuncs;
	sprintf(resourcename, "%s.background", progname);
	mainwindow->background = getColour(dpy,  db, resourcename, resourcename, "DarkGreen");
	sprintf(resourcename, "%s.border", progname);
	mainwindow->border = getColour(dpy,  db, resourcename, resourcename, "LightGreen");


	sprintf(resourcename, "%s.width", progname);
	temp = getResource(dpy, db, resourcename, resourcename, "400");
	mainwindow->width = 400;
	free(temp);
	sprintf(resourcename, "%s.height", progname);
	temp = getResource(dpy, db, resourcename, resourcename, "400");
	mainwindow->height = 400;
	free(temp);

	win = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), /* display, parent */
		0,0, /* x, y: the window manager will place the window elsewhere */
		mainwindow->width, mainwindow->height, /* width, height */
		2, mainwindow->border, /* border width & colour, unless you have a window manager */
		mainwindow->background); /* background colour */
	/* tell the display server what kind of events we would like to see */
	XSelectInput(dpy, win, StructureNotifyMask|ExposureMask);

	mainwindow->ctxt = ctxt;

	/* save the useful information about the window */
	XSaveContext(dpy, win, ctxt, (XPointer)mainwindow);

	Xutf8SetWMProperties(dpy, win, progname, progname, argv, argc,
		NULL, NULL, NULL);
	return win;
}

static void appConfigureNotify(Block *block, XEvent *ev){
	App *self = &block->app;
	XConfigureEvent *xce = &ev->xconfigure;
	MenuBar *menubar;
	if (self->height != xce->height){
		/* grab the new height before it's overwritten */
		self->height = xce->height;
	}
	if (self->width == xce->width) return;
	self->width = xce->width;

	if (!self->menubar) return;
	if (XFindContext(xce->display, self->menubar, self->ctxt, (XPointer*)&menubar)) 
		return;
	/* tell menubar it changed */
	/* fake a configure event  so the menubar can make any need mods immediately */
	resizeBlock((Block*)menubar, self->menubar, self->width, self->menubarHeight, ev);
}

/* returns the old menubar */
Window setAppMenuBar(Display *dpy, XContext ctxt, Window w, Window menubar,
		int width, int menubarHeight){
	App *app;
	Window old;
	XFindContext(dpy, w, ctxt, (XPointer *)&app);
	old = app->menubar;
	app->menubar = menubar;
	app->menubarHeight = menubarHeight;
	XResizeWindow(dpy, menubar, width, menubarHeight);
	return old;
}

static struct Funcs _AppFuncs = {
	appConfigureNotify,
	NULL, /* leave */
	NULL, /* enter */
	NULL, /* expose */
	NULL, /* button press */
	NULL /* button release */
};
struct Funcs *AppFuncs = &_AppFuncs;
