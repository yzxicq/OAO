/* First, a basic window with a white background is created. Then another
 * window, achild of hte first is created with  a black background.
 * This second window is repeatedly mapped into its parent window and
 * then removed after 3 seconds. Each mapping is at different location
 *
 * Coded by:	Ross Maloney
 * Date:	March 2011
 * Modified:	Abdellah Chelli
 * Date: 	January 2015
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <unistd.h> //sleep()

int
main (int argc, char * argv[])
{
  Display	*mydisp;
  XSetWindowAttributes	mywinat;
  Window	mywin,mysubwin;
  XWindowChanges	mywinchang;
  XSizeHints	wmsize;
  XWMHints	wmhints;
  XTextProperty windowName, iconName;
  XEvent	myevent;
  char		*window_name = "Walking";
  char		*icon_name = "Wk";
  int		screen_num, done;
  unsigned long valuemask;
  int		x, y;

  	/* 1. open connection to the server */
  mydisp = XOpenDisplay(NULL);
  
  	/* 2. create a top-level window */
  screen_num = DefaultScreen(mydisp);
  mywinat.background_pixel = WhitePixel(mydisp, screen_num);
  mywinat.border_pixel = BlackPixel(mydisp, screen_num);
  mywinat.event_mask = ExposureMask | StructureNotifyMask;
  valuemask = CWBackPixel | CWBorderPixel | CWEventMask;
  mywin = XCreateWindow(mydisp, RootWindow(mydisp, screen_num),
    200, 300, 350, 250, 2,
    DefaultDepth(mydisp, screen_num), InputOutput,
    DefaultVisual(mydisp, screen_num),
    valuemask, &mywinat);

	/* 3. give the window manager hints */
  wmsize.flags = USPosition | USSize;
  XSetWMNormalHints(mydisp, mywin, &wmsize);
  wmhints.initial_state= NormalState;
  wmhints.flags = StateHint;
  XSetWMHints(mydisp, mywin, &wmhints);
  XStringListToTextProperty(&window_name, 1, &windowName);
  XSetWMName(mydisp, mywin, &windowName);
  XStringListToTextProperty(&icon_name, 1, &iconName);
  XSetWMIconName(mydisp, mywin, &iconName);

	/* 4. establish window resources */
  mywinat.background_pixel = BlackPixel(mydisp, screen_num);

	/* 5. create all the other windows needed */
  mysubwin = XCreateWindow(mydisp, mywin,
    100, 30, 50, 70, 2,
    DefaultDepth(mydisp, screen_num), InputOutput,
    DefaultVisual(mydisp, screen_num),
    valuemask, &mywinat);

	/* 6. select events for each window */
  valuemask = CWX | CWY;

	/* 7. map the windows */
  XMapWindow(mydisp, mywin);

	/* 8. enter the event loop */
  done = 0;
  x = 11; y = 12;
  while( done == 0 )
  {
    mywinchang.x = x;
    mywinchang.y = y;
    XConfigureWindow(mydisp, mysubwin, valuemask, &mywinchang);
    XFlush(mydisp);
    XNextEvent(mydisp, &myevent);
    switch( myevent.type )
    {
    case Expose:
      break;
    case ConfigureNotify:
      XMapWindow(mydisp, mysubwin);
      sleep(3);
      x += 5; y += 6;
    }
  }

	/* 9. clean up before exiting */
  XUnmapWindow(mydisp, mywin);
  XDestroyWindow(mydisp, mywin);
  XCloseDisplay(mydisp);
  return 0;
}
