#ifndef LAUNCH_PROGRAM
#define LAUNCH_PROGRAM "zodiac"
#endif

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Return_Button.H>
#include <FL/fl_ask.H>

#include <string>
#include <sstream>
#include <unistd.h>

using namespace std;

struct Widgets
{
  Fl_Window *window;
  Fl_Box *box;
  Fl_Input *width,*height,*aspect;
  Fl_Button *calculateAspect,*fullscreen,*launch;

  Widgets();
  ~Widgets();
};

void button_pressed(Fl_Widget *w,void *data)
{
  Widgets *widgets=reinterpret_cast<Widgets *>(data);

  if(w==widgets->calculateAspect)
    {
      int width,height;
      
      stringstream widthStream(string(widgets->width->value()));
      if(!(widthStream>>width))
        {
          fl_message("Width must specify an integer.");
          Fl::focus(widgets->height);
          return;
        }
      
      stringstream heightStream(string(widgets->height->value()));
      if(!(heightStream>>height))
        {
          fl_message("Height must specify an integer.");
          Fl::focus(widgets->height);
          return;
        }

      if(width<1||height<1)
        {
          fl_message("Height and width must both be positive");
          return;
        }

      stringstream aspectStream;
      aspectStream<<((double)width/height);

      widgets->aspect->value(aspectStream.str().c_str());
    }
  else if(w==widgets->launch)
    {
      int width, height;
      double aspect;
      bool fullscreen;

      stringstream widthStream(string(widgets->width->value()));
      if(!(widthStream>>width))
        {
          fl_message("Width must specify an integer.");
          Fl::focus(widgets->height);
          return;
        }
      
      stringstream heightStream(string(widgets->height->value()));
      if(!(heightStream>>height))
        {
          fl_message("Height must specify an integer.");
          Fl::focus(widgets->height);
          return;
        }      

      
      stringstream aspectStream(string(widgets->aspect->value()));
      if(!(aspectStream>>aspect))
        {
          fl_message("Aspect is too silly to use.");
          Fl::focus(widgets->aspect);
          return;
        } 

      fullscreen=widgets->fullscreen->value();

      string widthString=string("-w")+widgets->width->value(),
        heightString=string("-h")+widgets->height->value(),
        aspectString=string("-a")+widgets->aspect->value();

      widgets->window->hide();

#ifndef WIN32
      if(fork()==0)
        {
#endif
          execl(LAUNCH_PROGRAM,LAUNCH_PROGRAM,widthString.c_str(),heightString.c_str(),aspectString.c_str(),(fullscreen?"-f":NULL),NULL);
#ifndef WIN32
          exit(1);
        }
#endif

      //system((string(LAUNCH_PROGRAM)+" "+widthString+" "+heightString+" "+aspectString+(fullscreen?" -f":"")).c_str());
    }
}

int main(int argc, char **argv) 
{
  Widgets widgets;

  widgets.window->end();
  widgets.window->show(argc, argv);
  Fl::run();

  return 0;
}

Widgets::Widgets()
{
  window = new Fl_Window(256,256,"Zodiac Launcher");
  box = new Fl_Box(0,0,256,70,"Zodiac Launcher");
  box->labelsize(25);
  box->labelfont(FL_BOLD);

  width=new Fl_Input(60,80,100,25,"Width:");
  width->value("800");
  
  height=new Fl_Input(60,110,100,25,"Height:");
  height->value("600");
  
  aspect=new Fl_Input(60,140,100,25,"Aspect:");
  aspect->value("1.33333");
  
  calculateAspect=new Fl_Button(170,140,80,25,"Calculate");
  calculateAspect->callback(button_pressed,this);
  
  fullscreen=new Fl_Check_Button(10,175,80,25," Fullscreen");
  
  launch=new Fl_Return_Button(88,215,80,25,"Launch");
  launch->callback(button_pressed,this);
  
  Fl::focus(launch);
}

Widgets::~Widgets()
{
  delete launch;
  delete fullscreen;
  delete calculateAspect;
  delete aspect;
  delete height;
  delete width;    
  delete box;
  delete window;
}

