
#ifndef XTUT_APP
#define XTUT_APP
typedef struct App App;
struct App {
	struct Funcs *funcs;
	Window menubar; /* so we can keep it the width of the app window */
	int menubarHeight;

	XContext ctxt;
	int width, height;
	unsigned long border, background, foreground;
};
#endif

extern struct Funcs *AppFuncs;
Window newApp(char *progname, Display *dpy, XrmDatabase db, XContext ctxt,
	int	width, int height, int argc, char ** argv);
Window setAppMenuBar(Display *dpy, XContext ctxt, Window w, Window menubar,
		int width, int menubarHeight);
