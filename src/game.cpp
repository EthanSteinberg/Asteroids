#include <GL/glew.h>

#include <SFML/Window.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>

#include <cmath>
#include <cstdio>

#include "game.h"
#include "util.h"
#include "const.h"

const double PI = 3.14159265;

extern sf::Input *Input;
extern int ScaleUniform;
extern int vbo[4];
extern int TextScaleUniform;
extern int RotateUniform;

//Class: t_rocks
//Purpose: Hold rocks
//Interface: moveall, drawall, add

void t_rocks::drawall() const
{
   for (unsigned int i = 0;i < rock.size();i++)
      rock[i]->draw();
}

void t_rocks::moveall(const boost::posix_time::time_duration &time)
{
   for (unsigned int i = 0;i < rock.size();i++)
      rock[i]->move(time);
}

void t_rocks::add(t_rock *newRock)
{
   rock.push_back(newRock);
}

size_t t_rocks::size() const
{
   return rock.size();
}

bool t_rocks::collided(const t_bullets &bullets)
{
   for (unsigned int l =0;l<size();l++)
   {
      for (unsigned int  i = 0;i<bullets.size();i++)
      {
	 if (distance(bullets,l,i) < .15 * .15)
	    return true;
      }
   }
   return false;
}

float t_rocks::distance(const t_bullets &bullets,int l, int i) const
{
   return (bullets.getPosition(i).x - rock[l]->getPosition().x) * (bullets.getPosition(i).x - rock[l]->getPosition().x)
        + (bullets.getPosition(i).y - rock[l]->getPosition().y) * (bullets.getPosition(i).y - rock[l]->getPosition().y);
}

//Class: t_rock
//Purpose: Be a rock and move
//Interface: moevup,movedown, turnleft,turnright draw

t_rock::t_rock(vec2 startpos, float startdirection)
{
   pos = startpos;
   direction = startdirection;
}

void t_rock::draw() const
{
   glUniform2f(TextScaleUniform,TEXTX,TEXTY);
   glUniform2f(ScaleUniform,.15,.15);
   glUniform1f(RotateUniform,direction);

   float PosArray[1][2];
   float TexArray[1][2];
   float ScaArray[1][2];

   PosArray[0][0] = pos.x;
   PosArray[0][1] = pos.y;

   TexArray[0][0] = 2;
   TexArray[0][1] = 0;
      
   ScaArray[0][0] = 1;
   ScaArray[0][1] = 1;

   glDisableVertexAttribArray(0);
   glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
   glBufferSubData(GL_ARRAY_BUFFER,0,1 * 2 * sizeof(GLfloat),PosArray);
   glVertexAttribPointer((GLuint) 0,2,GL_FLOAT,GL_FALSE,0,0);
   glEnableVertexAttribArray(0);
  
   glDisableVertexAttribArray(1);
   glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
   glBufferSubData(GL_ARRAY_BUFFER,0,1 * 2 * sizeof(GLfloat),TexArray);
   glVertexAttribPointer((GLuint) 1,2,GL_FLOAT,GL_FALSE,0,0);
   glEnableVertexAttribArray(1);

   glDisableVertexAttribArray(2);
   glBindBuffer(GL_ARRAY_BUFFER,vbo[2]);
   glBufferSubData(GL_ARRAY_BUFFER,0,1 * 2* sizeof(GLfloat),ScaArray);
   glVertexAttribPointer((GLuint) 2,2,GL_FLOAT,GL_FALSE,0,0);
   glEnableVertexAttribArray(2);
   
   glDrawElements(GL_POINTS,1,GL_UNSIGNED_BYTE,0);
   glUniform1f(RotateUniform,0);
}

void t_rock::move(const boost::posix_time::time_duration &time)
{
   pos.x += cos((direction + 90) * PI/180) * ROCKSPEED * time.total_microseconds()/100000000.0;
   pos.y += sin((direction + 90) * PI/180) * ROCKSPEED * time.total_microseconds()/100000000.0;

   if (fabs(pos.x) > 1.1)
      pos.x = -pos.x;

   if (fabs(pos.y) > 1.1)
      pos.y = -pos.y;
}

vec2 t_rock::getPosition() const
{
   return pos;
}

//Class: t_bullets
//Purpose: Hold bullets
//Interface: moveall, drawall, add

void t_bullets::drawall() const
{
   for (unsigned int i = 0;i < bullet.size();i++)
      bullet[i]->draw();
}

void t_bullets::moveall(const boost::posix_time::time_duration &time)
{
   for (unsigned int i = 0;i < bullet.size();i++)
      bullet[i]->move(time);
}

void t_bullets::add(t_bullet *newBullet)
{
   bullet.push_back(newBullet);
}

size_t t_bullets::size() const
{
   return bullet.size();
}

vec2 t_bullets::getPosition(int i) const
{
   return bullet[i]->getPosition();
}

//Class: t_bullet
//Purpose: Be a bullet and move
//Interface: moevup,movedown, turnleft,turnright draw

t_bullet::t_bullet(vec2 startpos, float startdirection)
{
   pos = startpos;
   direction = startdirection;
}

void t_bullet::draw() const
{
   glUniform2f(TextScaleUniform,TEXTX,TEXTY);
   glUniform2f(ScaleUniform,.15,.15);
   glUniform1f(RotateUniform,direction);

   float PosArray[1][2];
   float TexArray[1][2];
   float ScaArray[1][2];

   PosArray[0][0] = pos.x;
   PosArray[0][1] = pos.y;

   TexArray[0][0] = 0;
   TexArray[0][1] = 0;
      
   ScaArray[0][0] = 1;
   ScaArray[0][1] = 1;

   glDisableVertexAttribArray(0);
   glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
   glBufferSubData(GL_ARRAY_BUFFER,0,1 * 2 * sizeof(GLfloat),PosArray);
   glVertexAttribPointer((GLuint) 0,2,GL_FLOAT,GL_FALSE,0,0);
   glEnableVertexAttribArray(0);
  
   glDisableVertexAttribArray(1);
   glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
   glBufferSubData(GL_ARRAY_BUFFER,0,1 * 2 * sizeof(GLfloat),TexArray);
   glVertexAttribPointer((GLuint) 1,2,GL_FLOAT,GL_FALSE,0,0);
   glEnableVertexAttribArray(1);

   glDisableVertexAttribArray(2);
   glBindBuffer(GL_ARRAY_BUFFER,vbo[2]);
   glBufferSubData(GL_ARRAY_BUFFER,0,1 * 2* sizeof(GLfloat),ScaArray);
   glVertexAttribPointer((GLuint) 2,2,GL_FLOAT,GL_FALSE,0,0);
   glEnableVertexAttribArray(2);
   
   glDrawElements(GL_POINTS,1,GL_UNSIGNED_BYTE,0);
   glUniform1f(RotateUniform,0);
}

void t_bullet::move(const boost::posix_time::time_duration &time)
{
   pos.x += cos((direction + 90) * PI/180) * BULLETSPEED * time.total_microseconds()/100000000.0;
   pos.y += sin((direction + 90) * PI/180) * BULLETSPEED * time.total_microseconds()/100000000.0;

   if (fabs(pos.x) > 1.1)
      pos.x = -pos.x;

   if (fabs(pos.y) > 1.1)
      pos.y = -pos.y;
}

vec2 t_bullet::getPosition() const
{
   return pos;
}

//Class: t_background
//Purpose: Draw a background
//Interface: drawall

void t_background::draw() const
{
   glUniform2f(TextScaleUniform,TEXTX,TEXTY);
   glUniform2f(ScaleUniform,2,2);

   float PosArray[1][2];
   float TexArray[1][2];
   float ScaArray[1][2];

   PosArray[0][0] = -1;
   PosArray[0][1] = -1;

   TexArray[0][0] = 0;
   TexArray[0][1] = 1;
      
   ScaArray[0][0] = 1;
   ScaArray[0][1] = 1;

   glDisableVertexAttribArray(0);
   glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
   glBufferSubData(GL_ARRAY_BUFFER,0,1 * 2 * sizeof(GLfloat),PosArray);
   glVertexAttribPointer((GLuint) 0,2,GL_FLOAT,GL_FALSE,0,0);
   glEnableVertexAttribArray(0);
  
   glDisableVertexAttribArray(1);
   glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
   glBufferSubData(GL_ARRAY_BUFFER,0,1 * 2 * sizeof(GLfloat),TexArray);
   glVertexAttribPointer((GLuint) 1,2,GL_FLOAT,GL_FALSE,0,0);
   glEnableVertexAttribArray(1);

   glDisableVertexAttribArray(2);
   glBindBuffer(GL_ARRAY_BUFFER,vbo[2]);
   glBufferSubData(GL_ARRAY_BUFFER,0,1 * 2* sizeof(GLfloat),ScaArray);
   glVertexAttribPointer((GLuint) 2,2,GL_FLOAT,GL_FALSE,0,0);
   glEnableVertexAttribArray(2);
   
   glDrawElements(GL_POINTS,1,GL_UNSIGNED_BYTE,0);
}

//Class: t_ship
//Purpose: Be a ship, move, rotate, etc
//Interface: moevup,movedown, turnleft,turnright draw

t_ship::t_ship()
{
   pos.x = 0;
   pos.y = 0;

   direction = 0;
}

void t_ship::draw() const
{
   glUniform2f(TextScaleUniform,TEXTX,TEXTY);
   glUniform2f(ScaleUniform,.15,.15);
   glUniform1f(RotateUniform,direction);

   float PosArray[1][2];
   float TexArray[1][2];
   float ScaArray[1][2];

   PosArray[0][0] = pos.x;
   PosArray[0][1] = pos.y;

   TexArray[0][0] = 1;
   TexArray[0][1] = 1;
      
   ScaArray[0][0] = 1;
   ScaArray[0][1] = 1;

   glDisableVertexAttribArray(0);
   glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
   glBufferSubData(GL_ARRAY_BUFFER,0,1 * 2 * sizeof(GLfloat),PosArray);
   glVertexAttribPointer((GLuint) 0,2,GL_FLOAT,GL_FALSE,0,0);
   glEnableVertexAttribArray(0);
  
   glDisableVertexAttribArray(1);
   glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
   glBufferSubData(GL_ARRAY_BUFFER,0,1 * 2 * sizeof(GLfloat),TexArray);
   glVertexAttribPointer((GLuint) 1,2,GL_FLOAT,GL_FALSE,0,0);
   glEnableVertexAttribArray(1);

   glDisableVertexAttribArray(2);
   glBindBuffer(GL_ARRAY_BUFFER,vbo[2]);
   glBufferSubData(GL_ARRAY_BUFFER,0,1 * 2* sizeof(GLfloat),ScaArray);
   glVertexAttribPointer((GLuint) 2,2,GL_FLOAT,GL_FALSE,0,0);
   glEnableVertexAttribArray(2);
   
   glDrawElements(GL_POINTS,1,GL_UNSIGNED_BYTE,0);
   glUniform1f(RotateUniform,0);
}

void t_ship::moveup(const boost::posix_time::time_duration &time)
{
   pos.x += cos((direction + 90) * PI/180) * MOVEDISTANCE * time.total_microseconds()/100000000.0;
   pos.y += sin((direction + 90) * PI/180) * MOVEDISTANCE * time.total_microseconds()/100000000.0;

   if (fabs(pos.x) > 1.1)
      pos.x = -pos.x;

   if (fabs(pos.y) > 1.1)
      pos.y = -pos.y;
}

void t_ship::movedown(const boost::posix_time::time_duration &time)
{
   pos.x -= cos((direction + 90) * PI/180) * MOVEDISTANCE * time.total_microseconds()/100000000.0;
   pos.y -= sin((direction + 90) * PI/180) * MOVEDISTANCE * time.total_microseconds()/100000000.0;
   
   if (fabs(pos.x) > 1.2)
      pos.x = -pos.x;

   if (fabs(pos.y) > 1.2)
      pos.y = -pos.y;
}

void t_ship::turnleft(const boost::posix_time::time_duration &time)
{
   direction += TURNDISTANCE * time.total_microseconds()/100000000.0;
}

void t_ship::turnright(const boost::posix_time::time_duration &time)
{
   direction -= TURNDISTANCE * time.total_microseconds()/100000000.0;
}

t_bullet* t_ship::shoot() const
{
   return new t_bullet(pos,direction);
}

//Class: t_game
//Purpose: Be an asteroid game
//Interface: moveall and drawall

t_game::t_game()
{
   vec2 temp = {-.5,-.5};
   rocks.add(new t_rock(temp,30));
}

void t_game::drawall() const
{
   static boost::posix_time::ptime time(boost::posix_time::microsec_clock::universal_time());
   static int lols = 0;
   static char lol[50];

   //background.draw();   I need to see FPS   
   ship.draw();

   bullets.drawall();
   rocks.drawall();

   if (lols++ == 10)
   {
      sprintf(lol,"The FPS is %f\n",10000000.0 / (boost::posix_time::microsec_clock::universal_time() -time).total_microseconds());

      time = boost::posix_time::microsec_clock::universal_time();
      lols = 0;
   }
   
   drawText(lol,-1,-1);
}

bool t_game::moveall()
{
   static boost::posix_time::ptime time(boost::posix_time::microsec_clock::universal_time());
   static boost::posix_time::ptime lastPushed(boost::posix_time::microsec_clock::universal_time());

   if (Input->IsKeyDown(sf::Key::A))
      ship.turnleft(boost::posix_time::microsec_clock::universal_time() -time);
   
   if (Input->IsKeyDown(sf::Key::S))
      ship.turnright(boost::posix_time::microsec_clock::universal_time() -time);
   
   if (Input->IsKeyDown(sf::Key::W))
      ship.moveup(boost::posix_time::microsec_clock::universal_time() -time);
   
   if (Input->IsKeyDown(sf::Key::R))
      ship.movedown(boost::posix_time::microsec_clock::universal_time() -time);

   if (Input->IsKeyDown(sf::Key::Space) && boost::posix_time::microsec_clock::universal_time() > lastPushed)
      bullets.add(ship.shoot()),lastPushed = boost::posix_time::microsec_clock::universal_time() + boost::posix_time::milliseconds(1000);
   
   bullets.moveall(boost::posix_time::microsec_clock::universal_time() -time);
   rocks.moveall(boost::posix_time::microsec_clock::universal_time() -time);

   vec2 temp = {0,0};

   if (rocks.collided(bullets))
      rocks.add(new t_rock(temp,90));

   time = boost::posix_time::microsec_clock::universal_time();
   
   if (Input->IsKeyDown(sf::Key::D))
      printf("There are %d ships\n",static_cast<int>(bullets.size()));
   return Input->IsKeyDown(sf::Key::Escape);
}

