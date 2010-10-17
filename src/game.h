#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <boost/date_time/posix_time/posix_time.hpp>
#include <vector>

#include "vec.h"

class t_bullet
{
public:
   t_bullet(vec2 startpos,float direction);
   void draw() const;
   
   void move(const boost::posix_time::time_duration &time);

private:
   vec2 pos;
   float direction;
};

class t_background
{
public:
   void draw() const;
};

class t_ship
{
public:
   t_ship();
   void draw() const;
   
   void moveup(const boost::posix_time::time_duration &time);
   void movedown(const boost::posix_time::time_duration &time);

   void turnleft(const boost::posix_time::time_duration &time);
   void turnright(const boost::posix_time::time_duration &time);

   t_bullet* shoot() const;

private:
   vec2 pos;
   float direction;
};


class t_game
{
public:
   void drawall() const;

   bool moveall();

private:
   t_background background;
   t_ship ship;
   std::vector<t_bullet *> bullets;
};
#endif
