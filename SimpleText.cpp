/*
 * SimpleText.cpp
 * @author Joel Nises
 */

#include "SimpleText.h"
#include "SDL.h"
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include "GL/gl.h"
#endif
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <fstream>

using namespace std;

namespace megadodo
{
#include "SimpleTextCharmap.h"

  template<typename T>
  string toString(T val)
  {
    stringstream tmpstream;
    tmpstream<<val;
    return tmpstream.str();
  }
  

  unsigned char b642byte(unsigned char val)
  {
    if(val>64&&val<91)
      return val-65;
    else if(val>96&&val<123)
      return val-97+26;
    else if(val>47&&val<58)
      return val-48+52;
    else if(val=='+')
      return 62;
    else if(val=='/')
      return 63;
    else throw("Error in b642byte: "+toString(val)+" is not a base64 encoded letter");
  }


  vector<unsigned char> b64decode(const string &str)
  {
    vector<unsigned char> tmpvec;
    
    for(unsigned int loop0=0;loop0<str.length()/4;loop0++)
      {
        unsigned int tmpint=0;
        int bytestoread=0;
        for(unsigned int loop1=0;loop1<4;loop1++)
          {
            if(str[loop0*4+loop1]=='=')
              {
                if(loop1==2)
                  bytestoread=1;
                else if(loop1==3)
                  bytestoread=2;

                break;
              }
            else
              bytestoread=3;

            tmpint<<=6;
            tmpint+=b642byte(str[loop0*4+loop1]);
          }
        
        for(int loop1=16;loop1>=(3-bytestoread)*8;loop1-=8)
          tmpvec.push_back((tmpint>>loop1)&0xff);
      }

    return tmpvec;
  }

  vector<unsigned char> unpackChars(const string &chardata,int width,int height)
  {
    vector<unsigned char> packed=b64decode(chardata),unpacked(width*height);
    vector<unsigned char>::iterator outit=unpacked.begin();

    for(vector<unsigned char>::iterator it=packed.begin();it!=packed.end();it++)
      {
        outit=fill_n(outit,*it&0x3f,0);
        outit=fill_n(outit,(*it>>6),255);
      }

    return unpacked;
  }

  template<typename T>
  void flipVecSurf(vector<T> &vec,int width)
  {
    if(vec.size()%width!=0)
      throw("Width doesn't match length of vector");
    
    T tmpval;

    for(unsigned int loop0=0;loop0<vec.size()/width/2;loop0++)
      {
        for(int loop1=0;loop1<width;loop1++)
          {
            tmpval=vec[loop0*width+loop1];
            vec[loop0*width+loop1]=vec[(vec.size()/width-loop0-1)*width+loop1];
            vec[(vec.size()/width-loop0-1)*width+loop1]=tmpval;
          }
      }
  
  }

  bool SimpleText::init=false;
  GLuint SimpleText::firstchar;
  GLuint SimpleText::glnum;

  SimpleText::SimpleText():minfilter(GL_NEAREST),magfilter(GL_NEAREST)
  {
    if(!init)
      {
        firstchar=glGenLists(256);


        // Read the charmap
        unsigned char *pixels=new unsigned char[charsheight*charswidth];
        vector<unsigned char> tmpvec=unpackChars(charsdata,charswidth,charsheight);
        flipVecSurf(tmpvec,charswidth);
        copy(tmpvec.begin(),tmpvec.end(),pixels);


        glGenTextures(1,&glnum);
        glBindTexture(GL_TEXTURE_2D,glnum);

        glTexImage2D(GL_TEXTURE_2D,0,GL_ALPHA,charswidth,charsheight,0,GL_ALPHA,GL_UNSIGNED_BYTE,pixels);

        delete[] pixels;
        pixels=NULL;

        /*GLenum error=glGetError();
          if(error)cout<<"GLerror :"<<gluErrorString(error)<<endl;*/


        float texbasey,texbasex;
        float delta=(float)1/16;

        for(int loop0=0;loop0<256;loop0++)
          {
            texbasey=delta*(15-loop0/16);
            texbasex=delta*(loop0%16);

            glNewList(firstchar+loop0,GL_COMPILE);

            glBegin(GL_QUADS);
	    
            glTexCoord2f(texbasex+delta*0.3,texbasey);
            glVertex2f(0,0);
  
            glTexCoord2f(texbasex+delta*0.3,texbasey+delta);
            glVertex2f(0,1);
	    
            glTexCoord2f(texbasex+delta*0.7,texbasey+delta);
            glVertex2f(charwidth,1);
	    
            glTexCoord2f(texbasex+delta*0.7,texbasey);
            glVertex2f(charwidth,0);
	    
            glEnd();

            glTranslatef(charwidth,0,0);
            glEndList();
          }

        init=true;
      }
  }
  SimpleText::~SimpleText()
  {}

  void SimpleText::print(const unsigned char *text) const
  {
    if(!text)return;

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,glnum);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,minfilter);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,magfilter);

    glListBase(firstchar);

    glCallLists(strlen((char *)text),GL_UNSIGNED_BYTE,text);

    glPopMatrix();
    glListBase(0);
  }

  void SimpleText::print(const string &text) const
  {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,glnum);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,minfilter);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,magfilter);

    glListBase(firstchar);

    unsigned char *message=(unsigned char *)text.c_str();

    glCallLists(text.length(),GL_UNSIGNED_BYTE,message);

    glPopMatrix();
    glListBase(0);
  }

  float SimpleText::getTextHeight() {return 1;} // silly

  float SimpleText::getTextWidth(const string &text) const
  {
    return text.length()*charwidth;
  }

  float SimpleText::getCharWidth()
  {
    return charwidth;
  }

  int SimpleText::getCharDim()
  {
    return 16;
  }

  int SimpleText::getCharPixWidth()
  {
    return (int)(charwidth*16);
  }
  
  void SimpleText::setFilter(GLenum minfilter,GLenum magfilter)
  {
    this->minfilter=minfilter;
    this->magfilter=magfilter;
  }
}
