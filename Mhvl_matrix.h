/**
 * @file Mhvl_matrix.h
 * This file contains matrix function declarations.
 *
 * @author Joel Nises
 */

#ifndef MHVL_MATRIX_H_
#define MHVL_MATRIX_H_

namespace mhvl
{

    /**
       Main matrix class in mhvl.
    */
    template<typename T>
    class Matrix;

    /**
       Quick assignment iterator.
    */
    template<typename T>
    class QuickAssignmentIterator;

    //////////////////////
    // Prototypes
    //////////////////////

    // this function should use two separate typenames to enable multiplication between different types.
    template<typename T>
    inline Matrix<T> 
    operator*(const Matrix<T> &op0,const Matrix<T> &op1) throw(MhvlException);

    template<typename T>
    inline Matrix<T> 
    &operator*=(Matrix<T> &op0,const Matrix<T> &op1) throw(MhvlException);

    template<typename T>
    Matrix<T> minorMatrix(const Matrix<T> &matrix,int x,int y);

    template<typename T>
    T determinant(const Matrix<T> &matrix);

    template<typename T>
    T cofactor(const Matrix<T> &matrix,int x,int y);

    template<typename T>
    Matrix<T> invert(const Matrix<T> &matrix) throw(MhvlNonSquareMatrixException);

    template<typename T>
    Matrix<T> makeIdentityMatrix(unsigned int size);

    /** Create a 4 by 4 rotation matrix. */
    template<typename T>
    Matrix<T> makeRotationMatrix4(T angle,T x,T y,T z);

    template<typename T>
    std::ostream &operator<<(std::ostream &stream,const Matrix<T> &matrix);

    template<typename T>
    QuickAssignmentIterator<T> &operator<<(QuickAssignmentIterator<T> &it,const typename Matrix<T>::value_type &value);



    template<typename T>
    class Matrix
    {
    public:
        typedef T value_type;
        typedef Matrix<value_type> thistype;
        typedef value_type *iterator;
        typedef const value_type *const_iterator;
        typedef value_type &reference;
        typedef const value_type &const_reference;
        typedef value_type *pointer;
        typedef long difference_type;
        typedef unsigned int size_type;

    private:
        value_type *dataarr;
        unsigned int w,h,numelements;
        int *refcount;
        bool tainted;
        bool allocated;

        /** Note! this method doesn't take care to separate matrices. */
        void setElement(const value_type &value,unsigned int x,unsigned int y)
        {dataarr[x*h+y]=value;}
    
        /** Note! this method doesn't take care of separating matrices. */
        void setElement(const value_type &value,unsigned int num)
        {dataarr[num]=value;}

        /** Note! this method doesn't take care to separate matrices. */
        value_type &getElementRef(unsigned int x,unsigned int y)
        {return dataarr[x*h+y];}

        /** Note! this method doesn't take care of separating matrices. */
        value_type &getElementRef(unsigned int num)
        {return dataarr[num];}

    public:

        // constructors
        Matrix()//:dataarr(NULL),w(0),h(0),numelements(0),tainted(false),allocated(true)
        {
            //refcount=new int(1);
        }   

        Matrix(unsigned int width,unsigned int height):w(width),h(height),tainted(false),allocated(true)
        {
            numelements=w*h;
            dataarr=new value_type[numelements];
            refcount=new int(1);
        }

        Matrix(unsigned int width,unsigned int height,value_type initval):w(width),h(height),tainted(false),allocated(true)
        {
            numelements=w*h;
            dataarr=new value_type[numelements];

            for(unsigned int loop0=0;loop0<size();loop0++)
            {
                dataarr[loop0]=initval;
            }
      
            refcount=new int(1);
        }

        Matrix(const thistype &matrix)
        {
            memcpy(this,&matrix,sizeof(thistype));
            (*refcount)++;

            if(tainted) separate();
        }

        /*Matrix(unsigned int width,unsigned int height,const value_type *matrix):w(width),h(height),numelements(w*h),tainted(false),allocated(true)
          {
          dataarr=new value_type[numelements];
          memcpy(dataarr,matrix,numelements*sizeof(value_type));
          refcount=new int(1);
          }*/

        ~Matrix()
        {
            if((--(*refcount))<1)
            {
                if(allocated) delete [] dataarr;
                delete refcount;
            }
        }



        const thistype &operator=(const thistype &matrix)
        {
            if(this==&matrix) return *this;

            if((--(*refcount))<1)
            {
                if(allocated) delete [] dataarr;
                delete refcount;
            }

            memcpy(this,&matrix,sizeof(thistype));
            (*refcount)++;

            if(tainted) separate();

            return *this;
        }

        value_type &operator()(unsigned int x,unsigned int y)
        {
            separate();
            tainted=true;
            return dataarr[x*h+y];
        }

        value_type operator()(unsigned int x,unsigned int y) const
        {
            return dataarr[x*h+y];
        }

        value_type &operator[](unsigned int num){separate(); tainted=true; return dataarr[num];}
        value_type operator[](unsigned int num) const {return dataarr[num];}

        iterator begin(){separate(); tainted=true; return dataarr;}
        iterator end(){separate(); tainted=true; return dataarr+numelements;}
        const_iterator begin() const {separate(); return dataarr;}
        const_iterator end() const {separate(); return dataarr+numelements;}

        size_type size(){return numelements;}
        size_type maxsize(){return numelements;}
        bool empty(){return numelements==0;}

        /*void swap(Matrix<value_type> &matrix)
          {
          // It shouldn't be nessesary to copy the data.
          thistype tmpmat(matrix);
          std::memcpy(matrix.dataarr,dataarr,numelements*sizeof(value_type));
          std::memcpy(dataarr,tmpmat.dataarr,numelements*sizeof(value_type));
          }*/

        void assign(const value_type &value)
        {
            separate();
            std::fill_n(begin(),size(),value);
        }

        unsigned int getWidth() const {return w;}
        unsigned int getHeight() const {return h;}
        unsigned int size() const {return numelements;}

        void setDimensions(unsigned int width,unsigned int height)
        {
            if(width==w&&height==h) return;

            int numnewelements=width*height;
            value_type *tmpdata=new value_type[numnewelements];

            fill(tmpdata,tmpdata+numnewelements,0);

            unsigned int fillx=std::min(width,w),filly=std::min(height,h);

            for(unsigned int loopx=0;loopx<fillx;loopx++)
                memcpy(tmpdata+loopx*height,dataarr+loopx*h,filly*sizeof(value_type));

            if((--(*refcount))<1)
            {
                delete [] dataarr;
                delete refcount;
            }

            dataarr=tmpdata;
            refcount=new int(1);
            tainted=false;
            w=width;
            h=height;
            numelements=numnewelements;
        }

        const value_type *data() const 
        {return dataarr;}
        /*operator const value_type *() const
          {return data;}*/

        value_type *c_array()
        {
            separate();
            tainted=true;
            return dataarr;
        }

        /** Make sure this matrix is selfcontained. */
        void separate()
        {
            if(*refcount>1)
            {
                (*refcount)--;
                refcount=new int(1);

                value_type *tmpdata=new value_type[numelements];
                memcpy(tmpdata,dataarr,numelements*sizeof(value_type));
                allocated=true;
                tainted=false;
                dataarr=tmpdata;
            }
        }

        void setElement(unsigned int x,unsigned int y,const value_type &value)
        {
            separate();
            dataarr[x*h+y]=value;
        }

        // static methods
        static Matrix<T> identityMatrix(int size)
        {
            Matrix<T> tmpmatrix(size,size,0);
      
            typename Matrix<T>::iterator it=tmpmatrix.begin();
      
            for(int loop0=0;loop0<size;loop0++)
            {
                *it=1;
                it+=size+1;
            }
      
            return tmpmatrix;
        }

        /** Returns a Matrix that is using the supplied
            matrix as internal storage for the data. */
        static thistype constructFromPointer(unsigned int width,unsigned int height,value_type *matrix)
        {
            thistype tmpmatrix;
            tmpmatrix.w=width;
            tmpmatrix.h=height;
            tmpmatrix.numelements=width*height;
            tmpmatrix.dataarr=matrix;
            tmpmatrix.tainted=true;
            tmpmatrix.allocated=false;
            tmpmatrix.refcount=new int(1);

            return tmpmatrix;
        }

        /** Create a matrix with the contents of matrix as values.
            @param matrix is assumed to be in column major.
        */
        static thistype copyFromPointer(unsigned int width,unsigned int height,const value_type *matrix)
        {
            thistype tmpmatrix;
            tmpmatrix.w=width;
            tmpmatrix.h=height;
            tmpmatrix.numelements=width*height;
            tmpmatrix.dataarr=new value_type(tmpmatrix.numelements);
            memcpy(tmpmatrix.dataarr,matrix,tmpmatrix.numelements);
            tmpmatrix.tainted=false;
            tmpmatrix.allocated=true;
            tmpmatrix.refcount=new int(1);

            return tmpmatrix;
        }

        // Friendslist
        friend Matrix<value_type> operator*<value_type>(const Matrix<value_type> &op0,const Matrix<value_type> &op1);
        friend Matrix<value_type> &operator*=<value_type>(Matrix<value_type> &op0,const Matrix<value_type> &op1);
    };

    typedef Matrix<float> Matrixf;


    // Quick assignment iterator.
    template<typename T>
    class QuickAssignmentIterator
    {
        typename Matrix<T>::iterator currpos;
    public:
        QuickAssignmentIterator(typename Matrix<T>::iterator start):currpos(start){}

        friend QuickAssignmentIterator<T> &operator<<<T>(QuickAssignmentIterator<T> &it,const typename Matrix<T>::value_type &value);
    };
 

}

#include "Mhvl_matrix_def.h"

#endif




