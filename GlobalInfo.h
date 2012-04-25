/**
 * @file GlobalInfo.h
 * @author Joel Nises
 */

#ifndef GLOBALINFO_H_
#define GLOBALINFO_H_

#include "EventDispatch.h"
#include "KeyEventManager.h"

namespace megadodo
{
  class GenericException:public std::exception
  {
    std::string message;
  public:
    GenericException(std::string msg) throw():message(msg){}
    ~GenericException() throw(){}
    const char *what(){return message.c_str();}
  };

  /** Some variables needed by multiple parts of the program. */
  struct GlobalInfo
  {
    int winwidth,winheight; ///< window width and height.
    float aspect; ///< window aspect ratio.
    
    int bytesperpixel;
    
    bool doublebuffer;
    bool fullscreen;

    bool running; ///< set to false to stop execution.

    int multisamples; ///< number of antialiasing samples.

    EventDispatch *eventDispatch; ///< the main EventDispatch object.
    KeyEventManager *keyBinds; ///< the main KeyEventManager object.

  };

}
#endif
