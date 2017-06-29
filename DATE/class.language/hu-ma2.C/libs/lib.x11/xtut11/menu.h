#ifndef XTUT_MENU
#define XTUT_MENU
typedef struct Menu Menu;
struct Menu {
	struct Funcs *funcs;
	int width;
	int nsubws;
	XContext ctxt;
	Window *subws;
};
#endif
extern struct Funcs *MenuFuncs;

Window newMenu(char *progname, Display *dpy, XrmDatabase db,
	XContext ctxt, char *name);
Window *menuSetSubWins(Display *dpy, XContext ctxt, Window menu, Window *subws, int nsubws);
