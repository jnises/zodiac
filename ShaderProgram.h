/**
 * @file ShaderProgram.h
 * @author Joel Nises
 */

#ifndef SHADERPROGRAM_H_
#define SHADERPROGRAM_H_

#include <string>
#include <vector>
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include "GL/gl.h"
#endif

namespace megadodo
{
    class ShaderProgramException:public std::exception
    {
        std::string message;
    public:
        ShaderProgramException(std::string msg) throw():message(msg){}
        ~ShaderProgramException() throw(){}
        const char *what(){return message.c_str();}    
    };

    /** Encapsulates a GLSL shader program. */
    class ShaderProgram
    {
        std::vector<std::string> shaderfiles;
        std::vector<GLuint> shaders;
        GLuint program;
        bool linked;
    public:
        ShaderProgram();
        ~ShaderProgram() throw();

        /** @param type the gl type of shader: GL_VERTEX_SHADER or GL_FRAGMENT_SHADER . */
        void addShader(const std::string &filename,GLuint type) throw(ShaderProgramException);

        void link() throw(ShaderProgramException);

        void useShader() throw(ShaderProgramException);
        static void useFixedFunction();
        GLuint getUniform(const std::string &name);
        GLuint getAttrib(const std::string &name);
    };

}

#endif
