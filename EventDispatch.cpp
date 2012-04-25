/*
 * EventDispatch.cpp
 * @author Joel Nises
 */

#include "EventDispatch.h"
#include <algorithm>
#include <boost/bind.hpp>

using namespace std;
using namespace boost;

namespace megadodo
{

  void EventDispatch::addHandler(callbacktype handler)
  {
    eventhandlers.push_back(handler);
  }

  void EventDispatch::dispatch(const Event &event)
  {
    for(vector<callbacktype>::iterator it=eventhandlers.begin();it!=eventhandlers.end();it++)
      {
        if((*it)(event)) break;
      }
  }

}
