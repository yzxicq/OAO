

#ifndef XTUT_CALLBACK
#define XTUT_CALLBACK
typedef void (*Callback)(void *cbdata);
#endif


#ifndef XTUT_BUTTON
#define XTUT_BUTTON
typedef struct Button Button;
struct Button {
	XChar2b * text;
	int text_width;
	int font_ascent;
	int width, height;
	unsigned long border, background, foreground;
	void *cbdata;
	Callback buttonRelease;
};
#endif

void createButton(Display *dpy, Window parent, char *text, XFontStruct *font,
		int x, int y, int width, int height,
		unsigned long foreground, unsigned long background, unsigned long border,
		XContext ctxt, Callback callback, void *cbdata);
void buttonExpose(Button *button, XEvent *ev);
void buttonConfigure(Button *button, XEvent *ev);
void buttonEnter(Button *button, XEvent *ev);
void buttonLeave(Button *button, XEvent *ev);
void buttonExpose(Button *button, XEvent *ev);
