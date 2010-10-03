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

GLuint makeShader(const char *filename,GLuint type)
{
   GLuint shader = glCreateShader(type);

   char * source = makeSource(filename);

   glShaderSource(shader,1,(const GLchar **) &source,NULL);
   glCompileShader(shader);
   
   free(source);

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

