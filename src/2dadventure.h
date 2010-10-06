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

#ifndef ADVENTURE_H_INCLUDED
#define ADVENTURE_H_INCLUDED

#include <boost/tuple/tuple.hpp>

typedef boost::tuple<float,float> vec2;

class t_player;
{
public:
   t_player(float x,float y);

   void draw() const;

   bool movel();
   bool mover();
   bool moveu();
   bool moved();

private:
   vec2 pos;   
}

class t_background;
{
public:
   t_background(const char *filename);

void graphicsInit();
void graphicsTest();

#endif //ADVENTUER_H_INCLUDED
