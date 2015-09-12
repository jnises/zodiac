/**
 * @file SimpleText.h
 * @author Joel Nises
 */

#ifndef SIMPLETEXT_H
#define SIMPLETEXT_H

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include "GL/gl.h"
#endif

#include <cstring>
#include <string>

namespace megadodo
{
  /** A simple text drawer.
      This Class draws ugly bitmapped fonts using opengl
  */
  class SimpleText
  {
  private:
    static constexpr float charwidth=0.5;

    static GLuint firstchar;
    static bool init;
    static GLuint glnum;
    
    GLenum minfilter,magfilter;
  public:
    SimpleText();
    ~SimpleText();

    /**
	@param text a pointer to a string
    */
    void print(const unsigned char *text) const;

    /** Draws text to the screen.
	Lower left corner of the text is (0,0).
	@param text a std::string object.
    */
    void print(const std::string &text) const;

    
    float getTextHeight();
    float getTextWidth(const std::string &text) const;
    void setFilter(GLenum minfilter,GLenum magfilter);
    float getCharWidth();
    int getCharDim();
    int getCharPixWidth();
  };
}

#endif //SIMPLETEXT_H
