#include "common.h"
#include "button.h"

void createButton(Display *dpy, Window parent, char *text, XFontStruct *font,
		int x, int y, int width, int height,
		unsigned long foreground, unsigned long background, unsigned long border,
			XContext ctxt, Callback callback, void *cbdata){
	Button *button;
	Window win;
	int strlength = strlen(text);

	win = XCreateSimpleWindow(dpy, parent, x, y, width, height,
		2, border, background); /* borderwidth, border and background colour */
	if (!win) {
		fprintf(stderr, "unable to create a subwindow\n");
		exit(31);
	}

	button = calloc(sizeof(*button), 1);
	if (!button){
		fprintf(stderr, "unable to allocate any space, dieing\n");
		exit(32);
	}

	button->font_ascent = font->ascent;

	button->text = malloc(sizeof(*button->text) * (strlength+1));
	if (!button->text){
		fprintf(stderr, "unable to allocate any string space, dieing\n");
		exit(32);
	}
	strlength = utf8toXChar2b(button->text, strlength, text, strlength);
	button->text_width = XTextWidth16(font, button->text, strlength);
	button->buttonRelease = callback;
	button->cbdata = cbdata;
	button->width = width;
	button->height = height;
	button->background = background;
	button->foreground = foreground;
	button->border = border;

	XSelectInput(dpy, win,
		ButtonPressMask|ButtonReleaseMask|StructureNotifyMask|ExposureMask
			|LeaveWindowMask|EnterWindowMask);

	XSaveContext(dpy, win, ctxt, (XPointer)button);
	XMapWindow(dpy, win);
}


void buttonExpose(Button *button, XEvent *ev) {
	int textx, texty, len;
	if (!button) return;
	if (button->text){
		len = XChar2bLen(button->text);
   		textx = (button->width - button->text_width)/2;
   		texty = (button->height + button->font_ascent)/2;
   		XDrawString16(ev->xany.display, ev->xany.window, DefGC(ev->xany.display), textx, texty,
			button->text, len);
	} else {  /* if there's no text draw the big X */
		XDrawLine(ev->xany.display, ev->xany.window, DefGC(ev->xany.display), 0, 0, button->width, button->height);
		XDrawLine(ev->xany.display, ev->xany.window, DefGC(ev->xany.display), button->width, 0, 0, button->height);
	}
}
void buttonConfigure(Button *button, XEvent *ev){
	if (!button) return;
	if (button->width != ev->xconfigure.width
			|| button->height != ev->xconfigure.height) {
		button->width = ev->xconfigure.width;
		button->height = ev->xconfigure.height;
		XClearWindow(ev->xany.display, ev->xany.window);
	}
}

void buttonEnter(Button *button, XEvent *ev) {
	XSetWindowAttributes attrs;
	if(!button) return;
	attrs.background_pixel = button->border;
	attrs.border_pixel = button->background;
	XChangeWindowAttributes(ev->xany.display, ev->xany.window,
			CWBackPixel|CWBorderPixel, &attrs);
	XClearArea(ev->xany.display, ev->xany.window, 0, 0, button->width, button->height, 1);
}
void buttonLeave(Button *button, XEvent *ev) {
	XSetWindowAttributes attrs;
	if(!button) return;
	attrs.background_pixel = button->background;
	attrs.border_pixel = button->border;
	XChangeWindowAttributes(ev->xany.display, ev->xany.window,
			CWBackPixel|CWBorderPixel, &attrs);
	XClearArea(ev->xany.display, ev->xany.window, 0, 0, button->width, button->height, 1);
}
