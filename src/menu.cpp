#include <GL/glew.h>

#include <SFML/Window.hpp>

#include <cstdio>
#include <cstdlib>

#include "menu.h"
#include "util.h"

extern const sf::Input *Input;
extern int ScaleUniform;
extern int TextScaleUniform;
extern int vbo[4];
extern const sf::Input *Input;

//Class: t_menu
//Purpose: Be a menu, with heigh scores, start game and quit
//Interface: drawall, moveall, on
t_menu::t_menu()
{
   state = 1;
   selected = 3;
}

void t_menu::drawall() const
{
   static int cleared = 0;

   if (!cleared)
      glClearColor(0,0,1,1),cleared = 1;


   switch(state)
   {
      case 1: 
	 if (selected)
	 {
	    glUniform2f(TextScaleUniform,2,2);
	    glUniform2f(ScaleUniform,1,0.175);

	    float PosArray[1][2];
	    float TexArray[1][2];
	    float ScaArray[1][2];

            PosArray[0][0] = -.9;
	    PosArray[0][1] = .70 + -.25 * selected;

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
	 }
	 
	 drawText("Start game",-.8,.5);
	 drawText("Quit game",-.8,.25);
	 drawText("Top 10",-.8,0);
	 
	 break;
   }
}

void t_menu::moveall()
{
   switch(state)
   {
      case 1:
	 float x = Input->GetMouseX()/400.0 -1;
	 float y = 1 -Input->GetMouseY()/300.0;
	 if (Input->IsMouseButtonDown(sf::Mouse::Left))
	 {
	    if(x > -.9 && x < .1 && y > .45 && y< .625)
	       state = 0;
	 
	    else if(x > -.9 && x < .1 && y > .20 && y< .375)
	       exit(0);

	    else if(x > -.9 && x < .1 && y > -.05 && y< .125)
	       state = 2;
	 }

	 else
	 {
	    if(x > -.9 && x < .1 && y > .45 && y< .625)
	       selected = 1;
	 
	    else if(x > -.9 && x < .1 && y > .20 && y< .375)
	       selected = 2;

	    else if(x > -.9 && x < .1 && y > -.05 && y< .125)
	       selected = 3;

	    else
	       selected = 0;
	 }

	break;	 
   }
}

bool t_menu::valid() const
{
   return state;
}

void t_menu::open()
{
   state = 1;
}
