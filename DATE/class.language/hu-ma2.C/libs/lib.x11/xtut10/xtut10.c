#include "common.h"
#include "button.h"


typedef struct exitInfo ExitInfo;
struct exitInfo {
	Display *dpy;
	XFontStruct *font;
};

void exitButton(void *cbdata){
	ExitInfo *ei = (ExitInfo*)cbdata;
	XFreeFont(ei->dpy, ei->font);
	XCloseDisplay(ei->dpy);
	exit(0);
}

XContext setup(Display * dpy, int argc, char ** argv){
	static XrmOptionDescRec xrmTable[] = {
		{"-bg", "*background", XrmoptionSepArg, NULL},
		{"-fg", "*foreground", XrmoptionSepArg, NULL},
		{"-bc", "*bordercolour", XrmoptionSepArg, NULL},
		{"-font", "*font", XrmoptionSepArg, NULL},
	};
	Button *mainwindow;
	Window win;
	XGCValues values;

	XFontStruct * font;
	XrmDatabase db;

	XContext ctxt;

	ctxt = XUniqueContext();

	mainwindow = calloc(sizeof(*mainwindow), 1);

	db = setupDB(dpy, xrmTable, sizeof(xrmTable)/sizeof(xrmTable[0]),
			"xtut10", &argc, argv);

	font = getFont(dpy, db, "xtut10.font", "xtut10.Font", "fixed");
	mainwindow->background = getColour(dpy,  db, "xtut10.background", "xtut10.BackGround", "DarkGreen");
	mainwindow->border = getColour(dpy,  db, "xtut10.border", "xtut10.Border", "LightGreen");
	mainwindow->foreground = values.foreground = getColour(dpy,  db, "xtut10.foreground", "xtut10.ForeGround", "Red");


	mainwindow->width = 400;
	mainwindow->height = 400;

	win = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), /* display, parent */
		0,0, /* x, y: the window manager will place the window elsewhere */
		mainwindow->width, mainwindow->height, /* width, height */
		2, mainwindow->border, /* border width & colour, unless you have a window manager */
		mainwindow->background); /* background colour */

	Xutf8SetWMProperties(dpy, win, "XTut10", "xtut10", argv, argc,
		NULL, NULL, NULL);

	/* make the default pen what we want */
	values.line_width = 1;
	values.line_style = LineSolid;
	values.font = font->fid;

	XChangeGC(dpy, DefGC(dpy),
		GCForeground|GCLineWidth|GCLineStyle|GCFont,&values);


	{
	ExitInfo *exitInfo;
	exitInfo = malloc(sizeof(*exitInfo));
	exitInfo->dpy = dpy;
	exitInfo->font = font;
	createButton(dpy, win,
		getResource(dpy, db, "xtut10.exit", "xtut10.Exit", "Exit"),
		font, mainwindow->width/2-40, 17, 80, (font->ascent+font->descent)*2,
		mainwindow->foreground, mainwindow->background, mainwindow->border,
		ctxt, exitButton, exitInfo);
	}

	/* tell the display server what kind of events we would like to see */
	XSelectInput(dpy, win, StructureNotifyMask|ExposureMask);
	/* okay, put the window on the screen, please */
	XMapWindow(dpy, win);

	/* save the useful information about the window */
	XSaveContext(dpy, win, ctxt, (XPointer)mainwindow);

	return ctxt;
}



int main_loop(Display *dpy, XContext context){
	XEvent ev;

	/* as each event that we asked about occurs, we respond. */
	while(1){
		Button *button = NULL;
		XNextEvent(dpy, &ev);
		XFindContext(ev.xany.display, ev.xany.window, context, (XPointer*)&button);
		switch(ev.type){
		/* configure notify will only be sent to the main window */
		case ConfigureNotify:
			if (button)
				buttonConfigure(button, &ev);
			break;
		/* expose will be sent to both the button and the main window */
		case Expose:
			if (ev.xexpose.count > 0) break;
			if (button)
				buttonExpose(button, &ev);
			break;

		/* these three events will only be sent to the button */
		case EnterNotify:
			if (button)
				buttonEnter(button, &ev);
			break;
		case LeaveNotify:
			if (button)
				buttonLeave(button, &ev);
			break;
		case ButtonRelease:
			if (button && button->buttonRelease)
				button->buttonRelease(button->cbdata);
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
