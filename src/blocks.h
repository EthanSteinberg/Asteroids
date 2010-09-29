#ifndef BLOCKS_H_INCLUDED
#define BLOCKS_H_INCLUDED

#include <boost/tuple.hpp>

typename boost::tuple<float, float> cord;

class bord
{
public:
   void drawall();
   void moveall();

private:
   selblok selblock;
   arrblok arrblock;
}

class blok
{
public:
   void draw();
   void setpos(const cord &pos);
   void setcol(int col);
   int getcol();
   const cord& getpos();

private:
   int color;
   cord pos;
}

class arrblok
{
public:
   void drawall();
   bool checkrow(int y);
   void clearrow(int y);
   bool checksquare(int x,int y);

private:
   rowblok rowblock[ROWS];
}

class rowblok
{
public
   void drawall();
   bool checkrow();
   void clearrow();
   bool checksquare(int x);

private:
   blok block[COLUNMS];
}

class selblok
{
public:
   drawall();
   
   mover();
   movel();
   moved();
   
   rotater();
   rotatel();

private:
   blok block[4];
}
