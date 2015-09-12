/**
 * @file EventDispatch.h
 * @author Joel Nises
 */

#ifndef EVENTBASE_H_
#define EVENTBASE_H_

#include "SDL.h"
#include <vector>
#include <boost/function.hpp>

namespace megadodo
{
    class Event
    {
    public:
        SDL_Event *event;
    };

    /** Handles sdl-events. */
    class EventDispatch
    {
    public:
        typedef boost::function<bool (const Event &event)> callbacktype;
        void addHandler(callbacktype handler);

        template<typename T>
        void removeHandler(T handler)
        {eventhandlers.erase(remove(eventhandlers.begin(),eventhandlers.end(),handler),eventhandlers.end());}

        void dispatch(const Event &event);

    private:
        std::vector<callbacktype> eventhandlers;
    };
  
}

#endif
