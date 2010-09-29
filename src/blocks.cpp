//Class Bord
//Container for other classes with general function

void bord::drawall()
{
   selblock.drawall();
   arrblock.drawall();
}

void bord::moveall()
{
}

//Class Blok
//Holds color and position with a small wrapper

void blok::draw();
{
   glColor(col.get<0>(), col.get<1>(), col.get<2>());
   glrect2f(pos.get<0>(), pos.get<1>(), pos.get<0>() + 1, pos.get<1>() + 1);
}

void blok::setpos(const cord &newpos)
{
   pos = newpos;
}

const cord& blok::getpos()
{
   return pos;
}

void blok::setcol(const color &newcol)
{
   col = newcol;
}

const color& blok::getcol()
{
   return col;
}

bool blok::exists()
{
   return col != color(0,0,0);
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
      rowblocki[i] = rowblock[i+1];
}

bool arrblok::checkrow(int x,int y)
{
   return rowblock[y].checksquare(x);
}

//Class RowBlok
//Holds an array of blocks

void rowblok::drawall()
{
   for (int i = 0;i<COLUNMS;i++)
      block[i].draw();
}

bool rowblok::checkrow()
{
   for (int i = 0;i<COLUNMS;i++)
      if (!block[i].exists())
	 return 0;

   return 1;
}

bool rowblok::checksquare(int x)
{
   return block[x].exists();
}

