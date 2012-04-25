/**
 * @file Utils.h
 * @author Joel Nises
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <sstream>
//#include <sys/time.h>

namespace megadodo
{
  extern int (*getFPS)();

  template<class T> 
  std::string toString(const T &i)
  {
    std::stringstream s;
    s<<i;
    return s.str();
  }
  template<class T> 
  std::string toString(const T &i,int precision)
  {
    std::stringstream s;
    s<<std::fixed;
    s.precision(precision);
    s<<i;
    return s.str();
  }

  template<typename T>
  T clampValue(T val,T low,T high){return (val<low?low:(val>high?high:val));}
}
#endif
