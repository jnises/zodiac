/*
 * @author Joel Nises
 */

#include "glee/GLee.h"
#include "ShaderProgram.h"
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

namespace megadodo
{

  string readFile(string filename) throw(ShaderProgramException)
  {
    ifstream infile(filename.c_str(),ios::binary);

    if(!infile)
      {
        cerr<<"Unable to open file: "<<filename<<endl;
        throw(ShaderProgramException("Unable to open file"));
      }

    infile.seekg(0,ios::end);
    int filelen=infile.tellg();
    infile.seekg(0,ios::beg);
    
    char *tmparr=new char[filelen];
    infile.read(tmparr,filelen);
    
    string tmpstr(tmparr,filelen);
    delete [] tmparr;
        
    return tmpstr;
  }

  ShaderProgram::ShaderProgram():linked(false)
  {}

  ShaderProgram::~ShaderProgram() throw()
  {
    if(!glIsProgram(program)) return;

    for(vector<GLuint>::iterator it=shaders.begin();it!=shaders.end();it++)
      {
        if(glIsShader(*it)) 
          {
            glDetachShader(program,*it);
            glDeleteShader(*it);
          }
      }

    glDeleteProgram(program);
  }

  void ShaderProgram::addShader(const std::string &filename,GLuint type) throw(ShaderProgramException)
  {
    const char *tmparr[1];

    GLuint shader=glCreateShader(type);
    shaders.push_back(shader);
    shaderfiles.push_back(filename);
    string tmpstring=readFile(filename);
    *tmparr=tmpstring.c_str();
    glShaderSource(shader,1,tmparr,NULL);
    glCompileShader(shader);

    int tmpint;
    glGetShaderiv(shader,GL_COMPILE_STATUS,&tmpint);
    if(tmpint==GL_FALSE)
      {
        glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&tmpint);
        char *tmpchararr=new char[tmpint];
        int loglength;
        glGetShaderInfoLog(shader,tmpint,&loglength,tmpchararr); 
        cerr<<"Error in "<<filename<<" : "<<endl<<tmpchararr<<endl;
        delete [] tmpchararr;
        throw(ShaderProgramException("Unable to compile shader"));
      }
  }

  void ShaderProgram::link() throw(ShaderProgramException)
  {
    program=glCreateProgram();
    
    for(vector<GLuint>::iterator it=shaders.begin();it!=shaders.end();it++)
      glAttachShader(program,*it);

    glLinkProgram(program);

    int tmpint;
    glGetProgramiv(program,GL_LINK_STATUS,&tmpint);
    if(tmpint==GL_FALSE)
      {
        glGetProgramiv(program,GL_INFO_LOG_LENGTH,&tmpint);
        char *tmpchararr=new char[tmpint];
        int loglength;
        glGetProgramInfoLog(program,tmpint,&loglength,tmpchararr); 
        cerr<<"Error when linking ";
        for(vector<string>::iterator it=shaderfiles.begin();it!=shaderfiles.end();it++)
          cerr<<*it<<", ";
        cerr<<" : "<<endl<<tmpchararr<<endl;
        delete [] tmpchararr;
        throw(ShaderProgramException("Unable to link shader program"));
      }
    linked=true;
  }

  void ShaderProgram::useShader() throw(ShaderProgramException)
  {
    if(!linked) 
      {
        cerr<<"Can't use unlinked shader"<<endl;
        throw(ShaderProgramException("Can't use unlinked shader"));
      }
    glUseProgram(program);
  }

  void ShaderProgram::useFixedFunction()
  {glUseProgram(0);}

  GLuint ShaderProgram::getUniform(const string &name)
  {
    return glGetUniformLocation(program,name.c_str());
  }

  GLuint ShaderProgram::getAttrib(const std::string &name)
  {
    return glGetAttribLocation(program,name.c_str());
  }
}
