#include <blocks.h>
#include <GL/gl.h>

//Class Bord
//Container for other classes with general function

bord::bord()
{
   selblock = new selblok(0);
}

void bord::drawall()
{
   selblock->drawall();
   arrblock.drawall();
}

void bord::moveall()
{
   selblock->moved(arrblock);
}

//Class Blok
//Holds color and position with a small wrapper

void blok::draw()
{
   glColor3f(col.get<0>(), col.get<1>(), col.get<2>());
   glRectf(pos.get<0>(), pos.get<1>(), pos.get<0>() + 1, pos.get<1>() + 1);
}

void blok::setpos(double x,double y)
{
   pos.get<0>() = x,pos.get<1>() = y;
}

double blok::getx()
{
   return pos.get<0>();
}

double blok::gety()
{
   return pos.get<1>();
}

void blok::setcol(const color &newcol)
{
   col = newcol;
}

const color& blok::getcol()
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

bool blok::canmoved(const arrblok &arrblock)
{
   return pos.get<1>() - DOWNMOVEMENT > 0;
}

bool blok::canmovel(const arrblok &arrblock)
{
   return pos.get<0>() - MOVEMENT > 0;
}

bool blok::canmover(const arrblok &arrblock)
{
   return pos.get<0>() + MOVEMENT < COLUMNS;
}

//Class ArrBlok
//Holds an array of RowBlocks

void arrblok::drawall()
{
   for (int i = 0;i<ROWS;i++)
      rowblock[i].drawall();
}

bool arrblok::checkrow(int y)
{
   return rowblock[y].checkrow();
}

void arrblok::clearrow(int y)
{
   for (int i =y;i<ROWS;i++)
      rowblock[i] = rowblock[i+1],rowblock[i].movedown();
}

bool arrblok::checksquare(int x,int y)
{
   return rowblock[y].checksquare(x);
}

void arrblok::addblock(blok *newblock)
{
   rowblock[(int) newblock->gety()].addblock(newblock);
}

//Class RowBlok
//Holds an array of blocks

rowblok::rowblok() 
{
   boost::array<blok*,COLUMNS> block = {};
}

void rowblok::drawall()
{
   for (int i = 0;i<COLUMNS;i++)
      if (block[i])
	 block[i]->draw();
}

bool rowblok::checkrow()
{
   for (int i = 0;i<COLUMNS;i++)
      if (!block[i])
	 return 0;

   return 1;
}

bool rowblok::checksquare(int x)
{
   return block[x];
}

void rowblok::addblock(blok *newblock)
{
   block[(int) newblock->getx()] = newblock;
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

   for (int i = 0;i<4;i++)
      block[i].setcol(GREEN);


   block[2].setpos(COLUMNS/2,ROWS);
   block[3].setpos(COLUMNS/2 +1,ROWS);
   block[1].setpos(COLUMNS/2,ROWS -1);
   block[0].setpos(COLUMNS/2 -1,ROWS -1);
}

void selblok::drawall()
{
   for (int i =0;i<4;i++)
      block[i].draw();
}

void selblok::mover(const arrblok &arrblock)
{
   for (int i=0;i<4;i++)
      if (!block[i].canmover(arrblock))
	 return;

   for (int i=0;i<4;i++)
      block[i].moved();
}

void selblok::movel(const arrblok &arrblock)
{
   for (int i=0;i<4;i++)
      if (!block[i].canmovel(arrblock))
	 return;

   for (int i=0;i<4;i++)
      block[i].movel();
}

void selblok::moved(const arrblok &arrblock)
{
   for(int i=0;i<4;i++)
      if (!block[i].canmoved(arrblock))
	 return;

   for(int i=0;i<4;i++)
      block[i].moved();
}
