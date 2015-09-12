/**
 * @file Mhvl_vector.h
 * This file contains vector function declarations.
 *
 * @author Joel Nises
 */

#ifndef MHVL_VECTOR_H_
#define MHVL_VECTOR_H_


namespace mhvl
{
    /** The main vector class in mhvl. */
    template<typename vectype,unsigned int vecsize>
    class Vector;

    /*template<typename vectype>
      class VectorSlice;

      template<typename vectype>
      class VectorSliceIterator;*/

    /** Vector typedefs. */
    typedef MHVL_ALIGNED_16 Vector<float,4> Vector4f;
    typedef Vector<float,3> Vector3f;
    typedef Vector<float,2> Vector2f;

    /** Vanilla vector addition. */
    template<typename vectype,unsigned int vecsize>
    inline Vector<vectype,vecsize> operator+(const Vector<vectype,vecsize> &vec0,const Vector<vectype,vecsize> &vec1);

    /** Vector, scalar addition. */
    template<typename vectype,unsigned int vecsize>
    inline Vector<vectype,vecsize> operator+(const Vector<vectype,vecsize> &vec0,vectype val);

    /** Vanilla vector subtraction. */
    template<typename vectype,unsigned int vecsize>
    inline Vector<vectype,vecsize> operator-(const Vector<vectype,vecsize> &vec0,const Vector<vectype,vecsize> &vec1);

    /** Vector, scalar subtraction. */
    template<typename vectype,unsigned int vecsize>
    inline Vector<vectype,vecsize> operator-(const Vector<vectype,vecsize> &vec0,vectype val);

    /** Inner product. */
    template<typename vectype,unsigned int vecsize>
    inline vectype operator*(const Vector<vectype,vecsize> &vec0,const Vector<vectype,vecsize> &vec1);

    /** Element-wise division. */
    template<typename vectype,unsigned int vecsize>
    inline Vector<vectype,vecsize> divElem(const Vector<vectype,vecsize> &vec0,const Vector<vectype,vecsize> &vec1);

    /** Element-wise multiplication. */
    template<typename vectype,unsigned int vecsize>
    inline Vector<vectype,vecsize> multElem(const Vector<vectype,vecsize> &vec0,const Vector<vectype,vecsize> &vec1);

    /** Vector, scalar division. */
    template<typename vectype,unsigned int vecsize>
    inline Vector<vectype,vecsize> operator/(const Vector<vectype,vecsize> &vec0,vectype val);

    /** @return -vec. */
    template<typename vectype,unsigned int vecsize>
    inline Vector<vectype,vecsize> operator-(const Vector<vectype,vecsize> &vec);

    /** Vector scalar multiplication. */
    template<typename vectype,unsigned int vecsize>
    inline Vector<vectype,vecsize> operator*(const Vector<vectype,vecsize> &vec0,vectype scalar);

    /** Scalar vector multiplication. */
    template<typename vectype,unsigned int vecsize>
    inline Vector<vectype,vecsize> operator*(vectype scalar,const Vector<vectype,vecsize> &vec0);

    /** Addition assignment. */
    template<typename vectype,unsigned int vecsize>
    inline Vector<vectype,vecsize> &operator+=(Vector<vectype,vecsize> &vec0,const Vector<vectype,vecsize> &vec1);

    /** Return a normalized vector. */
    template<typename vectype,unsigned int vecsize>
    inline Vector<vectype,vecsize> normalize(const Vector<vectype,vecsize> &vec);
    
    /** Normalize vector in place. */
    template<typename vectype,unsigned int vecsize>
    inline void normalizeInPlace(Vector<vectype,vecsize> &vec);

    /** Subtraction of homogen vectors.
        The last element is cosidered to be a homogen coordinate, and is
        not subtracted. The last element of the return value is copied
        from vec0.  
    **/
    template<typename vectype,unsigned int vecsize>
    inline Vector<vectype,vecsize> homSub(const Vector<vectype,vecsize> &vec0,const Vector<vectype,vecsize> &vec1);

    /** Addition of homogen vectors. **/
    template<typename vectype,unsigned int vecsize>
    inline Vector<vectype,vecsize> homAdd(const Vector<vectype,vecsize> &vec0,const Vector<vectype,vecsize> &vec1);

    /** Clamping. **/
    template<typename vectype,unsigned int vecsize>
    inline Vector<vectype,vecsize> clamp(const Vector<vectype,vecsize> &vec0,const Vector<vectype,vecsize> &low,const Vector<vectype,vecsize> &high);

    template<typename vectype,unsigned int vecsize>
    inline Vector<vectype,vecsize> clamp(const Vector<vectype,vecsize> &vec0,const vectype &low,const vectype &high);

    /** Multiply the elements with each other. **/
    template<typename vectype,unsigned int vecsize>
    inline vectype cumMult(const Vector<vectype,vecsize> &vec0);

    /** Calculate the eucliean norm of the vector. */
    template<typename vectype,unsigned int vecsize>
    inline vectype norm(const Vector<vectype,vecsize> &vec);

    /** Calculate the eucliean norm of the homogen vector, ignoring the
        last element. */
    template<typename vectype,unsigned int vecsize>
    inline vectype homNorm(const Vector<vectype,vecsize> &vec);

    /** Special operator to add a Vector3f to a Vector4f. */
    inline Vector4f operator+(const Vector4f &vec0,const Vector3f &vec1);

    /** Overloaded output method. */
    template<typename vectype,unsigned int vecsize>
    std::ostream &operator<<(std::ostream &stream,const Vector<vectype,vecsize> &vector);

  
    ///////////////////////////////////
    // Non-templated functions
    ///////////////////////////////////  

    /** Return a Vector4f with the given components. */
    inline Vector4f makeVector4f(float x,float y,float z,float w=1);
    inline Vector4f makeVector4f(const Vector3f &vec,float w=1);
    inline Vector3f makeVector3f(float x,float y,float z);
    inline Vector2f makeVector2f(float x,float y);

    /** Fast approximation of 1/sqrt(x), depends on the floating point
        representation used on x86 computers. */
    //float InvSqrt (float x);

    /*void normalizeInPlaceApprox(Vector3f &vec);
      Vector3f normalizeApprox(const Vector3f &vec);*/

    /*
      vector class
    */
    template<typename vectype,unsigned int vecsize> 
    class Vector 
    {
    public:
        static const unsigned int datalength=vecsize;
        static const unsigned int datasize=vecsize*sizeof(vectype);
        typedef Vector<vectype,vecsize> thistype;
        typedef vectype value_type;
        typedef vectype *iterator;
        typedef const vectype *const_iterator;
        typedef vectype &reference;
        typedef const vectype &const_reference;
        typedef vectype *pointer;
        typedef long difference_type;
        typedef unsigned int size_type;
    
    
        /** Default constructor.
            Note this constructor doesn't set the vector to 0
        **/
        Vector(){/*memset(data,0,datasize);*/}

        explicit Vector(value_type initval)
        {
            std::fill_n(begin(),datalength,initval);
        }

        template<unsigned int srcsize>
        explicit Vector(const Vector<vectype,srcsize> &vec)
        {
            if(srcsize<vecsize)
            {
                std::copy(vec.begin(),vec.end(),begin());
                std::fill_n(begin()+srcsize,vecsize-srcsize,0);
            }
            else
                std::copy(vec.begin(),vec.end(),begin());
        }

        template<unsigned int srcsize>
        thistype &operator=(const Vector<vectype,srcsize> &vec)
        {
            if(srcsize<vecsize)
            {
                std::copy(vec.begin(),vec.end(),begin());
                std::fill_n(begin()+srcsize,vecsize-srcsize,0);
            }
            else
                std::copy(vec.begin(),vec.end(),begin());      

            return *this;
        }

        value_type operator()(unsigned int num) const {return dataarr[num];}
        value_type &operator[](unsigned int num) {return dataarr[num];}
        const value_type &operator[](unsigned int num) const {return dataarr[num];}
    
        /*operator value_type * (){return data;}   
          operator const value_type * () const {return data;}   */

        iterator begin(){return dataarr;}
        iterator end(){return dataarr+datalength;}

        const_iterator begin() const {return dataarr;}
        const_iterator end() const {return dataarr+datalength;}

        size_type size(){return datalength;}
        size_type maxsize(){return datalength;}
        bool empty(){return datalength==0;}

        const value_type *data() const {return dataarr;}
        value_type *c_array() {return dataarr;}

        void assign(const value_type &value)
        {
            std::fill_n(begin(),size(),value);
        }

    private:
        value_type dataarr[vecsize];
    };


    /* Vector slices combined with overloaded versions of all the
       operators defined for the Vector class would remove the need for
       special functions operating on homogen vectors and such. */
  
    /*
      template<typename vectype>
      class VectorSliceIterator
      {
      public:
      virtual const vectype &operator*() const =0;
      virtual vectype &operator*()=0;
    
      };


      template<typename vectype>
      class VectorSlice
      {
      public:
      typedef VectorSlice<vectype> thistype;
      typedef vectype value_type;
      typedef VectorSliceIterator iterator;
      typedef const VectorSliceIterator const_iterator;
      typedef vectype &reference;
      typedef const vectype &const_reference;
      typedef vectype *pointer;
      typedef long difference_type;
      typedef unsigned int size_type;
    
      virtual ~VectorSlice(){}

      virtual value_type operator()(unsigned int num) const =0;
      virtual value_type &operator[](unsigned int num) =0;
      virtual const value_type &operator[](unsigned int num) const =0;

      virtual iterator begin() =0;
      virtual iterator end() =0;
      virtual size_type size() =0;
      virtual size_type maxsize() =0;
      virtual bool empty() =0;

      virtual void clear(value_type value=0) =0;
      };
    */

}

#include "Mhvl_vector_def.h"

#endif
