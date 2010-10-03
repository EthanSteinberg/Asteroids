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

int array[20][20] = {{1,1,1,1,1,1,1,1,0},{1},{1},{1},{1},{1},{1},{1},{1},{1}};

void graphicsInit()
{
   GLuint frag = makeShader("frag",GL_FRAGMENT_SHADER);
   GLuint vert = makeShader("vert",GL_VERTEX_SHADER);

   Program = glCreateProgram();
   glAttachShader(Program,frag);
   glAttachShader(Program,vert);

   GLuint vao, vbo[3];

   const GLfloat square[5][2]= {
	    {  0.0,   0.0 },
	    {  0.0,   0.1 } , /* Top point */
	    {  0.1,   0.1 } , /* Right point */
	    {  0.1,   0.0 } , /* Bottom point */
	    {  0.0,   0.0 }  }; /* Left point */

   const GLfloat colors[5][3] = {
	    { 0.0,  0.0, 0.0 },
	    {  1.0,  0.0,  0.0  }, /* Red */
	    {  0.0,  1.0,  0.0  }, /* Green */
	    {  0.0,  0.0,  1.0  }, /* Blue */
	    {  1.0,  1.0,  1.0  }, }; /* White */

   const GLubyte index[7] = {4,3,1,2,4};

   glGenVertexArrays(1,&vao);
   glBindVertexArray(vao);

   glGenBuffers(3,vbo);

   glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
   glBufferData(GL_ARRAY_BUFFER,10 * sizeof(GLfloat),square,GL_STATIC_DRAW);
   glVertexAttribPointer((GLuint) 0,2,GL_FLOAT,GL_FALSE,0,0);
   glEnableVertexAttribArray(0);

   glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
   glBufferData(GL_ARRAY_BUFFER,15 * sizeof(GLfloat),colors,GL_STATIC_DRAW);
   glVertexAttribPointer((GLuint) 1,3,GL_FLOAT,GL_FALSE,0,0);
   glEnableVertexAttribArray(1);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vbo[2]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER,7 * sizeof(GLbyte),index,GL_STATIC_DRAW);

   glBindAttribLocation(Program, 0, "in_Position");
   glBindAttribLocation(Program, 1, "in_Color");

   glLinkProgram(Program);
   glUseProgram(Program);

   int text0 = makeTexture("res/atlas.svg");
   PosUniform = glGetUniformLocation(Program,"MyTexture");

   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D,text0);
   glUniform1i(PosUniform,0);
   
   PosUniform = glGetUniformLocation(Program,"bottomLeft");
   TextUniform = glGetUniformLocation(Program,"text");
   YScaleUniform = glGetUniformLocation(Program,"YScale");

   char log[1000];
   glGetShaderInfoLog(frag,1000,NULL,log);
   printf("The log is comming in.\n%s",log);
}

void graphicsTest()
{
   glUniform1i(YScaleUniform,1);  
   
   int *place = array[0];
   for (float x = -1;x<1;x += .1)
      for (float y = -1;y<1;y += .1)
      {
	 int l = 1 - *place++;
	 glUniform2i(TextUniform,1,l);
         glUniform2f(PosUniform,x,y);
	 glDrawElements(GL_TRIANGLE_STRIP,4, GL_UNSIGNED_BYTE,0);
      }

   glUniform1i(YScaleUniform,2);
   glUniform2i(TextUniform,0,0);
   glUniform2f(PosUniform,-.5,-.9);
   glDrawElements(GL_TRIANGLE_STRIP, 4,GL_UNSIGNED_BYTE,0);
   
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

