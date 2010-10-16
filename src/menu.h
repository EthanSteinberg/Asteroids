#ifndef MENU_H_INCLUDED
#define MENU_H_INCLDUED

class t_menu
{
public:
   t_menu();

   void drawall() const;
   void moveall();

   bool valid() const;
   
   void open();

private:
   int state;
   int selected;
};

#endif
