#ifndef BLOCKS_H_INCLUDED
#define BLOCKS_H_INCLUDED

#include <boost/tuple.hpp>

typename boost::tuple<float, float> cord;
typename boost::tuple<float, float, float> color;

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
   void setpos(const cord &newpos);
   const cord& getpos();
   void setcol(const color &newcol);
   const color& getcol();
   bool exists();

private:
   color col;
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
   rowblok rowblock[ROWS + 1];
}

class rowblok
{
public
   void drawall();
   bool checkrow();
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
