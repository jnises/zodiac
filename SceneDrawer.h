/**
 * @file SceneDrawer.h
 * @author Joel Nises
 */

#ifndef SCENEDRAWER_H_
#define SCENEDRAWER_H_

#include "Mhvl.h"
#include "SDL_opengl.h"
#include "SimpleText.h"
#include <string>
#include "GlobalInfo.h"
#include "ShaderProgram.h"
#include "ParticleSystem.h"
#include "EventDispatch.h"

namespace megadodo
{
  class SceneDrawer;

  class SceneDrawerException:public std::exception
  {
    std::string message;
  public:
    SceneDrawerException(std::string msg) throw():message(msg){}
    ~SceneDrawerException() throw(){}
    const char *what() const noexcept override {return message.c_str();}
  };

  class SceneInfo
  {
  public:
  };

  /** On Screen Display. */
  class OSD
  {
    const GlobalInfo *ginfo;
    megadodo::SimpleText text;

    int fps,numframes;
    double cumtime;
  public:
    OSD(const GlobalInfo *globalinfo);
    void update(double dtime);
    void draw();
  };

  class EnvBox
  {
    GLuint boxlist;
    ShaderProgram envshader;

    double timesum;

    GLuint envTime;
  public:
    EnvBox();
    ~EnvBox();
    void update(double dtime);
    void draw();

    ShaderProgram &shader(){return envshader;}
  };

  class SceneDrawer
  {
    GlobalInfo *ginfo;
    SceneInfo *sinfo;
    
    ParticleSystem *particles;
    OSD *osd;
    EnvBox *envbox;

    mhvl::Vector3f waterTransmittance;
    mhvl::Vector3f lnWaterTransmittance;

    mhvl::Vector3f emission;
    mhvl::Vector3f sunLight;

    float surfaceLevel;

    GLuint partTrans,partLnTrans,partEmission,partSun,partSurface,
      envTrans,envLnTrans,envEmission,envSun,envSurface;

    float cameraXRot,cameraYRot,mouseSenseX,mouseSenseY;
    bool mousepressed;
    mhvl::Matrixf cameramatrix;
    
      SceneDrawer(const SceneDrawer &drawer) = delete;
      SceneDrawer &operator=(const SceneDrawer &drawer) = delete;
  public:
    SceneDrawer(GlobalInfo *globalinfo,SceneInfo *sceneinfo);
    ~SceneDrawer();
    void update(double dtime);
    void draw();
    
    /** Mouse event handling method. */
    bool mouseInput(const Event &event);
  };
}
#endif
