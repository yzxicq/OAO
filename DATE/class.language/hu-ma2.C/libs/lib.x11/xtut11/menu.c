#include "funcs.h"
#include "menu.h"

void resizeBlock(Block *block, Window win, int width, int height, XEvent *ev){
	XEvent temp;
	XResizeWindow(ev->xany.display, win, width, height);
	if (!(block &&  block->funcs && block->funcs->configureNotify))
		return;
	temp.xconfigure.type = ev->xany.type;
	temp.xconfigure.display = ev->xany.display;
	temp.xconfigure.window = win;
	temp.xconfigure.event = win;
	temp.xconfigure.width = width;
	temp.xconfigure.height = height;
	temp.xconfigure.border_width = 0; /* assume we never use borders */
	temp.xconfigure.above = None;
	temp.xconfigure.override_redirect = False;
	block->funcs->configureNotify(block, &temp);
}

void menuConfigureNotify(Block *block, XEvent *ev){
	Menu *self = &block->menu;
	XConfigureEvent *xce = &ev->xconfigure;
	int j;
	int  fullheight = 0;
	Window w = xce->window;

	
	if (xce->width == self->width) return;
	for (j = 0; j < self->nsubws; j += 1){
		Button *button;
		if (XFindContext(xce->display, self->subws[j], self->ctxt, (XPointer*)&button))
			continue;
		fullheight += button->height;
		if (button->funcs!= ButtonFuncs){
			fprintf(stderr, "I've got a bad feeling about this \n");
		} else {
			resizeBlock((Block*)button, self->subws[j], xce->width, button->height, ev);
		}
	}
	if (fullheight == 0){
		fprintf(stderr, "fullheight = 0\n");
		return;
	}
	if (xce->height != fullheight){
		XResizeWindow(xce->display, w, xce->width, fullheight);
	}

}

Window newMenu(char *progname, Display *dpy, XrmDatabase db,
		XContext ctxt, char *name){
	Window win;
	Menu *menu;
	Window parent = DefaultRootWindow(dpy);
	XSetWindowAttributes xswa;

	xswa.save_under = True;
	xswa.override_redirect = True;

	menu = calloc(sizeof(*menu), 1);
	if (!menu){
		fprintf(stderr, "unable to allocate any space for %s menu, dieing\n", name);
		exit(32);
	}
	menu->ctxt = ctxt;
	menu->funcs = MenuFuncs;
	win = XCreateWindow(dpy, parent, 0, 0, 100, 100,
		0, CopyFromParent, CopyFromParent, CopyFromParent,
		 CWOverrideRedirect|CWSaveUnder , &xswa);

	XSelectInput(dpy, win, StructureNotifyMask);
	XSaveContext(dpy, win, ctxt, (XPointer)menu);
	return win;
}

Window *menuSetSubWins(Display *dpy, XContext ctxt, Window win, Window *subws, int nsubws){
	Menu *menu;
	Window *old;
	int j, fullheight =100, maxwidth =100;
	XFindContext(dpy, win, ctxt, (XPointer*)&menu);
	old = menu->subws;
	menu->subws = subws;
	menu->nsubws = nsubws;


	for(j =0; j < nsubws; j ++ ){
		Button *button;
		if (XFindContext(dpy, menu->subws[j], ctxt, (XPointer*)&button))
			continue;
		fullheight += button->height;
		if (maxwidth < button->width);
			maxwidth = button->width ;
	}
	for(j =0; j < nsubws; j ++ ){
		Button *button;
		if (XFindContext(dpy, menu->subws[j], ctxt, (XPointer*)&button))
			continue;
		button->width = maxwidth; /* cheating non encapsulated. */
	}
	XResizeWindow(dpy, win, maxwidth, fullheight);

	return old;
}

static struct Funcs _MenuFuncs = {
	menuConfigureNotify,
	NULL, /* leave */
	NULL, /* enter */
	NULL, /* expose */
	NULL, /* button press */
	NULL /* button release */
};
struct Funcs *MenuFuncs = &_MenuFuncs;
