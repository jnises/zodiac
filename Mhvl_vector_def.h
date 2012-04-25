/*
 * Mhvl_vector_def.h
 * This file contains definitions of vector functions. They have been
 * placed in a separate file to avoid clutter.
 * @author Joel Nises
 */

#ifndef MHVL_VECTOR_DEF_H_
#define MHVL_VECTOR_DEF_H_

namespace mhvl
{
  // vector vector addition
  template<typename vectype,unsigned int vecsize>
  inline Vector<vectype,vecsize> operator+(const Vector<vectype,vecsize> &vec0,const Vector<vectype,vecsize> &vec1)
  {
    Vector<vectype,vecsize> tmpvec;
    for(unsigned int loop0=0;loop0<vecsize;loop0++)
      tmpvec[loop0]=vec0[loop0]+vec1[loop0];

    return tmpvec;
  }

  // vector scalar addition, might conflict with the vector vector addition
  template<typename vectype,unsigned int vecsize>
  inline Vector<vectype,vecsize> operator+(const Vector<vectype,vecsize> &vec0,vectype val)
  {
    Vector<vectype,vecsize> tmpvec;
    for(unsigned int loop0=0;loop0<vecsize;loop0++)
      tmpvec[loop0]=vec0[loop0]+val;

    return tmpvec;
  }

  // vector vector subtraction
  template<typename vectype,unsigned int vecsize>
  inline Vector<vectype,vecsize> operator-(const Vector<vectype,vecsize> &vec0,const Vector<vectype,vecsize> &vec1)
  {
    Vector<vectype,vecsize> tmpvec;
    for(unsigned int loop0=0;loop0<vecsize;loop0++)
      tmpvec[loop0]=vec0[loop0]-vec1[loop0];

    return tmpvec;
  }

  // vector scalar subtraction, might conflict with the vector vector subtraction
  template<typename vectype,unsigned int vecsize>
  inline Vector<vectype,vecsize> operator-(const Vector<vectype,vecsize> &vec0,vectype val)
  {
    Vector<vectype,vecsize> tmpvec;
    for(unsigned int loop0=0;loop0<vecsize;loop0++)
      tmpvec[loop0]=vec0[loop0]-val;

    return tmpvec;
  }

  // vector, scalar division
  template<typename vectype,unsigned int vecsize>
  inline Vector<vectype,vecsize> operator/(const Vector<vectype,vecsize> &vec0,vectype val)
  {
    Vector<vectype,vecsize> tmpvec;
    for(unsigned int loop0=0;loop0<vecsize;loop0++)
      tmpvec[loop0]=vec0[loop0]/val;

    return tmpvec;
  }

  /**
     Inner product.
  */
  template<typename vectype,unsigned int vecsize>
  inline vectype operator*(const Vector<vectype,vecsize> &vec0,const Vector<vectype,vecsize> &vec1)
  {
    vectype tmpval=0;
    for(unsigned int loop0=0;loop0<vecsize;loop0++)
      tmpval+=vec0[loop0]*vec1[loop0];

    return tmpval;
  }

  // Vector scalar multiplication.
  template<typename vectype,unsigned int vecsize>
  inline Vector<vectype,vecsize> operator*(const Vector<vectype,vecsize> &vec0,vectype scalar)
  {
    Vector<vectype,vecsize> tmpvec;

    for(unsigned int loop0=0;loop0<vecsize;loop0++)
      tmpvec[loop0]=vec0[loop0]*scalar;

    return tmpvec;
  }

  // Scalar vector multiplication. 
  template<typename vectype,unsigned int vecsize>
  inline Vector<vectype,vecsize> operator*(vectype scalar,const Vector<vectype,vecsize> &vec0)
  {
    Vector<vectype,vecsize> tmpvec;

    for(unsigned int loop0=0;loop0<vecsize;loop0++)
      tmpvec[loop0]=vec0[loop0]*scalar;

    return tmpvec;
  }


  // Addition assignment. 
  template<typename vectype,unsigned int vecsize>
  inline Vector<vectype,vecsize> &operator+=(Vector<vectype,vecsize> &vec0,const Vector<vectype,vecsize> &vec1)
  {
    for(unsigned int loop0=0;loop0<vecsize;loop0++)
      vec0[loop0]+=vec1[loop0];
    
    return vec0;
  }


  template<typename vectype,unsigned int vecsize>
  inline Vector<vectype,vecsize> divElem(const Vector<vectype,vecsize> &vec0,const Vector<vectype,vecsize> &vec1)
  {
    Vector<vectype,vecsize> tmpvec;
    for(unsigned int loop0=0;loop0<vecsize;loop0++)
      tmpvec[loop0]=vec0[loop0]/vec1[loop0];

    return tmpvec;
  }

  template<typename vectype,unsigned int vecsize>
  inline Vector<vectype,vecsize> mulElem(const Vector<vectype,vecsize> &vec0,const Vector<vectype,vecsize> &vec1)
  {
    Vector<vectype,vecsize> tmpvec;
    for(unsigned int loop0=0;loop0<vecsize;loop0++)
      tmpvec[loop0]=vec0[loop0]*vec1[loop0];

    return tmpvec;
  }

  // return -vec
  template<typename vectype,unsigned int vecsize>
  inline Vector<vectype,vecsize> operator-(const Vector<vectype,vecsize> &vec)
  {
    Vector<vectype,vecsize> vector;

    for(unsigned int loop0=0;loop0<vecsize;loop0++)
      vector[loop0]=-vector[loop0];

    return vector;
  }

  template<typename vectype,unsigned int vecsize>
  inline Vector<vectype,vecsize> normalize(const Vector<vectype,vecsize> &vec)
  {
    Vector<vectype,vecsize> tmpvec=vec;

    vectype length=0;
    for(unsigned int loop0=0;loop0<vecsize;loop0++)
      length+=vec[loop0]*vec[loop0];

    length=sqrt(length);

    if(length==0) throw(MhvlDivideByZeroException());

    for(unsigned int loop0=0;loop0<vecsize;loop0++)
      tmpvec[loop0]/=length;

    return tmpvec;
  }

  template<typename vectype,unsigned int vecsize>
  inline void normalizeInPlace(Vector<vectype,vecsize> &vec)
  {
    vectype length=0;
    for(unsigned int loop0=0;loop0<vecsize;loop0++)
      length+=vec[loop0]*vec[loop0];

    length=sqrt(length);

    if(length==0) throw(MhvlDivideByZeroException());

    for(unsigned int loop0=0;loop0<vecsize;loop0++)
      vec[loop0]/=length;

  }

  template<typename vectype,unsigned int vecsize>
  inline Vector<vectype,vecsize> homSub(const Vector<vectype,vecsize> &vec0,const Vector<vectype,vecsize> &vec1)
  {
    Vector<vectype,vecsize> tmpvec;
    
    for(unsigned int loop0=0;loop0<vecsize-1;loop0++)
      tmpvec[loop0]=vec0[loop0]-vec1[loop0];
    
    tmpvec[vecsize-1]=vec0[vecsize-1];

    return tmpvec;
  }

  template<typename vectype,unsigned int vecsize>
  inline Vector<vectype,vecsize> homAdd(const Vector<vectype,vecsize> &vec0,const Vector<vectype,vecsize> &vec1)
  {
    Vector<vectype,vecsize> tmpvec;
    
    for(unsigned int loop0=0;loop0<vecsize-1;loop0++)
      tmpvec[loop0]=vec0[loop0]+vec1[loop0];
    
    tmpvec[vecsize-1]=vec0[vecsize-1];
    
    return tmpvec;
  }


  // Clamping. 
  template<typename vectype,unsigned int vecsize>
  inline Vector<vectype,vecsize> clamp(const Vector<vectype,vecsize> &vec0,const Vector<vectype,vecsize> &low,const Vector<vectype,vecsize> &high)
  {
    Vector<vectype,vecsize> tmpvec;

    for(unsigned int loop0=0;loop0<vecsize;loop0++)
      tmpvec[loop0]=(vec0[loop0]<low[loop0]?low[loop0]:(vec0[loop0]>high[loop0]?high[loop0]:vec0[loop0]));

    return tmpvec;
  }

  template<typename vectype,unsigned int vecsize>
  inline Vector<vectype,vecsize> clamp(const Vector<vectype,vecsize> &vec0,const vectype &low,const vectype &high)
  {
    Vector<vectype,vecsize> tmpvec;

    for(unsigned int loop0=0;loop0<vecsize;loop0++)
      tmpvec[loop0]=(vec0[loop0]<low?low:(vec0[loop0]>high?high:vec0));

    return tmpvec;
  }

  // Multiply the elements with each other. 
  template<typename vectype,unsigned int vecsize>
  inline vectype cumMult(const Vector<vectype,vecsize> &vec0)
  {
    return std::accumulate(vec0.begin(),vec0.end(),(vectype)1,std::multiplies<vectype>());
  }

  // Euclidean norm
  template<typename vectype,unsigned int vecsize>
  inline vectype norm(const Vector<vectype,vecsize> &vec)
  {
    return sqrt(vec*vec);
  }

  // Calculate the eucliean norm of the homogen vector. 
  template<typename vectype,unsigned int vecsize>
  inline vectype homNorm(const Vector<vectype,vecsize> &vec)
  {
    vectype tmpval=0.0;

    for(unsigned int loop0=0;loop0<vecsize-1;loop0++)
      tmpval+=vec[loop0]*vec[loop0];
    
    return sqrt(tmpval);
  }



  inline Vector4f operator+(const Vector4f &vec0,const Vector3f &vec1)
  {
    Vector4f tmpvec;
    
    tmpvec[0]=vec0[0]+vec1[0];
    tmpvec[1]=vec0[1]+vec1[1];
    tmpvec[2]=vec0[2]+vec1[2];
    tmpvec[3]=vec0[3];

    return tmpvec;
  }


  // overloaded output method
  template<typename vectype,unsigned int vecsize>
  std::ostream &operator<<(std::ostream &stream,const Vector<vectype,vecsize> &vector)
  {
    for(unsigned int loop0=0;loop0<vecsize;loop0++)
      {
        stream<<vector[loop0]<<",";
      }
    return stream;
  }

  Vector4f makeVector4f(float x,float y,float z,float w)
  {
    Vector4f tmpvec;
    tmpvec[0]=x;
    tmpvec[1]=y;
    tmpvec[2]=z;
    tmpvec[3]=w;

    return tmpvec;
  }

  Vector4f makeVector4f(const mhvl::Vector3f &vec,float w)
  {
    Vector4f tmpvec;
    tmpvec[0]=vec[0];
    tmpvec[1]=vec[1];
    tmpvec[2]=vec[2];
    tmpvec[3]=w;

    return tmpvec;
  }

  Vector3f makeVector3f(float x,float y,float z)
  {
    Vector3f tmpvec;
    tmpvec[0]=x;
    tmpvec[1]=y;
    tmpvec[2]=z;

    return tmpvec;
  }

  Vector2f makeVector2f(float x,float y)
  {
    Vector2f tmpvec;
    tmpvec[0]=x;
    tmpvec[1]=y;

    return tmpvec;
  }
}

#endif
