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

#include <cstdio>
#include <cstdlib>

#include <GL/glew.h>
#include <Magick++.h>

#include "util.h"

int Program;
int PosUniform;
int TextUniform;
int YScaleUniform;

GLuint makeShader(const char *filename,GLuint type);
GLuint makeTexture(const char *filename);
char *  makeSourc(const char *path);

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

   int text0 = maketexture("atlas.svg");
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

GLuint makeShader(const char *filename,GLuint type)
{
   GLuint shader = glCreateShader(type);

   const char * source = makeSource(filename);

   glShaderSource(shader,1,(GLchar **) &source,NULL);
   glCompileShader(shader);
   
   free(source);

   return shader;
}

GLuint makeTexture(const char *filename)
{
   Image image(filename);
   Blob blob;

   image.magick("RGBA");
   image.write(&blob);

   GLuint texture;
   glGenTextures(1,&texture);

   glBindTexture(GL_TEXTURE_2D,texture);
   
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,image.columns(),image.rows(),0,GL_RGBA,GL_UNSIGNED_BYTE,blob.data());

   return texture;
}

char * makeSource(const char *path)
{
   FILE *fd;
   long len, r;
   char *str;
   
   if (!(fd = fopen(path,"r")))
   {
      printf("File open of %s failed\n",path);
      return NULL;
   }

   fseek(fd,0,SEEK_END);
   len = ftell(fd);

   printf("File %s is %ld long\n",path,len);

   fseek(fd,0,SEEK_SET);

   str = (char *) malloc(len * sizeof(char));
   if (!str)
      printf("Malloc failed");

   r = fread(str,sizeof(char),len,fd);

   str[r -1] = '\0';

   fclose(fd);
   
   return str;

}

