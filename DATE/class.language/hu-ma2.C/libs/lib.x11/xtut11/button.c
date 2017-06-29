#include "funcs.h"
#include "button.h"

Window newButton(char * progname, Display *dpy, Window parent, XrmDatabase db, XContext ctxt,
		char *name, int x, int y, int width, int height,
		Callback callback, void *cbdata){
	Button *button;
	Window win;

	char resourcename[256];
	char classname[256];
	char * text;
	int strlength;

	sprintf(resourcename, "%s.%s", progname, name);
	sprintf(classname, "%s.%s", progname, name);
	text = getResource(dpy, db, resourcename, classname, name);

	button = calloc(sizeof(*button), 1);
	if (!button){
		fprintf(stderr, "unable to allocate any space for %s, dieing\n", name);
		exit(32);
	}
	button->funcs = ButtonFuncs;
	sprintf(resourcename, "%s.%s.font", progname, name);
	sprintf(classname, "%s.font", progname);
	button->font = getFont(dpy, db, resourcename, classname, "fixed");
	button->font_ascent = button->font->ascent;


	strlength = strlen(text);
	button->text = malloc(sizeof(*button->text) * (strlength +1));
	if (!button->text){
		fprintf(stderr, "Can't display text %d\n", (sizeof(*button->text) * (strlength +1)));
		strlength = 0;
		exit(33);
	} else {
	strlength = utf8toXChar2b(button->text, strlength, text, strlength);
	button->text_width = XTextWidth16(button->font, button->text, strlength);
	}

	sprintf(resourcename, "%s.%s.foreground", progname, name);
	sprintf(classname, "%s.foreground", progname);
	button->foreground = getColour(dpy, db, resourcename, classname, "blue");

	sprintf(resourcename, "%s.%s.background", progname, name);
	sprintf(classname, "%s.background", progname);
	button->background = getColour(dpy, db, resourcename, classname, "cornflowerblue");

	sprintf(resourcename, "%s.%s.border", progname, name);
	sprintf(classname, "%s.border", progname);
	button->border = getColour(dpy, db, resourcename, classname, "lightblue");

	button->buttonRelease = callback;
	button->cbdata = cbdata;
	if (width < button->text_width + 2*button->font->ascent){
		width = button->text_width + 2*button->font->ascent;
	}
	button->width = width;

	if (height < 2*button->font->ascent){
		height = 2*button->font->ascent;
	}
	button->height = height;
	if (button->height <= 0){
		fprintf(stderr, "font ascent * 2 = %d\n", button->height);
	}

	win = XCreateSimpleWindow(dpy, parent, x, y, width, height,
		2, button->border, button->background); /* borderwidth, border and background colour */
	if (!win) {
		fprintf(stderr, "unable to create a subwindow for %s\n", name);
		exit(31);
	}

	XSelectInput(dpy, win,
		ButtonPressMask|ButtonReleaseMask|StructureNotifyMask|ExposureMask
			|LeaveWindowMask|EnterWindowMask);

	if (button->funcs != ButtonFuncs){
		fprintf(stderr, "Weird!\n");
	}
	XSaveContext(dpy, win, ctxt, (XPointer)button);
	XMapWindow(dpy, win);
	return win;
}

void buttonRedraw(Button *button, XEvent *ev) {
	int textx, texty, len;
	XGCValues values;
	/* printf("buttonRedraw called\n"); */
	if (!button) return;

	/*
	printXc(button->text);
	printf("\n"); */

	values.line_width = 1;
	values.line_style = LineSolid;
	values.foreground = button->foreground;
	values.font = button->font->fid;
	XChangeGC(ev->xany.display, DefGC(ev->xany.display),
			GCForeground|GCLineWidth|GCLineStyle|GCFont,&values);
	if (button->text){
		len = XChar2bLen(button->text);
   		/*textx = (button->width - button->text_width)/2; */
   		textx = 0; /* left justify all menu and menubar button texts */
   		texty = (button->height + button->font_ascent)/2;
   		XDrawString16(ev->xany.display, ev->xany.window, DefGC(ev->xany.display), textx, texty,
			button->text, len);
	} else {  /* if there's no text draw the big X */
		XDrawLine(ev->xany.display, ev->xany.window, DefGC(ev->xany.display), 0, 0, button->width, button->height);
		XDrawLine(ev->xany.display, ev->xany.window, DefGC(ev->xany.display), button->width, 0, 0, button->height);
	}
}

static void buttonExpose(Block *block, XEvent *ev) {
	/* printf("buttonExpose called\n"); */
	if (ev->xexpose.count > 0) return;
	buttonRedraw(&block->button, ev);
}

static void buttonConfigureNotify(Block *block, XEvent *ev){
	Button * button = &block->button;
	if (!button) return;
	if (button->width != ev->xconfigure.width
			|| button->height != ev->xconfigure.height) {
		button->width = ev->xconfigure.width;
		button->height = ev->xconfigure.height;
		XClearWindow(ev->xany.display, ev->xany.window);

		/* redraw please */
		buttonRedraw(button, ev);
	}
}

void buttonEnter(Block *block, XEvent *ev) {
	Button *button = &block->button;
	XSetWindowAttributes attrs;
	GC gc = DefGC(ev->xany.display);
	if(!button) return;
	attrs.background_pixel = button->border;
	attrs.border_pixel = button->background;
	XChangeWindowAttributes(ev->xany.display, ev->xany.window,
			CWBackPixel|CWBorderPixel, &attrs);
	XSetForeground(ev->xany.display, gc, button->border);
	XFillRectangle(ev->xany.display, ev->xany.window, gc, 0, 0, button->width, button->height);
	buttonRedraw(button, ev);
}
void buttonLeave(Block *block, XEvent *ev) {
	Button *button = &block->button;
	XSetWindowAttributes attrs;
	GC gc = DefGC(ev->xany.display);
	if(!button) return;
	attrs.background_pixel = button->background;
	attrs.border_pixel = button->border;
	XChangeWindowAttributes(ev->xany.display, ev->xany.window,
			CWBackPixel|CWBorderPixel, &attrs);
	XSetForeground(ev->xany.display, gc, button->background);
	XFillRectangle(ev->xany.display, ev->xany.window, gc, 0, 0, button->width, button->height);
	buttonRedraw(button, ev);
}
static void buttonRelease(Block *block, XEvent *ev){
	Button *button = &block->button;
	if (button && button->buttonRelease)
		button->buttonRelease(button->cbdata, ev);
}
static void menuBarButtonPress(Block *block, XEvent *ev){
	
}
static void menuBarButtonRelease(Block *block, XEvent *ev){
}

static struct Funcs _ButtonFuncs = {
	/*struct Funcs*type; */
	buttonConfigureNotify,
	buttonLeave, /* leave */
	buttonEnter, /* enter */
	buttonExpose, /* expose */
	NULL,
	buttonRelease /* button release */
};
struct Funcs *ButtonFuncs = &_ButtonFuncs;

static struct Funcs _MenuBarButtonFuncs = {
	/*struct Funcs*type; */
	buttonConfigureNotify,
	buttonLeave, /* leave */
	buttonEnter, /* enter */
	buttonExpose, /* expose */
	menuBarButtonPress, /* button release */
	menuBarButtonRelease /* button release */
};
struct Funcs *MenuBarButtonFuncs = &_MenuBarButtonFuncs;
