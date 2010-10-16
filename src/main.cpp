//  Copyright (C) 2010 Ethan Steinberg
//
//  This software is provided 'as-is', without any express or implied
//  warranty.  In no event will the authors be held liable for any damages
//  arising from the use of this software.
//
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute it
//  freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//     claim that you wrote the original software. If you use this software
//     in a product, an acknowledgment in the product documentation would be
//     appreciated but is not required.
//  2. Altered source versions must be plainly marked as such, and must not be
//     misrepresented as being the original software.
//  3. This notice may not be removed or altered from any source distribution.

#include <GL/glew.h>
#include <SFML/Window.hpp>

#include <iostream>
#include <cstdio>

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/array.hpp>

#include "asteroids.h"

t_asteroids *asteroids;

sf::Window *App;
const sf::Input *Input;

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
    App = new sf::Window(sf::VideoMode(800, 600, 32), "SFML Window");
    Input = &(App->GetInput());
    assert(App->SetActive(true));
    
    glewInit();
    SetupRC();
    ReSize(800,600);
    MainLoop();

    std::cin.get();
}

void SetupRC()
{
    //initilize here
    asteroids = new t_asteroids;

    glClearColor(0.0f,1.0f,1.0f,1.0f);

    glColor3f(0.0f,1.0f,0.0f);

    glShadeModel(GL_FLAT);
}

void ReSize (int w, int h)
{
    if (h == 0)
        h = 1;
    glViewport(0,0,w,h);
}

void MainLoop()
{
    StartMoveEvents();

    boost::system_time time = boost::get_system_time();

    for (;;)
    {
       
        sf::Event Event;
        while (App->GetEvent(Event))
        ;
	/*{
            if(Event.Type == sf::Event::Resized)
            {
                ReSize(Event.Size.Width,Event.Size.Height);
            }

            else if(Event.Type == sf::Event::KeyPressed)
            {
                switch (Event.Key.Code)
                {
                case sf::Key::Return:
                    exit(0);
                    break;
                case 'r':
                    PressedKeys[0] = 1;
                    break;
                case 'w':
                    PressedKeys[1] = 1;
                    break;
	        case 's':
		    PressedKeys[2] = 1;
		    break;
	        case 'a':
		    PressedKeys[3] = 1;
		    break;
	        default: ;
                }
            }

            else if(Event.Type == sf::Event::KeyReleased)
            {
                switch (Event.Key.Code)
                {
                case 'r':
                    PressedKeys[0] = 0;
                    break;
                case 'w':
                    PressedKeys[1] = 0;
                    break;
	        case 's':
		    PressedKeys[2] = 0;
		    break;
	        case 'a':
		    PressedKeys[3] = 0;
		    break;
                default: ;
		}
            }

	    else if(Event.Type == sf::Event::MousePressed)
        }
	*/
        boost::this_thread::sleep(time);
        time = boost::get_system_time() + boost::posix_time::milliseconds(20); //minimum time between draws
        RenderScene();
    }
}

void RenderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //draw stuff here
    asteroids->drawall();

    App->Display();
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
        asteroids->moveall();
	 
        boost::this_thread::sleep(time);
    }
}
