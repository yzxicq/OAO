#include "common.h"

XrmDatabase setupDB(Display *dpy,
			XrmOptionDescRec *xrmTable, int nCommandLineResources,
			const char *progname, int *argc, char **argv){
	XrmDatabase db;
	char filename[256];

	XrmInitialize();
	db = XrmGetDatabase(dpy);
	XrmParseCommand(&db, xrmTable, nCommandLineResources, progname, argc, argv);
	sprintf(filename, "%.240s.resources", progname);
	if(XrmCombineFileDatabase(filename, &db, False)){
		printf("read %s\n", filename);
	} else  {
		printf("didn't read %s\n", filename);
	}
	return db;
}

char *getResource(Display *dpy, XrmDatabase db, char *name, char *cl, char *def){
	XrmValue v;
	char * type;
	if(XrmGetResource(db, name, cl, &type, &v))
		return strdup(v.addr);
	return strdup(def);
}


unsigned long getColour(Display *dpy,  XrmDatabase db, char *name,
			char *cl, char *def){
	XrmValue v;
	XColor col1, col2;
	Colormap cmap = DefaultColormap(dpy, DefaultScreen(dpy));
	char * type;

	if (XrmGetResource(db, name, cl, &type, &v)
			&& XAllocNamedColor(dpy, cmap, v.addr, &col1, &col2)) {
	} else {
		XAllocNamedColor(dpy, cmap, def, &col1, &col2);
	}
	return col2.pixel;
}

XFontStruct *getFont(Display *dpy, XrmDatabase db, char *name,
		char *cl, char *def){
	XrmValue v;
	char * type;
	XFontStruct *font = NULL;

	if (XrmGetResource(db, name, cl, &type, &v)){
		if (v.addr)
			font = XLoadQueryFont(dpy, v.addr);
	}
	if (!font) {
		if (v.addr)
		fprintf(stderr, "unable to load preferred font: %s using fixed\n", v.addr);
		else 
		fprintf(stderr, "couldn't figure out preferred font\n");
		font = XLoadQueryFont(dpy, def);
	}
	return font;
}
