/**
 * @file KeyEventManager.h
 *
 * @author Joel Nises
 */

#ifndef KEYEVENTMANAGER_H_
#define KEYEVENTMANAGER_H_

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include "SDL.h"
#include <map>
#include "EventDispatch.h"

namespace megadodo
{
  /** Manages key events. */
  class KeyEventManager
  {
    typedef boost::function<void (const SDL_KeyboardEvent *)> functiontype;
    typedef std::map<SDLKey,functiontype> keybindmaptype;

    keybindmaptype keybindmap;
  public:
    bool operator()(const Event &event);
    
    void bindKey(SDLKey key,boost::function<void (const SDL_KeyboardEvent *)> f);

    /** Bind key to f.
        Note, you cannot have both a keyup and a keydown bind to the
        same key.
    */
    void bindKeyUp(SDLKey key,boost::function<void ()> f);

    /** Bind key to f.
        Note, you cannot have both a keyup and a keydown bind to the
        same key.
    */
    void bindKeyDown(SDLKey key,boost::function<void ()> f);

    // pointer to member function
    template<typename T> void bindKey(SDLKey key,T *object,void (T::*pmf)(const SDL_KeyboardEvent *))
    {keybindmap[key]=functiontype(boost::bind(pmf,object,_1));}
    // smartpointer to member function
    template< template<typename T> class C ,typename T> void bindKey(SDLKey key,C<T> object,void (T::*pmf)(const SDL_KeyboardEvent *))
    {keybindmap[key]=functiontype(boost::bind(pmf,object,_1));}
    
    void unbindKey(SDLKey key);
  };

  void setBool(bool *flag,bool value);
  void toggleBool(bool *flag);
}

#endif
