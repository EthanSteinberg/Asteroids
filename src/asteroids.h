#ifndef ASTEROIDS_H_INCLUDED
#define ASTEROIDS_H_INCLUDED

#include <boost/tuple/tuple.hpp>
#include "game.h"
#include "menu.h"

typedef boost::tuple<float,float> vec2;

class t_asteroids
{
public:
   t_asteroids();

   void drawall() const;

   void moveall();

private:
   t_menu menu;

};

#endif //ASTEROIDS_H_INCLUDED
