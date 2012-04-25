/*
 * Utils.cpp
 * @author Joel Nises
 */

#include "Utils.h"
#include "SDL.h"

namespace megadodo
{
  using namespace std;

  //////////
  // getFPS

  Uint32 lastFpsTime;
  int getFPS_final()
  {
    static int numframes;
    static int fps=0;
    Uint32 currtime;

    currtime=SDL_GetTicks();

    if(currtime-lastFpsTime>=1000)
      {
        fps=numframes;
        numframes=0;
        lastFpsTime=currtime;
      }
    else
      numframes++;

    return fps;
  }

  int getFPS_init()
  {
    lastFpsTime=SDL_GetTicks();
    getFPS=getFPS_final;
    return getFPS();
  }

  int (*getFPS)()=getFPS_init;

}
