/**
 * @author Joel Nises
 */

#include "glee/GLee.h"

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include "GL/gl.h"
#include "GL/glu.h"
#endif

#include <iostream>
#include <string>
#include "SDL.h"
#include "SDL_opengl.h"
#include "Mhvl.h"
#include "GlobalInfo.h"
#include "EventDispatch.h"
#include "Utils.h"
#include "KeyEventManager.h"
#include <boost/bind.hpp>
#include "SceneDrawer.h"
#ifdef USE_PROGRAM_OPTIONS
#include <boost/program_options.hpp>
namespace po=boost::program_options;
#endif

using namespace std;
using namespace megadodo;
using namespace mhvl;
using namespace boost;

SDL_Window *window = nullptr;

void initSDL(GlobalInfo &info);
void initGL(GlobalInfo &info);
void initEvent(GlobalInfo &info);
void mainloop(GlobalInfo &info);
void cleanup();
void cleanupmess(const string &message);

int main(int argc,char **argv)
{
    GlobalInfo info;
  
    // Defaults
    info.bytesperpixel=32;
    info.doublebuffer=true;
    info.fullscreen=false;
    info.running=true;
    info.multisamples=0;

#ifdef USE_PROGRAM_OPTIONS
    // program options
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("width,w", po::value<int>(&info.winwidth)->default_value(800), "screen width")
        ("height,h", po::value<int>(&info.winheight)->default_value(600), "screen height")
        ("aspect,a", po::value<float>(), "aspect ratio")
        ("fullscreen,f", "fullscreen switch")
        ("samples,s", po::value<int>(), "number of antialiasing samples")
        ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);   
  
    if(vm.count("help"))
    {
        cout<<desc<<endl;
        exit(0);
    }
  
    if(vm.count("fullscreen"))
        info.fullscreen=true;
  
    if(vm.count("aspect"))
        info.aspect=vm["aspect"].as<float>();
    else
        info.aspect=(float)info.winwidth/info.winheight;

    if(vm.count("samples"))
        info.multisamples=vm["samples"].as<int>();
  
#else
    info.winwidth=800;
    info.winheight=600;
    info.aspect=4/3.0;
#endif
  

    atexit(cleanup);

    initEvent(info);
    initSDL(info);
    initGL(info);

    mainloop(info);

    delete info.keyBinds;
    delete info.eventDispatch;

    return 0;
}

void initSDL(GlobalInfo &info)
{
    if(SDL_Init(SDL_INIT_VIDEO))
    {
        cerr<<"Unable to init SDL at "<<__FILE__<<" line: "<<__LINE__<<endl;
    }


    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,info.doublebuffer);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,info.bytesperpixel);  

    if(info.multisamples>1)
    {
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,1);  
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,info.multisamples);
    }

    window = SDL_CreateWindow("zodiac", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, info.winwidth,info.winheight, SDL_WINDOW_OPENGL|(info.fullscreen?SDL_WINDOW_FULLSCREEN:0));
    if(!window)
    {
        cleanupmess("Unable to set up video surface at "+toString(__FILE__)+" line: "+toString(__LINE__));
    }
    auto context = SDL_GL_CreateContext(window);
}

void initGL(GlobalInfo &info)
{
    if(!GLEE_VERSION_2_0)
        cleanupmess("This program requires openGL version 2.0");

    /*if(!GLEE_EXT_framebuffer_object)
      cleanupmess("This program requires EXT_framebuffer_object");*/

    /*if(!GLEE_ARB_texture_rectangle)
      cleanupmess("This program requires ARB_texture_rectangle");*/

    int maxfloats;
    glGetIntegerv(GL_MAX_VARYING_FLOATS,&maxfloats);

    cout<<"MAX_VARYING_FLOATS: "<<maxfloats<<endl;
}

bool quitEventHandler(bool *qflag,const Event &event)
{
    if(event.event->type==SDL_QUIT)
    {
        *qflag=false;
        return true;
    }
    return false;
}

void initEvent(GlobalInfo &info)
{
    info.eventDispatch=new EventDispatch();
    info.eventDispatch->addHandler(bind(quitEventHandler,&info.running,_1));
    info.keyBinds=new KeyEventManager();
    info.eventDispatch->addHandler(*info.keyBinds);
    info.keyBinds->bindKeyUp(SDLK_ESCAPE,bind(setBool,&info.running,false));
    info.keyBinds->bindKeyUp(SDLK_q,bind(setBool,&info.running,false));
}

void mainloop(GlobalInfo &info)
{
    SDL_Event event;
    Event tmpevent;
    Uint32 lasttime;//,currtime;
    Uint32 before;

    SceneInfo sceneinfo;

    SceneDrawer drawer(&info,&sceneinfo);

    lasttime=SDL_GetTicks();

    while(info.running)
    {
        /*currtime=SDL_GetTicks();
          Uint32 elptime=currtime-lasttime;

          long waittime=17-elptime;
          if(waittime>0)SDL_Delay((Uint32)waittime);*/
      
        before=SDL_GetTicks();

        while(SDL_PollEvent(&event))
        {
            tmpevent.event=&event;
            info.eventDispatch->dispatch(tmpevent);
        }

        // draw stuff
        drawer.update(((double)before-lasttime)/1000.0);
        lasttime=before;
        drawer.draw();
      
        SDL_GL_SwapWindow(window);

        //lasttime=currtime;

        GLenum error;
        while((error=glGetError()))
            cerr<<"GL error: "<<gluErrorString(error)<<endl;

        long waittime=17-static_cast<long>(SDL_GetTicks()-before);
        if(waittime>0) SDL_Delay(waittime);
    }
}

void cleanup()
{
    SDL_Quit();
}

void cleanupmess(const string &message)
{
    cerr<<message<<endl;
    exit(1);
}

