/*
 * @author Joel Nises
 */

#include "KeyEventManager.h"
#include <iostream>

using namespace std;
using namespace boost;

namespace megadodo
{
  void keyDownHelper(boost::function<void ()> f,const SDL_KeyboardEvent *kevent)
  {
    if(kevent->type==SDL_KEYDOWN) f();
  }

  void keyUpHelper(boost::function<void ()> f,const SDL_KeyboardEvent *kevent)
  {
    if(kevent->type==SDL_KEYUP) f();
  }

  bool KeyEventManager::operator()(const Event &event)
  {
    if(event.event->type!=SDL_KEYUP&&event.event->type!=SDL_KEYDOWN) return false;

    keybindmaptype::iterator keyfunc=keybindmap.find(event.event->key.keysym.sym);
    if(keyfunc==keybindmap.end()) return false;

    (*keyfunc).second(&event.event->key);
    return true;
  }

  void KeyEventManager::bindKey(SDLKey key,boost::function<void (const SDL_KeyboardEvent *)> f)
  {
    keybindmap[key]=f;
  }

  void KeyEventManager::bindKeyUp(SDLKey key,boost::function<void ()> f)
  {
    keybindmap[key]=bind(keyUpHelper,f,_1);
  }

  void KeyEventManager::bindKeyDown(SDLKey key,boost::function<void ()> f)
  {
    keybindmap[key]=bind(keyDownHelper,f,_1);
  }


  void KeyEventManager::unbindKey(SDLKey key)
  {
    keybindmap.erase(key);
  }

  void setBool(bool *flag,bool value){*flag=value;}
  void toggleBool(bool *flag){*flag=!*flag;}

}
