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

#include <GL/glew.h>
#include <cstdio>
#include <cmath>
#include <boost/tuple/tuple.hpp>

#include "util.h"
#include "2dadventure.h"

int Program;
int ScaleUniform;
GLuint vao, vbo[3];

//Title: t_player class
//Purpose: Acts as mario
//Interface: can draw, move, and request position

t_player::t_player(float x,float y) : pos(x,y)
{}

void t_player::draw(float xpos) const
{
   glUniform2i(ScaleUniform,1,2);

   GLfloat PosArray[1][2];
   GLfloat TexArray[1][2];

   PosArray[0][0] = (pos.get<0>() - xpos)/10;
   PosArray[0][1] = pos.get<1>() /10;

   TexArray[0][0] = 0;
   TexArray[0][0] = 0;

   glDisableVertexAttribArray(0);
   glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
   glBufferSubData(GL_ARRAY_BUFFER,0,2 * sizeof(GLfloat),PosArray);
   glVertexAttribPointer((GLuint) 0,2,GL_FLOAT,GL_FALSE,0,0);
   glEnableVertexAttribArray(0);
  
   glDisableVertexAttribArray(1);
   glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
   glBufferSubData(GL_ARRAY_BUFFER,0,2 * sizeof(GLfloat),TexArray);
   glVertexAttribPointer((GLuint) 1,2,GL_FLOAT,GL_FALSE,0,0);
   glEnableVertexAttribArray(1);

   glDrawElements(GL_POINTS,1,GL_UNSIGNED_BYTE,0);
}

float t_player::getx() const
{
   return pos.get<0>();
}

float t_player::gety() const
{
   return pos.get<1>();
}

bool t_player::movel(const t_background &/*back*/)
{
   return false;
}   

bool t_player::mover(const t_background &/*back*/)
{
   return false;
}

bool t_player::moveu(const t_background &/*back*/)
{
   return false;
}

bool t_player::moved(const t_background &/*back*/)
{
   return false;
}

//Title: t_background class
//Purpose: Act as the background for the game
//Interface: Can draw and request if a block exists

t_background::t_background() 
{
   board = {{1,1,1,1,1,1,1,1,0},{1},{1},{1},{1},{1},{1},{1},{1},{1}};
}

void t_background::draw(float xpos) const
{
   GLfloat PosArray[20][2];
   GLfloat TexArray[20][2];

   glUniform2i(ScaleUniform,1,1);
   int i ;
   const int *place = board[(int) xpos];
   for (float x = -1 + floor(xpos) - xpos;x<1;x += .1)
   {
      i = 0;
      for (float y = -1;y<1;y += .1)
      {
	 PosArray[i][0] = x;
	 PosArray[i][1] = y;
	 int l = *place++;
	 TexArray[i][0] = 1;
	 TexArray[i][1] = 1-l;
	 //printf("%f %f %d\n",x,y,i);
	 i++;
      }
   
      glDisableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
      glBufferSubData(GL_ARRAY_BUFFER,0,20 * 2 * sizeof(GLfloat),PosArray);
      glVertexAttribPointer((GLuint) 0,2,GL_FLOAT,GL_FALSE,0,0);
      glEnableVertexAttribArray(0);
  
      glDisableVertexAttribArray(1);
      glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
      glBufferSubData(GL_ARRAY_BUFFER,0,20 *2 * sizeof(GLfloat),TexArray);
      glVertexAttribPointer((GLuint) 1,2,GL_FLOAT,GL_FALSE,0,0);
      glEnableVertexAttribArray(1);

      glDrawElements(GL_POINTS,20,GL_UNSIGNED_BYTE,0);
   }
}

bool t_background::checksquare(float xpos,float ypos) const
{
   return board[(int) xpos][(int) ypos] == 1;
}

//Title: t_game class
//Purpose: Abstract the background and player classes
//Interface: Simple moveall and drawall

t_game::t_game() : play(-5,-9)
{
   xpos = 0;
   
   GLuint frag = makeShader("frag",GL_FRAGMENT_SHADER);
   GLuint vert = makeShader("vert",GL_VERTEX_SHADER);
   GLuint geom = makeShader("geom",GL_GEOMETRY_SHADER);

   const GLubyte index[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};

   Program = glCreateProgram();
   glAttachShader(Program,frag);
   glAttachShader(Program,vert);
   glAttachShader(Program,geom);

   glGenVertexArrays(1,&vao);
   glBindVertexArray(vao);

   glGenBuffers(3,vbo);

   glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
   glBufferData(GL_ARRAY_BUFFER,20 * 2 * sizeof(GLfloat),NULL,GL_STREAM_DRAW);
   glVertexAttribPointer((GLuint) 0,2,GL_FLOAT,GL_FALSE,0,0);
   glEnableVertexAttribArray(0);

   glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
   glBufferData(GL_ARRAY_BUFFER,20 * 2 * sizeof(GLfloat),NULL,GL_STREAM_DRAW);
   glVertexAttribPointer((GLuint) 1,2,GL_FLOAT,GL_FALSE,0,0);
   glEnableVertexAttribArray(1);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vbo[2]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER,20 * sizeof(GLbyte),index,GL_STATIC_DRAW);

   glBindAttribLocation(Program, 0, "in_Position");
   glBindAttribLocation(Program, 1, "in_Textcord");

   glLinkProgram(Program);
   glUseProgram(Program);

   int TextUniform = glGetUniformLocation(Program,"Texture");
   ScaleUniform = glGetUniformLocation(Program,"scale");

   int text0 = makeTexture("res/atlas.svg");
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D,text0);
   glUniform1i(TextUniform,0);
   
   char log[1000];
   glGetProgramInfoLog(Program,1000,NULL,log);
   printf("The log is comming in.\n%s",log);
}

void t_game::drawall() const
{
   back.draw(xpos);
   play.draw(xpos);
}

void t_game::moveall()
{
}


int array[20][20] = {{1,1,1,1,1,1,1,1,0},{1},{1},{1},{1},{1},{1},{1},{1},{1}};

void graphicsInit()
{
   GLuint frag = makeShader("frag",GL_FRAGMENT_SHADER);
   GLuint vert = makeShader("vert",GL_VERTEX_SHADER);
   GLuint geom = makeShader("geom",GL_GEOMETRY_SHADER);

   const GLubyte index[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};

   Program = glCreateProgram();
   glAttachShader(Program,frag);
   glAttachShader(Program,vert);
   glAttachShader(Program,geom);

   
   glGenVertexArrays(1,&vao);
   glBindVertexArray(vao);

   glGenBuffers(3,vbo);

   glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
   glBufferData(GL_ARRAY_BUFFER,20 * 2 * sizeof(GLfloat),NULL,GL_STREAM_DRAW);
   glVertexAttribPointer((GLuint) 0,2,GL_FLOAT,GL_FALSE,0,0);
   glEnableVertexAttribArray(0);

   glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
   glBufferData(GL_ARRAY_BUFFER,20 * 2 * sizeof(GLfloat),NULL,GL_STREAM_DRAW);
   glVertexAttribPointer((GLuint) 1,2,GL_FLOAT,GL_FALSE,0,0);
   glEnableVertexAttribArray(1);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vbo[2]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER,20 * sizeof(GLbyte),index,GL_STATIC_DRAW);

   glBindAttribLocation(Program, 0, "in_Position");
   glBindAttribLocation(Program, 1, "in_Textcord");

   glLinkProgram(Program);
   glUseProgram(Program);

   int TextUniform = glGetUniformLocation(Program,"Texture");
   ScaleUniform = glGetUniformLocation(Program,"scale");

   int text0 = makeTexture("res/atlas.svg");
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D,text0);
   glUniform1i(TextUniform,0);
   
   char log[1000];
   glGetProgramInfoLog(Program,1000,NULL,log);
   printf("The log is comming in.\n%s",log);
}

void graphicsTest()
{
   GLfloat TextPos[20][2];
   GLfloat LolPos[20][2];

   glUniform2i(ScaleUniform,1,1);
   int i ;
   int *place = array[0];
   for (float x = -1;x<1;x += .1)
   {
      i = 0;
      for (float y = -1;y<1;y += .1)
      {
	 TextPos[i][0] = x;
	 TextPos[i][1] = y;
	 int l = *place++;
	 LolPos[i][0] = 1;
	 LolPos[i][1] = 1-l;
	 //printf("%f %f %d\n",x,y,i);
	 i++;
      }
   
      glDisableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
      glBufferSubData(GL_ARRAY_BUFFER,0,20 * 2 * sizeof(GLfloat),TextPos);
      glVertexAttribPointer((GLuint) 0,2,GL_FLOAT,GL_FALSE,0,0);
      glEnableVertexAttribArray(0);
  
      glDisableVertexAttribArray(1);
      glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
      glBufferSubData(GL_ARRAY_BUFFER,0,20 *2 * sizeof(GLfloat),LolPos);
      glVertexAttribPointer((GLuint) 1,2,GL_FLOAT,GL_FALSE,0,0);
      glEnableVertexAttribArray(1);

      glDrawElements(GL_POINTS,20,GL_UNSIGNED_BYTE,0);
   }
   
   glUniform2i(ScaleUniform,1,2);

   TextPos[0][0] = -.5;
   TextPos[0][1] = -.9;

   LolPos[0][0] = 0;
   LolPos[0][1] = 0;

   glDisableVertexAttribArray(0);
   glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
   glBufferSubData(GL_ARRAY_BUFFER,0,2 * sizeof(GLfloat),TextPos);
   glVertexAttribPointer((GLuint) 0,2,GL_FLOAT,GL_FALSE,0,0);
   glEnableVertexAttribArray(0);
  
   glDisableVertexAttribArray(1);
   glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
   glBufferSubData(GL_ARRAY_BUFFER,0,2 * sizeof(GLfloat),LolPos);
   glVertexAttribPointer((GLuint) 1,2,GL_FLOAT,GL_FALSE,0,0);
   glEnableVertexAttribArray(1);

   glDrawElements(GL_POINTS,1,GL_UNSIGNED_BYTE,0);

   //glUniform1i(YScaleUniform,2);
   //glUniform2i(TextUniform,0,0);
   //glUniform2f(PosUniform,-.5,-.9);
   //glDrawElements(GL_TRIANGLE_STRIP, 4,GL_UNSIGNED_BYTE,0);
   
   int GLERROR;

   while ((GLERROR = glGetError()))
   {
   if (GLERROR == GL_INVALID_ENUM)
      printf("Invalid enum error");
   else if(GLERROR == GL_INVALID_VALUE)
      printf("Invalid value");
   else if (GLERROR == GL_INVALID_OPERATION)
      printf("Invalid op");   
   else if (GLERROR == GL_OUT_OF_MEMORY)
      printf("Out of mem");
   else 
      printf("Other error");
   }
}

