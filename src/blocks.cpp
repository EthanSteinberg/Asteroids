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

#include <blocks.h>
#include <iostream>

#include <cassert>
#include <cmath>
#include <cstdlib>

#include <SFML/Window.hpp>

extern boost::array<int,32> PressedKeys;

//Class Bord
//Container for other classes with general function

bord::bord()
{
   std::srand(time(NULL));
   selblock = new selblok(0);
}

void bord::drawall() const
{
   selblock->drawall();
   arrblock.drawall();
}

void bord::moveall()
{
   if (PressedKeys[0])
      PressedKeys[0] = 0,selblock->rotatel(arrblock);
   if (PressedKeys[1])
      PressedKeys[1] = 0,selblock->rotater(arrblock);
   if (PressedKeys[2])
      selblock->mover(arrblock);
   if (PressedKeys[3])
      selblock->movel(arrblock);

   if (selblock->moved(arrblock))
   {
      selblock->addblock(&arrblock);
      delete selblock;
      selblock = new selblok(rand() %7);
   }

   for (int i =0;i<ROWS;i++)
      if (arrblock.checkrow(i))
	 arrblock.clearrow(i);
}

//Class Blok
//Holds color and position with a small wrapper

void blok::draw() const
{
   glColor3f(col.get<0>(), col.get<1>(), col.get<2>());
   glRectf(pos.get<0>(), pos.get<1>(), pos.get<0>() + 1, pos.get<1>() + 1);
   assert(pos.get<0>() >= 0);
   assert(pos.get<1>() >= 0);
}

void blok::setpos(double x,double y)
{
   pos.get<0>() = x,pos.get<1>() = y;
}

double blok::getx() const
{
   return pos.get<0>();
}

double blok::gety() const
{
   return pos.get<1>();
}

void blok::setcol(const color &newcol)
{
   col = newcol;
}

const color& blok::getcol() const
{
   return col;
}

void blok::movedown()
{
   --pos.get<1>();
}

void blok::moved()
{
   pos.get<1>() -= DOWNMOVEMENT;
}

void blok::movel()
{
   pos.get<0>() -= MOVEMENT;
}

void blok::mover()
{
   pos.get<0>() += MOVEMENT;
}

bool blok::canmoved(const arrblok &arrblock) const
{
   return (pos.get<1>() - 4 * DOWNMOVEMENT > 0) && !arrblock.checksquare(pos.get<0>() + .5,pos.get<1>());
}

bool blok::canmovel(const arrblok &arrblock) const
{
   return (pos.get<0>() - MOVEMENT > 0) && !arrblock.checksquare(pos.get<0>(),pos.get<1>() + .5);
}

bool blok::canmover(const arrblok &arrblock) const
{
   return ((pos.get<0>() + MOVEMENT + 1) < COLUMNS) && !arrblock.checksquare(pos.get<0>() + 1,pos.get<1>() + .5);
}

void blok::rotater(const blok *origin)
{
   double x = -pos.get<1>() + origin->gety() + origin->getx();
   pos.get<1>() = pos.get<0>() - origin->getx() + origin->gety();
   pos.get<0>() = x;
}
   
void blok::rotatel(const blok *origin)
{
   double x = pos.get<1>() - origin->gety() + origin->getx();
   pos.get<1>() = -pos.get<0>() + origin->getx() + origin->gety();
   pos.get<0>() = x;
}

bool blok::canrotate(const arrblok &arrblock)
{
   return ( (pos.get<1>() > 0) && 
	    (pos.get<1>() < ROWS) && 
	    (pos.get<0>() > 0) && 
	    (pos.get<0>() < COLUMNS -1) && 
	    !arrblock.checksquare(pos.get<0>(),pos.get<1>())
	  );
}
//Class ArrBlok
//Holds an array of RowBlocks

void arrblok::drawall() const
{
   for (int i = 0;i<ROWS;i++)
      rowblock[i].drawall();
}

bool arrblok::checkrow(int y) const
{
   return rowblock[y].checkrow();
}

void arrblok::clearrow(int y)
{
   for (int i =y;i<ROWS;i++)
      rowblock[i] = rowblock[i+1],rowblock[i].movedown();
}

bool arrblok::checksquare(int x,int y) const
{
   return rowblock[y].checksquare(x);
}

void arrblok::addblock(blok *newblock)
{
   rowblock[(int) (newblock->gety() + .5)].addblock(newblock);
}

//Class RowBlok
//Holds an array of blocks

rowblok::rowblok() 
{
   for (int i =0;i<COLUMNS;i++)
      block[i] = NULL;
}

void rowblok::drawall() const
{
   for (int i = 0;i<COLUMNS;i++)
      if (block[i] != NULL)
	 block[i]->draw();
}

bool rowblok::checkrow() const
{
   for (int i = 0;i<COLUMNS;i++)
      if (!block[i])
	 return 0;

   return 1;
}

bool rowblok::checksquare(int x) const
{
   return block[x];
}

void rowblok::addblock(blok *newblock)
{
   block[(int) (newblock->getx() + .5)] = newblock;
   newblock->setpos(std::floor(newblock->getx() + .5),std::floor(newblock->gety() + .5));
}

void rowblok::movedown()
{
   for (int i = 0;i<COLUMNS;i++)
      if (block[i])
	 block[i]->movedown();
}

//Class SelBlok
//Holds an array of selected blocks and can move/rotate them

selblok::selblok(int type)  
{
   const color GREEN(0,1,0);
   const color RED(1,0,0);
   const color BLUE(0,0,.5);
   const color PURPLE(.6,0,1);
   const color ORANGE(1,.6,0);
   const color OLIVE(.5,.5,0);
   const color YELLOW(1,1,0);

   switch(type)
   {
      case 0:
	 for (int i = 0;i<4;i++)
	 {
	    block[i] = new blok;
	    block[i]->setcol(GREEN);
	 }

	 block[2]->setpos(COLUMNS/2,ROWS);
	 block[3]->setpos(COLUMNS/2 +1,ROWS -1);
	 block[1]->setpos(COLUMNS/2,ROWS -1);
	 block[0]->setpos(COLUMNS/2 -1,ROWS -1);
	 break;

      case 1:
	 for (int i = 0;i<4;i++)
         {
            block[i] = new blok;
            block[i]->setcol(BLUE);
         }
                                                   
         block[2]->setpos(COLUMNS/2,ROWS);
         block[3]->setpos(COLUMNS/2 +1,ROWS);
         block[1]->setpos(COLUMNS/2,ROWS -1);
         block[0]->setpos(COLUMNS/2 + 1,ROWS -1);
	 break;

      case 2:
	 for (int i =0;i<4;i++)
	 {
	    block[i] = new blok;
	    block[i]->setcol(RED);
	 }

	 block[2]->setpos(COLUMNS/2,ROWS);
	 block[3]->setpos(COLUMNS/2 -1,ROWS);
	 block[1]->setpos(COLUMNS/2,ROWS -1);
	 block[0]->setpos(COLUMNS/2 +1,ROWS -1);
	 break;

      case 3:
	 for (int i =0;i<4;i++)
	 {
	    block[i] = new blok;
	    block[i]->setcol(PURPLE);
	 }
	 
	 block[2]->setpos(COLUMNS/2,ROWS);
	 block[3]->setpos(COLUMNS/2+1,ROWS);
	 block[1]->setpos(COLUMNS/2,ROWS-1);
	 block[0]->setpos(COLUMNS/2-1,ROWS-1);
	 break;

      case 4:
	 for (int i =0;i<4;i++)
	 {
	    block[i] = new blok;
	    block[i]->setcol(YELLOW);
	 }
	 
	 block[2]->setpos(COLUMNS/2,ROWS);
	 block[3]->setpos(COLUMNS/2+1,ROWS);
	 block[1]->setpos(COLUMNS/2+2,ROWS);
	 block[0]->setpos(COLUMNS/2-1,ROWS);
	 break;

      case 5:
	 for (int i =0;i<4;i++)
	 {
	    block[i] = new blok;
	    block[i]->setcol(OLIVE);
	 }
	 
	 block[2]->setpos(COLUMNS/2,ROWS);
	 block[3]->setpos(COLUMNS/2+1,ROWS);
	 block[1]->setpos(COLUMNS/2-1,ROWS);
	 block[0]->setpos(COLUMNS/2-1,ROWS-1);
	 break;

      case 6:
	 for (int i =0;i<4;i++)
	 {
	    block[i] = new blok;
	    block[i]->setcol(ORANGE);
	 }
	 
	 block[2]->setpos(COLUMNS/2,ROWS);
	 block[3]->setpos(COLUMNS/2+1,ROWS);
	 block[1]->setpos(COLUMNS/2-1,ROWS);
	 block[0]->setpos(COLUMNS/2+1,ROWS-1);
	 break;
   }

}

void selblok::drawall() const
{
   for (int i =0;i<4;i++)
      block[i]->draw();
}

bool selblok::mover(const arrblok &arrblock)
{
   for (int i=0;i<4;i++)
      if (!block[i]->canmover(arrblock))
      {
	 return 1;
      }

   for (int i=0;i<4;i++)
      block[i]->mover();
   
   return 0;
}

bool selblok::movel(const arrblok &arrblock)
{
   for (int i=0;i<4;i++)
      if (!block[i]->canmovel(arrblock))
      {
	 return 1;
      }

   for (int i=0;i<4;i++)
      block[i]->movel();

   return 0;
}

bool selblok::moved(const arrblok &arrblock)
{
   for(int i=0;i<4;i++)
      if (!block[i]->canmoved(arrblock))
      {
	 return 1;
      }

   for(int i=0;i<4;i++)
      block[i]->moved();

   return 0;
}

void selblok::addblock(arrblok *arrblock)
{
   for(int i=0;i<4;i++)
   {
      arrblock->addblock(block[i]);
   }
}

void selblok::rotater(const arrblok  &arrblock)
{
   for (int i = 0;i<4;i++)
      block[i]->rotater(block[2]);

   int bad = 0;
   for (int i =0;i<4;i++)
      if (!block[i]->canrotate(arrblock))
	 bad =1;

   if (bad)
      for (int i =0;i<4;i++)
	 block[i]->rotatel(block[2]);
}

void selblok::rotatel(const arrblok &arrblock)
{
   for (int i = 0;i<4;i++)
      block[i]->rotatel(block[2]);

   int bad = 0;
   for (int i =0;i<4;i++)
      if (!block[i]->canrotate(arrblock))
	 bad =1;

   if (bad)
      for (int i=0;i<4;i++)
	 block[i]->rotater(block[2]);
}
