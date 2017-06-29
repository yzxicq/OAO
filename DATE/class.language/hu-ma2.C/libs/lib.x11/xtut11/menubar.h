#ifndef XTUT_MENUBAR
#define XTUT_MENUBAR
typedef struct MenuBar MenuBar;
struct MenuBar {
	struct Funcs *funcs;
	Window helpButton; /* so we can keep it on the far right */
	int width;
	XContext ctxt;
};
#endif
extern struct Funcs *MenuBarFuncs;
/* returns the window */
Window newMenuBar(char *progname, Display *dpy, Window parent,
		XrmDatabase db, XContext ctxt, char * name, int x, int y, int width, int height);

/* returns the old helpButton */
Window setHelpButton(Display *dpy, XContext ctxt, Window mb, Window helpButton,
	int x);

