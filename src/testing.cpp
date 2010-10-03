#include <cstdio>
#include <cstdlib>
#include <GL/glew.h>
#include "testing.h"
#include <cassert>
#include <Magick++.h>

using namespace Magick;

int array[20][20] = {{1,1,1,1,1,1,1,1,0},{1},{1},{1},{1},{1},{1},{1},{1},{1}};
static int PosUniform;
static int TextUniform;

GLuint makeShader(const char *filename,GLuint type);
GLuint maketexture(const char *filename);

char * file2string(const char *path)
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

GLuint Program;

void testing()
{
   static int PointsToDraw = 4;
  
   int *place = array[0];
   for (float x = -1;x<1;x += .1)
      for (float y = -1;y<1;y += .1)
      {
	 int l = 1 - *place++;
	 glUniform2i(TextUniform,l,1);
         glUniform2f(PosUniform,x,y);
	 glDrawElements(GL_TRIANGLE_STRIP, PointsToDraw, GL_UNSIGNED_BYTE,0);
      }

   glUniform2i(TextUniform,0,0);
   glUniform2f(PosUniform,-.5,-.9);
   glDrawElements(GL_TRIANGLE_STRIP, PointsToDraw,GL_UNSIGNED_BYTE,0);
//      glUniform2i(TextUniform,0,1);
//      glUniform2f(PosUniform,0,0);
//      glDrawElements(GL_TRIANGLE_STRIP,PointsToDraw,GL_UNSIGNED_BYTE,0);
   
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
   if (PointsToDraw == 5)
      PointsToDraw = 2;
}

void testingInit()
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

   char log[1000];
   glGetShaderInfoLog(frag,1000,NULL,log);
   printf("The log is comming in.\n%s",log);
}

GLuint makeShader(const char *filename,GLuint type)
{
   GLuint shader = glCreateShader(type);

   const GLchar *lol = (GLchar *) file2string(filename);

   glShaderSource(shader,1,&lol,NULL);
   glCompileShader(shader);
   
   free((char *) lol);

   return shader;
}

GLuint maketexture(const char *filename)
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
