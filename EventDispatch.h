/**
 * @file EventDispatch.h
 * @author Joel Nises
 */

#pragma once

#include <vector>
#include <functional>

union SDL_Event;

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
        typedef std::function<bool (const Event &event)> callbacktype;
        void addHandler(callbacktype handler);

        template<typename T>
        void removeHandler(T handler)
        {eventhandlers.erase(remove(eventhandlers.begin(),eventhandlers.end(),handler),eventhandlers.end());}

        void dispatch(const Event &event);

    private:
        std::vector<callbacktype> eventhandlers;
    };
  
}
