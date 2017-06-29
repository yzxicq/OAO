#include "funcs.h"
#include "button.h"


typedef struct exitInfo ExitInfo;
struct exitInfo {
	Display *dpy;
};

void exitButton(void *cbdata, XEvent *ev){
	ExitInfo *ei = (ExitInfo*)cbdata;
	XCloseDisplay(ei->dpy);
	exit(0);
}

typedef struct HelpInfo {
	Window menuToClose;
} HelpInfo;

void helpButtonCB(void *cbdata, XEvent *ev){
	HelpInfo *hi = (HelpInfo*)cbdata;
	printf("you pressed help!\n");
	XUnmapWindow(ev->xany.display, hi->menuToClose);
}

typedef struct MenuBarButton {
	Display *dpy;
	Window toshow;
} MenuBarButton;

void menuBarButton(void *cbdata, XEvent *ev){
	MenuBarButton *mbb = (MenuBarButton*)cbdata;
	if (ev->type == ButtonRelease)
	XMoveWindow(mbb->dpy, mbb->toshow, ev->xbutton.x_root, ev->xbutton.y_root);
	XMapWindow(mbb->dpy, mbb->toshow);
}

XContext setup(Display * dpy, int argc, char ** argv){
	static XrmOptionDescRec xrmTable[] = {
		{"-bg", "*background", XrmoptionSepArg, NULL},
		{"-fg", "*foreground", XrmoptionSepArg, NULL},
		{"-bc", "*bordercolour", XrmoptionSepArg, NULL},
		{"-font", "*font", XrmoptionSepArg, NULL},
	};
	Window win;
	XrmDatabase db;
	XContext ctxt;

	ctxt = XUniqueContext();

	db = setupDB(dpy, xrmTable, sizeof(xrmTable)/sizeof(xrmTable[0]),
			"xtut11", &argc, argv);

	win = newApp("xtut11", dpy, db, ctxt, 400, 400, argc, argv);

	{
	ExitInfo *exitInfo;
	HelpInfo *helpInfo;
	MenuBarButton *fmbbInfo, *hmbbInfo;
	Window *filemenuwins, *helpmenuwins;
	Window filemenu, helpmenu;
	Window menubar, helpButton;

	exitInfo = malloc(sizeof(*exitInfo));
	exitInfo->dpy = dpy;

	helpInfo = malloc(sizeof(*helpInfo));


	filemenu = newMenu("xtut11", dpy,  db, ctxt, "File");

	filemenuwins = malloc(sizeof(*filemenuwins));
	filemenuwins[0] = newButton("xtut11", dpy, filemenu, db, ctxt, "exit",
					 0, 0, 80, 0, exitButton, exitInfo); 
	menuSetSubWins(dpy, ctxt, filemenu, filemenuwins, 1);

	fmbbInfo = malloc(sizeof(*fmbbInfo));
	fmbbInfo->dpy = dpy;
	fmbbInfo->toshow = filemenu;

	helpmenu = newMenu("xtut11", dpy,  db, ctxt, "Help");
	helpInfo->menuToClose = helpmenu;
	helpmenuwins = malloc(sizeof(*helpmenuwins));
	helpmenuwins[0] = newButton("xtut11", dpy, helpmenu, db, ctxt, "help",
					 0, 0, 80, 0, helpButtonCB, helpInfo); 
	menuSetSubWins(dpy, ctxt, helpmenu, helpmenuwins, 1);
	hmbbInfo = malloc(sizeof(*hmbbInfo));
	hmbbInfo->dpy = dpy;
	hmbbInfo->toshow = helpmenu;

	menubar = newMenuBar("xtut11", dpy, win, db, ctxt, "menubar", 0, 0, 400, 20);
	newButton("xtut11", dpy, menubar, db, ctxt, "filemenu", 0, 0, 30, 0,
		menuBarButton, fmbbInfo);
	helpButton = newButton("xtut11", dpy, menubar, db, ctxt, "helpmenu", 0, 0, 30, 0,
		menuBarButton, hmbbInfo);
	setHelpButton(dpy, ctxt, menubar, helpButton, 400-80);
	setAppMenuBar(dpy, ctxt, win, menubar, 400, 20);
	}

	/* okay, put the window on the screen, please */
	XMapWindow(dpy, win);

	return ctxt;
}



int main_loop(Display *dpy, XContext context){
	XEvent ev;

	/* as each event that we asked about occurs, we respond. */
	while(1){
		Block *block = NULL;
		XNextEvent(dpy, &ev);
		XFindContext(ev.xany.display, ev.xany.window, context, (XPointer*)&block);
		switch(ev.type){
		case ConfigureNotify:
			if (block && block->funcs && block->funcs->configureNotify)
				block->funcs->configureNotify(block, &ev);
			break;
		case Expose:
			if (block && block->funcs && block->funcs->expose)
				block->funcs->expose(block, &ev);
			break;

		case EnterNotify:
			if (block && block->funcs && block->funcs->enterNotify)
				block->funcs->enterNotify(block, &ev);
			break;
		case LeaveNotify:
			if (block && block->funcs && block->funcs->leaveNotify)
				block->funcs->leaveNotify(block, &ev);
			break;
		case ButtonPress:
			if (block && block->funcs && block->funcs->buttonPress)
				block->funcs->buttonPress(block, &ev);
			break;
		case ButtonRelease:
			if (block && block->funcs && block->funcs->buttonRelease)
				block->funcs->buttonRelease(block, &ev);
			break;
		}
	}
}

int main(int argc, char ** argv){
	Display *dpy;
	XContext ctxt;

	/* First connect to the display server */
	dpy = XOpenDisplay(NULL);
	if (!dpy) {fprintf(stderr, "unable to connect to display\n");return 7;}
	ctxt = setup(dpy, argc, argv);
	return main_loop(dpy, ctxt);
}
