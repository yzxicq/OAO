#include "common.h"

typedef struct FontTable FontTable;
struct FontTable {
	int nnames;
	int max_names;
	char **names;
	XFontStruct **fonts;
};

typedef struct ColourTable ColourTable;
struct ColourTable {
	int nnames;
	int max_names;
	char **names;
	unsigned long*colours;
};

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
		/*printf("read %s\n", filename); 
	} else  {
		printf("didn't read %s\n", filename); */
	}
	return db;
}

char *getResource(Display *dpy, XrmDatabase db, char *name, char *cl, char *def){
	extern char *strdup(char *);
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
	int j;
	char * type;
	extern char *strdup(char *);
	static ColourTable table = { 0, 0, NULL, NULL};
	/*printf("getColour called (%s, %s, %s)\n", name, cl, def); */

	for(j = 0; j < table.nnames; j += 1){
		if(!strcmp(table.names[j], name)){
			/*printf("returning %ld at %d\n", table.colours[j], j); */
			return table.colours[j];
		}
	}

	if (XrmGetResource(db, name, cl, &type, &v)
			&& XAllocNamedColor(dpy, cmap, v.addr, &col1, &col2)) {
	} else {
		XAllocNamedColor(dpy, cmap, def, &col1, &col2);
	}
	/* printf("returning %ld\n", col2.pixel); */
	if (j == 0){
		table.names = malloc(4*sizeof(*table.names));
		table.colours = malloc(4*sizeof(*table.colours));
	} else if (j >= table.max_names){
		void * temp;
		temp = realloc(table.names, j *2*sizeof(*table.names));
		if (!temp) return col2.pixel;
		table.max_names = j*2;
		table.names = temp;
		temp = realloc(table.colours, j *2*sizeof(*table.colours));
		if (!temp) return col2.pixel;
		table.colours = temp;
	}
	table.colours[j] = col2.pixel;
	table.names[j] = strdup(name);
	table.nnames = j +1;
	return col2.pixel;
}

XFontStruct *getFont(Display *dpy, XrmDatabase db, char *name,
		char *cl, char *def){
	static FontTable table = { 0, 0, NULL, NULL};
	XrmValue v;
	char * type;
	XFontStruct * font = NULL;
	extern char *strdup(char *);
	int j;

	for(j = 0; j < table.nnames; j += 1){
		if(!strcmp(table.names[j], name)){
			return table.fonts[j];
		}
	}

	if (XrmGetResource(db, name, cl, &type, &v)){
		if (v.addr)
			font = XLoadQueryFont(dpy, v.addr);
	}
	if (!font) {
		if (v.addr)
		fprintf(stderr, "unable to load preferred font: %s using fixed\n", v.addr);
		else 
		fprintf(stderr, "couldn't figure out preferred font (%s : %s )\n", name, cl);
		font = XLoadQueryFont(dpy, def);
	}
	if (j == 0){
		table.names = malloc(4*sizeof(*table.names));
		table.fonts = malloc(4*sizeof(*table.fonts));
	} else if (j >= table.max_names){
		void * temp;
		temp = realloc(table.names, j *2*sizeof(*table.names));
		if (!temp) return font;
		table.max_names = j*2;
		table.names = temp;
		temp = realloc(table.fonts, j *2*sizeof(*table.fonts));
		if (!temp) return font;
		table.fonts = temp;
	}
	table.fonts[j] = font;
	table.names[j] = strdup(name);
	table.nnames = j+1;
	return font;
}
