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

#include "util.h"
#include "2dadventure.h"

int Program;
int PosUniform;
int TextUniform;
int YScaleUniform;
GLuint vao, vbo[3];

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

   int text0 = makeTexture("res/atlas.svg");
   PosUniform = glGetUniformLocation(Program,"Texture");

   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D,text0);
   glUniform1i(PosUniform,0);
   
   char log[1000];
   glGetProgramInfoLog(Program,1000,NULL,log);
   printf("The log is comming in.\n%s",log);
}

void graphicsTest()
{
   float TextPos[20][2];
   float LolPos[20][2];

   int i = 0;
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
	 LolPos[i][1] = l;
	 i++;
      }
   
      glDisableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
      glBufferSubData(GL_ARRAY_BUFFER,0,20 * 2 * sizeof(float),TextPos);
      glVertexAttribPointer((GLuint) 0,2,GL_FLOAT,GL_FALSE,0,0);
      glEnableVertexAttribArray(0);
  
      glDisableVertexAttribArray(1);
      glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
      glBufferSubData(GL_ARRAY_BUFFER,0,20 *2 * sizeof(float),LolPos);
      glVertexAttribPointer((GLuint) 1,2,GL_FLOAT,GL_FALSE,0,0);
      glEnableVertexAttribArray(1);

      glDrawElements(GL_POINTS,20,GL_UNSIGNED_BYTE,0);
   }
   
   TextPos[0][0] = -.5;
   TextPos[0][1] = -.9;

   TextPos[0][0] = 0;
   TextPos[0][1] = 0;

   glDisableVertexAttribArray(0);
   glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
   glBufferSubData(GL_ARRAY_BUFFER,0,2 * sizeof(float),TextPos);
   glVertexAttribPointer((GLuint) 0,2,GL_FLOAT,GL_FALSE,0,0);
   glEnableVertexAttribArray(0);
  
   glDisableVertexAttribArray(1);
   glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
   glBufferSubData(GL_ARRAY_BUFFER,0,2 * sizeof(float),LolPos);
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

