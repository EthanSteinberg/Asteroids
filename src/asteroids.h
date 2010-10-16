#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED

#include "menu.h"
#include "game.h"

class t_asteroids
{
public:
   t_asteroids();
   void drawall() const;
   
   void moveall();

private:
   t_menu menu;
   t_game game;
};

#endif //TEST_H_INCLUDED
