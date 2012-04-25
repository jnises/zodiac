/*
 * Mhvl_matrix_def.h
 * This file contains definitions of matrix functions. They have been
 * placed in a separate file to avoid clutter.
 * @author Joel Nises
 */

#ifndef MHVL_MATRIX_DEF_H_
#define MHVL_MATRIX_DEF_H_

namespace mhvl
{

  /////////////////////
  // Matrix functions
  /////////////////////

  template<typename T>
  Matrix<T> operator*(const Matrix<T> &op0,const Matrix<T> &op1) throw(MhvlException)
  {
    if(op0.getWidth()!=op1.getHeight()) throw(MhvlException("Matrix sizes doesn't match in operator*"));

    Matrix<T> tmpmatrix(op1.getWidth(),op0.getHeight(),0);

    typename Matrix<T>::iterator dest=tmpmatrix.begin();

    for(unsigned int loopx=0;loopx<op1.getWidth();loopx++)
      {
        for(unsigned int loopy=0;loopy<op0.getHeight();loopy++)
          {
            for(unsigned int loop0=0;loop0<op0.getWidth();loop0++)
              {
                *dest+=op0[loopy+loop0*op0.getHeight()]*op1[loop0+loopx*op0.getWidth()];
              }
            dest++;
          }
      }

    return tmpmatrix;
  }

  template<typename T>
  inline Matrix<T> 
  &operator*=(Matrix<T> &op0,const Matrix<T> &op1) throw(MhvlException)
  {
    if(op0.getWidth()!=op1.getHeight()) throw(MhvlException("Matrix sizes doesn't match in operator*"));

    Matrix<T> tmpmatrix(op1.getWidth(),op0.getHeight(),0);

    typename Matrix<T>::iterator dest=tmpmatrix.begin();

    for(unsigned int loopx=0;loopx<op1.getWidth();loopx++)
      {
        for(unsigned int loopy=0;loopy<op0.getHeight();loopy++)
          {
            for(unsigned int loop0=0;loop0<op0.getWidth();loop0++)
              {
                *dest+=op0[loopy+loop0*op0.getHeight()]*op1[loop0+loopx*op0.getWidth()];
              }
            dest++;
          }
      }

    op0=tmpmatrix;
    return op0;  
  }

  template<typename T>
  Matrix<T> minorMatrix(const Matrix<T> &matrix,int x,int y)
  {
    Matrix<T> tmpmatrix(matrix.getWidth()-1,matrix.getHeight()-1);
    typename Matrix<T>::iterator tmpmin=tmpmatrix.begin();

    for(unsigned int loopx=0;loopx<matrix.getWidth();loopx++)
      {
        if((long)loopx==(long)x) continue;
        
        for(unsigned int loopy=0;loopy<matrix.getHeight();loopy++)
          {
            if((long)loopy==(long)y) continue;
            
            *(tmpmin++)=matrix(loopx,loopy);
          }
      }

    return tmpmatrix;
  }

  template<typename T>
  T determinant(const Matrix<T> &matrix)
  {
    if(matrix.getWidth()!=matrix.getHeight()) throw(MhvlNonSquareMatrixException());
    if(matrix.size()==1) return matrix[0];

    T tmpdet=0;
    T signflt=1;

    for(unsigned int loop0=0;loop0<matrix.getWidth();loop0++)
      {
        tmpdet+=signflt*matrix(loop0,0)*determinant(minorMatrix(matrix,loop0,0));
        signflt=-signflt;
      }

    return tmpdet;
  }


  template<typename T>
  T cofactor(const Matrix<T> &matrix,int x,int y)
  {
    return pow(-1.0,y+x)*determinant(minorMatrix(matrix,x,y));
  }

  template<typename T>
  Matrix<T> invert(const Matrix<T> &matrix) throw(MhvlNonSquareMatrixException)
  {
    T det=determinant(matrix);

    if(det==0) throw(MhvlZeroDeterminantException());

    Matrix<T> inverse(matrix.getWidth(),matrix.getHeight());
    typename Matrix<T>::iterator dest=inverse.begin();

    for(unsigned int loopx=0;loopx<matrix.getWidth();loopx++)
      {
        for(unsigned int loopy=0;loopy<matrix.getHeight();loopy++)
          {
            *(dest++)=cofactor(matrix,loopy,loopx)/det;
          }
      }

    return inverse;
  }

  template<typename T>
  Matrix<T> makeIdentityMatrix(unsigned int size)
  {
    Matrix<T> tmpmatrix(size,size,0);

    typename Matrix<T>::iterator it=tmpmatrix.begin();

    for(unsigned int loop0=0;loop0<size;loop0++)
      {
        *it=1;
        it+=size+1;
      }

    return tmpmatrix;
  }

  // Create a 4 by 4 rotation matrix.
  template<typename T>
  Matrix<T> makeRotationMatrix4(T angle,T x,T y,T z)
  {
    Matrix<T> tmpmatrix(4,4);
    T radangle=MHVL_DEG2RAD_CONST*angle;
    T cosang=cos(radangle),sinang=sin(radangle);

    T onemcosang=1-cosang;
    T omcosxy=onemcosang*y*x;
    T omcosxz=onemcosang*z*x;
    T omcoszy=onemcosang*z*y;
    T sinx=sinang*x,siny=sinang*y,sinz=sinang*z;

    QuickAssignmentIterator<T> assit(tmpmatrix.begin());
    assit<<(cosang+onemcosang*x*x)<<(omcosxy+sinz)<<(omcosxz-siny)<<0
         <<(omcosxy+sinz)<<(cosang+onemcosang*y*y)<<(omcoszy+sinx)<<0
         <<(omcosxz+siny)<<(omcoszy-sinx)<<(cosang+onemcosang*z*z)<<0
         <<0<<0<<0<<1;
      
    return tmpmatrix;
  }

  template<typename T>
  std::ostream &operator<<(std::ostream &stream,const Matrix<T> &matrix)
  {
    for(unsigned int loopy=0;loopy<matrix.getHeight();loopy++)
      {
        stream<<"|";
        for(unsigned int loopx=0;loopx<matrix.getWidth();loopx++)
          {
            stream<<" "<<matrix(loopx,loopy);
          }
        stream<<" |"<<std::endl;
      }
    return stream;
  }


  //////////////////
  // Other functions
  //////////////////
  
  template<typename T>
  QuickAssignmentIterator<T> &operator<<(QuickAssignmentIterator<T> &it,const typename Matrix<T>::value_type &value)
  {
    *it.currpos=value;
    it.currpos++;
    return it;
  }


}

#endif
