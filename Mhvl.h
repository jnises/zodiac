/**
** @file Mhvl.h
** MHVL (Merry Happy Vector Library) is a simple matrix/vector library.
** @version 0.1.1 
**
** @author Joel Nises
** 
** Started on  Sun Jul 23 21:29:47 2006 moushkka
*/

// MHVL (Merry Happy Vector Library) 

#ifndef   	MHVL_H_
#define   	MHVL_H_

////////////////////
// Static define settings
////////////////////
#ifndef MHVL_ALIGN
#define MHVL_ALIGN
#endif


////////////////////
// Other macros
////////////////////
#ifdef MHVL_ALIGN
#define MHVL_ALIGNED_16 __attribute__ ((aligned (16)))
#else
#define MHVL_ALIGNED_16
#endif //MHVL_ALIGN

#include <cstring>
#include <cmath>
#include <string>
#include <algorithm>
#include <iostream>
#include <numeric>

#define MHVL_DEG2RAD_CONST 0.017453292519943295
#define MHVL_RAD2DEG_CONST 57.295779513082323

namespace mhvl
{
  class MhvlException:public std::exception
  {
    std::string message;
  public:
    MhvlException(const std::string &msg):message(msg){}
    ~MhvlException() throw(){}
    virtual const char *what() const noexcept override {return message.c_str();}
  };

  class MhvlZeroDeterminantException
  {};

  class MhvlNonSquareMatrixException
  {};

  class MhvlWrongSizeMatrixException
  {};

  class MhvlDivideByZeroException
  {};

}

#include "Mhvl_vector.h"

#include "Mhvl_matrix.h"

#endif 	    /* !MHVL_H_ */
