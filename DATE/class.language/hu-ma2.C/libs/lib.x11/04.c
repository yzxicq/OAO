#include <stdio.h>
#include <X11/Xlib.h>
 
int
main (int argc, char **argv)
{
    Display *display;
    Window window;
    Window root;
    XSetWindowAttributes attributes;
    Pixmap pixmap;
    Visual *visual;
    int screen;
    int depth;
    Atom wmDeleteMessage;
    XEvent event;
 
    display = XOpenDisplay (NULL);
    screen = DefaultScreen (display);
    visual = DefaultVisual (display, screen);
    root = XRootWindow (display, screen);
    depth = DefaultDepth (display, screen);
    attributes.background_pixel = XWhitePixel (display, screen);
    attributes.override_redirect = 0;
 
    window = XCreateWindow (display, root,
                0, 0, 400, 200,
                                0, depth, InputOutput,
                visual,
                CWBackPixel | CWBorderPixel |
                CWOverrideRedirect, &attributes);
 
    wmDeleteMessage = XInternAtom (display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols (display, window, &wmDeleteMessage, 1);
    XStoreName (display, window, "test");
    XMapWindow (display, window);
 
    do {
        XNextEvent (display, &event);
        if (event.type == Expose) {
        } else if (event.type == ClientMessage
               && event.xclient.data.l[0] == wmDeleteMessage) {
            break;
        }
    } while (event.type != KeyPress);
 
    printf ("closing display\n");
    XCloseDisplay (display);
}
