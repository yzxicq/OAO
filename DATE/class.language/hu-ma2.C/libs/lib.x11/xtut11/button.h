

#ifndef XTUT_CALLBACK
#define XTUT_CALLBACK
typedef void (*Callback)(void *cbdata, XEvent *ev);
#endif


#ifndef XTUT_BUTTON
#define XTUT_BUTTON
typedef struct Button Button;
struct Button {
	struct Funcs* funcs;
	XChar2b * text;
	int text_width;
	int font_ascent;
	XFontStruct *font;
	int width, height;
	unsigned long border, background, foreground;
	void *cbdata;
	Callback buttonRelease;
};
#endif
extern struct Funcs *ButtonFuncs;

Window newButton(char *progname, Display *dpy, Window w, XrmDatabase db, XContext ctxt,
	char *name, int x, int y, int width, int height,
	Callback callback, void *cbdata);
	
