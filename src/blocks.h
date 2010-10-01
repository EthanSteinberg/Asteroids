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
   void draw() const;
   void setpos(double x,double y);
   double getx() const;
   double gety() const;
   void setcol(const color &newcol);
   const color& getcol() const;
   void movedown();

   void moved();
   void movel();
   void mover();

   bool canmoved(const arrblok &arrblock) const;
   bool canmovel(const arrblok &arrblock) const;
   bool canmover(const arrblok &arrblock) const;

   void rotater(const blok *origin);
   void rotatel(const blok *origin);

private:
   color col;
   cord pos;
};

class rowblok
{
public:
   rowblok();
   void drawall() const;
   bool checkrow() const;
   bool checksquare(int x) const;
   void addblock(blok *newblock);
   void movedown();

private:
   boost::array<blok*,COLUMNS> block;
};

class arrblok
{
public:
   void drawall() const;
   bool checkrow(int y) const;
   void clearrow(int y);
   bool checksquare(int x,int y) const;
   void addblock(blok *newblock);

private:
   boost::array<rowblok,ROWS + 1> rowblock;
};

class selblok
{
public:
   selblok(int type);
   void drawall() const;
   void addblock(arrblok *arrblock);

   bool mover(const arrblok &arrblock);
   bool movel(const arrblok &arrblock);
   bool moved(const arrblok &arrblock);
   
   void rotater(const arrblok &arrblock);
   void rotatel(const arrblok &arrblock);

private:
   boost::array<blok*,4> block;
};

class bord
{
public:
   bord();
   void drawall() const;
   void moveall();

private:
   selblok *selblock;
   arrblok arrblock;
};

#endif //BLOCKS_H_INCLUDED


