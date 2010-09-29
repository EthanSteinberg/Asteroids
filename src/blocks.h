#ifndef BLOCKS_H_INCLUDED
#define BLOCKS_H_INCLUDED

#include <boost/tuple/tuple.hpp>

typedef boost::tuple<float, float> cord;
typedef boost::tuple<float, float, float> color;

const int ROWS = 10;
const int COLUMNS = 10;
const float DOWNMOVEMENT = .1;
const float MOVEMENT = .1;

class bord;
class blok;
class arrblok;
class rowblok;
class selblok;

class blok
{
public:
   blok();
   blok(const color &newcol);
   void draw();
   void setpos(const cord &newpos);
   const cord& getpos();
   void setcol(const color &newcol);
   const color& getcol();
   bool exists();
   void movedown();

   void moved();
   void movel();
   void mover();

private:
   color col;
   cord pos;
   bool valid;
};

class rowblok
{
public:
   void drawall();
   bool checkrow();
   bool checksquare(int x);
   void movedown();

private:
   blok block[COLUMNS];
};

class arrblok
{
public:
   void drawall();
   bool checkrow(int y);
   void clearrow(int y);
   bool checksquare(int x,int y);

private:
   rowblok rowblock[ROWS + 1];
};

class selblok
{
public:
   void drawall();
   
   void mover();
   void movel();
   void moved();
   
   void rotater();
   void rotatel();

private:
   blok block[4];
};

class bord
{
public:
   void drawall();
   void moveall();

private:
   selblok selblock;
   arrblok arrblock;
};

#endif //BLOCKS_H_INCLUDED
