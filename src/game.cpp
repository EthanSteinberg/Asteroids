#include <SFML/Window.hpp>

#include "game.h"

extern sf::Input *Input;

//Class: t_game
//Purpose: Be an asteroid game
//Interface: moveall and drawall

void t_game::drawall() const
{
}

bool t_game::moveall()
{
   return Input->IsKeyDown(sf::Key::Escape);
}

