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
#include <fstream>

#include <boost/scoped_ptr.hpp>
#include <GL/glew.h>
#include <Magick++.h>
#include <yajl/yajl_parse.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include "util.h"

struct ivec2
{
   int x;
   int y;
};

extern int vbo[4];
extern int TextScaleUniform;
extern int TextUniform;
extern int ScaleUniform;

int width;
int height;

FT_Face face;
FT_Library library;
FT_BitmapGlyph glyphs[96];
ivec2 glyphsPos[96];

GLuint makeShader(const char *filename,GLuint type)
{
   GLuint shader = glCreateShader(type);

   boost::scoped_ptr<char> source(makeSource(filename));

   glShaderSource(shader,1,(const GLchar **) &source,NULL);
   glCompileShader(shader);

   return shader;
}

GLuint makeTexture(const char *filename)
{
   Magick::Image image(filename);
   Magick::Blob blob;

   image.magick("RGBA");
   image.write(&blob);

   GLuint texture;
   glGenTextures(1,&texture);

   glBindTexture(GL_TEXTURE_2D,texture);
   
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
   //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
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

int loadint(void *ctx,long integer)
{
   *(*(int **) ctx)++ = integer;
   return 1;
}

yajl_callbacks call= {NULL,NULL,loadint,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
yajl_parser_config conf = {0,1};

void jsonfill(const char *filename,int *array)
{
   std::ifstream fin(filename);
   fin.seekg(0,std::ios::end);
   int length = fin.tellg();
   fin.seekg(0,std::ios::beg);
   unsigned char *buffer = new unsigned char  [length];
   fin.read ((char *) buffer,length);
   fin.close();

   yajl_handle handl = yajl_alloc(&call,&conf,NULL,&array);
   yajl_parse(handl,buffer,length);
   yajl_free(handl);

   free(buffer);
}

int nextPowerOfTwo(int num)
{
   int x = 1;
   while (x < num)
      x *= 2;

   return x;
}

void fontInit(int size,int dpi)
{
   FT_Init_FreeType(&library);

   FT_New_Face(library,"res/DejaVuSerif.ttf",0,&face);

   FT_Set_Char_Size(face,0,size *64,0,dpi);
}

void loadGlyph(char letter)
{
   int glyph_index = FT_Get_Char_Index( face, letter);

   FT_Load_Glyph(face,glyph_index,FT_LOAD_RENDER);
   
   FT_Render_Glyph(face->glyph,FT_RENDER_MODE_NORMAL);

   FT_Get_Glyph(face->glyph,(FT_Glyph *) &glyphs[letter -32]);
}

void drawText(const char *string,float startx,float starty)
{
   float PosArray[20][2];
   float TexArray[20][2];
   float ScaArray[20][2];

   int count = 0;
  
   glUniform1i(TextUniform,1);
   glUniform2f(TextScaleUniform,width,height);
   glUniform2f(ScaleUniform,0.005,0.005);
   
   while(*string && count < 20)
   {
      FT_BBox bbox;

      FT_Glyph_Get_CBox( (FT_Glyph) glyphs[*string -32], FT_GLYPH_BBOX_PIXELS, &bbox );

      PosArray[count][0] = startx;
      PosArray[count][1] = starty + (int) bbox.yMin/200.0;

      TexArray[count][0] = glyphsPos[*string -32].x;
      TexArray[count][1] = glyphsPos[*string -32].y;
      
      ScaArray[count][0] = glyphs[*string -32]->bitmap.width;
      ScaArray[count][1] = glyphs[*string -32]->bitmap.rows;

      startx += ((FT_Glyph) glyphs[*string -32])->advance.x/(65532.0) * 0.005;
      
      string++, count++;
   }

   glDisableVertexAttribArray(0);
   glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
   glBufferSubData(GL_ARRAY_BUFFER,0,20 * 2 * sizeof(GLfloat),PosArray);
   glVertexAttribPointer((GLuint) 0,2,GL_FLOAT,GL_FALSE,0,0);
   glEnableVertexAttribArray(0);
  
   glDisableVertexAttribArray(1);
   glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
   glBufferSubData(GL_ARRAY_BUFFER,0,20 * 2 * sizeof(GLfloat),TexArray);
   glVertexAttribPointer((GLuint) 1,2,GL_FLOAT,GL_FALSE,0,0);
   glEnableVertexAttribArray(1);

   glDisableVertexAttribArray(2);
   glBindBuffer(GL_ARRAY_BUFFER,vbo[2]);
   glBufferSubData(GL_ARRAY_BUFFER,0,20 * 2* sizeof(GLfloat),ScaArray);
   glVertexAttribPointer((GLuint) 2,2,GL_FLOAT,GL_FALSE,0,0);
   glEnableVertexAttribArray(2);
   
   glDrawElements(GL_POINTS,count,GL_UNSIGNED_BYTE,0);
   
   glUniform1i(TextUniform,0);
}

void loadText()
{
   fontInit(20,0);
   
   width = 1024,height = 1024;
   int x = 0,y = 0,maxy = 0;

   unsigned char *lol = new unsigned char[width * height];
   memset(lol,0,width * height);

   for (int i = 32;i< 128;i++)
   {


      loadGlyph(i);

      int w = glyphs[i -32]->bitmap.width;
      int h = glyphs[i -32]->bitmap.rows;
      
      maxy = maxy > h ? maxy : h;

      if ((x + w) > width)
	 y+= maxy + 3,x = 0,maxy = 0;

      glyphsPos[i -32].x = x;
      glyphsPos[i -32].y = y;
      
      for (int l = 0;l<h;l++)
	 memcpy(lol + (l +y)*width + x,glyphs[i -32]->bitmap.buffer + l * w,w);

      x+=w + 3;
   }

   glActiveTexture(GL_TEXTURE1);
   GLuint texttext;;
   glGenTextures(1,&texttext);

   glBindTexture(GL_TEXTURE_2D,texttext);

   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
   glTexImage2D(GL_TEXTURE_2D,0,GL_ALPHA,width,height,0,GL_ALPHA,GL_UNSIGNED_BYTE,lol );

   glUniform1i(TextUniform,1);
}
