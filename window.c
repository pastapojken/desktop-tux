#include <X11/extensions/XInput2.h>
#include <stdio.h>
#include <stdlib.h>
#include "display.h"
#include "window.h"
#include "list.h"

Window getRootWindow(display *disp) {
    return DefaultRootWindow(disp->myDisplay);
}

void newRootWindow(display *disp, window *win) {
    win->myWindow = 0;
    win->myWindow = getRootWindow(disp);
    win->myDisplay = disp;
    win->mask.deviceid = XIAllMasterDevices;
    win->mask.mask_len = XIMaskLen(XI_LASTEVENT);
    win->mask.mask = calloc(win->mask.mask_len, sizeof (unsigned char));
    XISetMask(win->mask.mask, XI_RawKeyPress);
    XISetMask(win->mask.mask, XI_RawKeyRelease);
    XISetMask(win->mask.mask, XI_RawButtonPress);
    XISetMask(win->mask.mask, XI_RawButtonRelease);
    XISetMask(win->mask.mask, XI_RawMotion);
}

void createWindow(display *myDisplay, window *temp, int x, int y, int width, int height, int borderWidth, int depth, unsigned int class, Visual *visual, unsigned long valuemask, XSetWindowAttributes *attributes) {
    Display *disp = myDisplay->myDisplay;
    temp->myDisplay = myDisplay;
    temp->myWindow = XCreateWindow(disp, getRootWindow(myDisplay), x, y, width, height, borderWidth, depth, class, visual, valuemask, attributes);
    temp->myWinAttr = attributes;
    Atom delWindow = XInternAtom(disp, "WM_DELETE_WINDOW", 0);
    XSelectInput(disp, temp->myWindow, ExposureMask | KeyPressMask | XI_RawMotionMask | StructureNotifyMask | ResizeRedirectMask | PropertyChangeMask);
    XSetWMProtocols(disp, temp->myWindow, &delWindow, 1);
}

void createSimpleWindow(display *display, window *temp, int x, int y, int width, int height, int borderSize, unsigned long border, unsigned long background) {
    Display *disp = display->myDisplay;
    temp->myDisplay = display;
    temp->myWindow = XCreateSimpleWindow(disp, getRootWindow(display), x, y, width, height, borderSize, border, background);
    Atom delWindow = XInternAtom(disp, "WM_DELETE_WINDOW", 0);
    XSetWMProtocols(disp, temp->myWindow, &delWindow, 1);    
}

void destroyAllDisplayWindows(display *myDisplay) {
    Display *disp = myDisplay->myDisplay;
    for (int i = 0; i < L_getListSize(myDisplay->List_myWindows); i++) {
        List *tempList = L_getListElem(myDisplay->List_myWindows, i);
        window *temp = (struct window*) tempList->data;
        XDestroyWindow(disp, temp->myWindow);
        fprintf(stdout, "%i\n", i);
    }
}

void mapWindow(display *disp, window *w){
    L_push_back(disp->List_myWindows, sizeof (w), w);
    XMapWindow(disp->myDisplay, w->myWindow);
}