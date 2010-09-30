#include <blocks.h>
#include <GL/gl.h>
#include <iostream>
#include <cassert>
#include <cmath>
#include <cstdlib>

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
   int hit = 0;
   selblock->moved(arrblock,&hit);
   if (PressedKeys[0])
      selblock->movel(arrblock,&hit);
   if (PressedKeys[1])
      selblock->mover(arrblock,&hit);

   if (hit)
   {
      selblock->addblock(&arrblock);
      delete selblock;
      int lol = std::rand() % 4;
      std::cout<<lol<<std::endl;
      selblock = new selblok(lol);
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

bool blok::canmoved(const arrblok &arrblock, int *hit) const
{
   return !((*hit += !(pos.get<1>() - 4 * DOWNMOVEMENT > 0)) || (*hit += arrblock.checksquare(pos.get<0>() + .5,pos.get<1>())) );
}

bool blok::canmovel(const arrblok &arrblock, int *hit) const
{
   return (pos.get<0>() - MOVEMENT > 0) && !(*hit += arrblock.checksquare(pos.get<0>(),pos.get<1>() + .5));
}

bool blok::canmover(const arrblok &arrblock, int *hit) const
{
   return ((pos.get<0>() + MOVEMENT + 1) < COLUMNS) && !(*hit += arrblock.checksquare(pos.get<0>() + 1,pos.get<1>() + .5));
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
   }

}

void selblok::drawall() const
{
   for (int i =0;i<4;i++)
      block[i]->draw();
}

bool selblok::mover(const arrblok &arrblock,int *hit)
{
   for (int i=0;i<4;i++)
      if (!block[i]->canmover(arrblock,hit))
      {
	 return 1;
      }

   for (int i=0;i<4;i++)
      block[i]->mover();
   
   return 0;
}

bool selblok::movel(const arrblok &arrblock,int *hit)
{
   for (int i=0;i<4;i++)
      if (!block[i]->canmovel(arrblock,hit))
      {
	 return 1;
      }

   for (int i=0;i<4;i++)
      block[i]->movel();

   return 0;
}

bool selblok::moved(const arrblok &arrblock,int *hit)
{
   for(int i=0;i<4;i++)
      if (!block[i]->canmoved(arrblock,hit))
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
