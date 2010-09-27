#include <GL/glew.h>
#include <GL/glx.h>
#include <GL/glu.h>

#include <X11/Xlib.h>
#include <X11/keysym.h>

#include <iostream>

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/array.hpp>

#include "blocks.h"

Display *dpy;
Window win;
GLXContext glc;

boost::array<int,32> PressedKeys;

void ReSize (int w, int h);
void SetupRC();
void xlibInit();
void MainLoop();
void RenderScene();
void StartMoveEvents();
void MoveEvents();

using namespace std;

int main ()
{
    glewInit();

    xlibInit();

    SetupRC();

    MainLoop();
}

void SetupRC()
{
    glClearColor(0.0f,0.0f,1.0f,1.0f);

    glColor3f(0.0f,1.0f,0.0f);

    glShadeModel(GL_FLAT);
}

void ReSize (int w, int h)
{
    if (h == 0)
        h = 1;
    glViewport(0,0,w,h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-100.0f,100.0f,-100.0f,100.0f,0.0f,1.0f);

    glMatrixMode(GL_MODELVIEW);
}

void xlibInit()
{
    XSetWindowAttributes swa;
    GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };

    dpy = XOpenDisplay(NULL); //connect to X server
    Window root = DefaultRootWindow(dpy); //Get window of OS
    XVisualInfo *vi = glXChooseVisual(dpy, 0, att); //get closest visual to what we want

    Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone); //create color map
    swa.colormap = cmap; //set color map to created one
    swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask; //set event map to key presses and resizes
    win = XCreateWindow(dpy, root, 0, 0, 600, 600, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa); //create window

    XMapWindow(dpy, win); //draw window
    XStoreName(dpy, win, "VERY SIMPLE APPLICATION"); //name window
    glc = glXCreateContext(dpy, vi, NULL, GL_FALSE);//connect to OpenGl, with new context
    glXMakeCurrent(dpy, win, glc); //connect opengl calls to opengl window
}

void MainLoop()
{
    XEvent xev;
    XWindowAttributes gwa;
    XAutoRepeatOff(dpy);

    StartMoveEvents();

    boost::system_time time = boost::get_system_time();

    for (;;)
    {
        while (XPending(dpy))
        {
            XNextEvent(dpy, &xev);

            if(xev.type == Expose)
            {
                XGetWindowAttributes(dpy, win, &gwa);
                ReSize(gwa.width, gwa.height);
            }

            else if(xev.type == KeyPress)
            {
                switch (XLookupKeysym(&xev.xkey,0))
                {
                case XK_Return:
                    glXMakeCurrent(dpy, None, NULL);
                    glXDestroyContext(dpy, glc);
                    XDestroyWindow(dpy, win);
                    XCloseDisplay(dpy);
                    exit(0);
                    break;
                case XK_a:
                    PressedKeys[0] = 1;
                    break;
                case XK_s:
                    PressedKeys[1] = 1;
                    break;
                }
            }

            else if(xev.type == KeyRelease)
            {
                switch (XLookupKeysym(&xev.xkey,0))
                {
                case XK_a:
                    PressedKeys[0] = 0;
                    break;
                case XK_s:
                    PressedKeys[1] = 0;
                    break;
                }
            }
        }
        boost::this_thread::sleep(time);
        time = boost::get_system_time() + boost::posix_time::milliseconds(20); //minimum time between draws
        RenderScene();
    }
}

void RenderScene()
{
    glClear(GL_COLOR_BUFFER_BIT);

    //draw stuff here

    glXSwapBuffers(dpy,win);
}

void StartMoveEvents()
{
    //call stuff that only runs once

    boost::thread moveevents(MoveEvents);
}

void MoveEvents()
{
    boost::system_time time;

    for (;;)
    {
        time = boost::get_system_time() + boost::posix_time::milliseconds(33); //minimum time between movements

        //call other functions here
        //see if keys are pressed by checking the KeyPressed array

        boost::this_thread::sleep(time);
    }
}
