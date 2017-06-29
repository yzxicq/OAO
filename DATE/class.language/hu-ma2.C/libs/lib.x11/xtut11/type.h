
#include <X11/Xlib.h>
#include "common.h"
#include "app.h"
#include "menubar.h"
#include "menu.h"
#include "button.h"
#ifndef XTUT_TYPE
#define XTUT_TYPE 1
typedef union Block Block;
typedef void (*EvCallback)(Block*, XEvent*);

typedef struct Funcs Funcs;
struct Funcs {
	EvCallback configureNotify;
	EvCallback leaveNotify;
	EvCallback enterNotify;
	EvCallback expose;
	EvCallback buttonPress;
	EvCallback buttonRelease;
};

union Block {
	struct Funcs *funcs;
	struct App app;
	struct MenuBar menubar;
	struct Menu menu;
	struct Button button;
};
#endif
