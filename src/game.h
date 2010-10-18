#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <boost/date_time/posix_time/posix_time.hpp>
#include <vector>

#include "vec.h"

class t_bullets;
class t_ship;

class t_rock
{
public:
   t_rock(vec2 startpos,float direction);
   void draw() const;

   void move(const boost::posix_time::time_duration &time);

   vec2 getPosition() const;

private:
   vec2 pos;
   float direction;
};

class t_rocks
{
public:
   void drawall() const;

   void moveall(const boost::posix_time::time_duration &time);
   
   void add(t_rock *newRock);

   size_t size() const;

   bool collided(const t_bullets &bullets);
   bool collided(const t_ship &ship);

private:
   std::vector<t_rock *> rock;
   float distance(const t_bullets &bullets,int l,int i) const;
};

class t_bullet
{
public:
   t_bullet(vec2 startpos,float direction);
   void draw() const;
   
   void move(const boost::posix_time::time_duration &time);
   
   vec2 getPosition() const;

private:
   vec2 pos;
   float direction;
};

class t_bullets
{
public:
   void drawall() const;

   void moveall(const boost::posix_time::time_duration &time);

   void add(t_bullet *newBullet);
   
   size_t size() const;

   vec2 getPosition(int i) const;

private:
   std::vector<t_bullet *> bullet;
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
   t_game();
   void drawall() const;

   bool moveall();

private:
   t_background background;
   t_ship ship;
   t_bullets bullets;
   t_rocks rocks;
};
#endif
