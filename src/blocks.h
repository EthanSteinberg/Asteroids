#ifndef BLOCKS_H_INCLUDED
#define BLOCKS_H_INCLUDED

#include <boost/tuple/tuple.hpp>
#include <boost/array.hpp>

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
   blok(double x,double y, const color &newcol);
   void draw();
   void setpos(double x,double y);
   const cord& getpos();
   void setcol(const color &newcol);
   const color& getcol();
   void init();
   bool exists();
   void movedown();

   void moved();
   void movel();
   void mover();

   bool canmoved(const arrblok &arrblock);
   bool canmovel(const arrblok &arrblock);
   bool canmover(const arrblok &arrblock);

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
   boost::array<blok,COLUMNS> block;
};

class arrblok
{
public:
   void drawall();
   bool checkrow(int y);
   void clearrow(int y);
   bool checksquare(int x,int y);

private:
   boost::array<rowblok,ROWS + 1> rowblock;
};

class selblok
{
public:
   selblok(int type);
   void drawall();
   
   void mover(const arrblok &arrblock);
   void movel(const arrblok &arrblock);
   void moved(const arrblok &arrblock);
   
   void rotater();
   void rotatel();

private:
   boost::array<blok,4> block;
};

class bord
{
public:
   bord();
   void drawall();
   void moveall();

private:
   selblok *selblock;
   arrblok arrblock;
};

#endif //BLOCKS_H_INCLUDED
