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

#include <boost/array.hpp>
#include <boost/tuple/tuple.hpp>

const float MOVEX = 20;
const float MOVEY = 30;
const float ALLOWENCE = .45;
const float JUMPSECS = 400;
typedef boost::tuple<float,float> vec2;

class t_background
{
public:
   t_background();

   void draw(float xpos) const;

   bool checksquare(float xpos,float ypos) const;

private:
   //boost::array<boost::array<int,20>,20> board;
   int board[20][20];
};

class t_enemy
{
public:
   t_enemy(float x,float y);

   void draw(float xpos) const;

   float getx() const;
   float gety() const;

   void move(const t_background &back,const boost::posix_time::time_duration &time);

   bool movel(const t_background &back,const boost::posix_time::time_duration &time);
   bool mover(const t_background &back,const boost::posix_time::time_duration &time);
   bool moveu(const t_background &back,const boost::posix_time::time_duration &time);
   bool moved(const t_background &back,const boost::posix_time::time_duration &time);

private:
   vec2 pos;   
};

class t_player
{
public:
   t_player(float x,float y);

   void draw(float xpos) const;

   float getx() const;
   float gety() const;

   bool movel(const t_background &back,const boost::posix_time::time_duration &time);
   bool mover(const t_background &back,const boost::posix_time::time_duration &time);
   bool moveu(const t_background &back,const boost::posix_time::time_duration &time);
   bool moved(const t_background &back,const boost::posix_time::time_duration &time);

private:
   vec2 pos;   
};

class t_game
{
public:
   t_game();

   void drawall() const;

   void moveall();

private:
   t_enemy *enem;
   t_player play;
   t_background back;
   float xpos;
};

void graphicsInit();
void graphicsTest();

#endif //ADVENTUER_H_INCLUDED
