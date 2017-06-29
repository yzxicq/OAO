#include "funcs.h"
#include "menubar.h"

static void menubarConfigureNotify(Block *block, XEvent *ev){
	MenuBar *self = &block->menubar;
	Button *help = NULL;
	XConfigureEvent *xce = &ev->xconfigure;

	if (xce->width == self->width) return;
	self->width = xce->width;
	if (!self->helpButton) return;
	XFindContext(xce->display, self->helpButton, self->ctxt, (XPointer*)&help);
	if (!help) {
		printf("no find help button %p %p %p\n", self->helpButton, xce->display,
				self->ctxt);
		return; /* what width should we set it to ? */
	}

	XMoveWindow(xce->display, self->helpButton, xce->width - help->width, 0);
	/* fake a configure event  so the button can make any need mods immediately */
	resizeBlock((Block *)help, self->helpButton, help->width, xce->height, ev);
}

Window newMenuBar(char *progname, Display *dpy, Window parent,
		XrmDatabase db, XContext ctxt, char * name, int x, int y, int width, int height){
	Window win;
	MenuBar *menubar;
	char resourcename[256];
	char classname[256];
	unsigned long background;

	menubar = calloc(sizeof(*menubar), 1);
	if (!menubar){
		fprintf(stderr, "unable to allocate any space for %s, dieing\n", name);
		exit(32);
	}
	menubar->funcs = MenuBarFuncs;
	menubar->ctxt = ctxt;

	sprintf(resourcename, "%s.%s.background", progname, name);
	sprintf(classname, "%s.background", progname);
	background = getColour(dpy, db, resourcename, classname, "cornflowerblue");

	win = XCreateSimpleWindow(dpy, parent, x, y, width, height,
		0, 0, background); /* borderwidth, border and background colour */

	XSelectInput(dpy, win, StructureNotifyMask);

	XSaveContext(dpy, win, ctxt, (XPointer)menubar);
	XMapWindow(dpy, win);
	return win;
}

Window setHelpButton(Display *dpy, XContext ctxt, Window win, Window helpButton,
	int x){
	MenuBar *mb;
	Window old;
	XFindContext(dpy, win, ctxt, (XPointer*)&mb);
	old = mb->helpButton;
	mb->helpButton = helpButton;
	XMoveWindow(dpy, mb->helpButton, x, 0);
	return old;
}


static struct Funcs _MenuBarFuncs = {
	menubarConfigureNotify,
	NULL, /* leave */
	NULL, /* enter */
	NULL, /* expose */
	NULL, /* button press */
	NULL /* button release */
};
struct Funcs *MenuBarFuncs = &_MenuBarFuncs;
