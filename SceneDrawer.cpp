/*
 * @author Joel Nises
 */

#include "glee/GLee.h"
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include "GL/gl.h"
#include "GL/glu.h"
#endif

#include "SceneDrawer.h"
#include "Utils.h"
#include <iterator>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include "Mhvl.h"
#include "SDL.h"
#include <boost/bind.hpp>

using namespace mhvl;
using namespace std;
using namespace boost;

namespace megadodo
{
    /*
     * Draws a solid cube. Code stolen from freeglut
     */
    void glutSolidCube( GLdouble dSize )
    {
        double size = dSize * 0.5;
        
#   define V(a,b,c) glVertex3d( a size, b size, c size );
#   define N(a,b,c) glNormal3d( a, b, c );
        
        /* PWO: Again, I dared to convert the code to use macros... */
        glBegin( GL_QUADS );
        N( 1.0, 0.0, 0.0); V(+,-,+); V(+,-,-); V(+,+,-); V(+,+,+);
        N( 0.0, 1.0, 0.0); V(+,+,+); V(+,+,-); V(-,+,-); V(-,+,+);
        N( 0.0, 0.0, 1.0); V(+,+,+); V(-,+,+); V(-,-,+); V(+,-,+);
        N(-1.0, 0.0, 0.0); V(-,-,+); V(-,+,+); V(-,+,-); V(-,-,-);
        N( 0.0,-1.0, 0.0); V(-,-,+); V(-,-,-); V(+,-,-); V(+,-,+);
        N( 0.0, 0.0,-1.0); V(-,-,-); V(-,+,-); V(+,+,-); V(+,-,-);
        glEnd();
        
#   undef V
#   undef N
    }
    
    
    SceneDrawer::SceneDrawer(GlobalInfo *globalinfo,SceneInfo *sceneinfo):ginfo(globalinfo),sinfo(sceneinfo),cameramatrix(Matrixf::identityMatrix(4))
    {
        particles=new ParticleSystem(10000);
        osd=new OSD(globalinfo);
        
        cameraXRot=0;
        cameraYRot=0;
        
        mouseSenseX=0.5;
        mouseSenseY=0.5;
        
        ginfo->eventDispatch->addHandler(bind(&SceneDrawer::mouseInput,this,_1));
        
        // Set up the gl state
        glClearColor(0,0,0,1);
        glViewport(0,0,ginfo->winwidth,ginfo->winheight);
        
        envbox=new EnvBox();
        
        //waterTransmittance=makeVector3f(0.8,0.8,0.8);
        waterTransmittance=makeVector3f(0.79,0.87,0.9);
        lnWaterTransmittance=makeVector3f(log(waterTransmittance[0]),log(waterTransmittance[1]),log(waterTransmittance[2]));
        
        emission=makeVector3f(0.06,0.06,0.06);
        sunLight=makeVector3f(2.05,2.0,1.9);
        
        surfaceLevel=13;
        
        particles->shader().useShader();
        
        partTrans=particles->shader().getUniform("waterTransmittance");
        partLnTrans=particles->shader().getUniform("lnWaterTransmittance");
        partEmission=particles->shader().getUniform("emission");
        partSun=particles->shader().getUniform("sunLight");
        partSurface=particles->shader().getUniform("surfaceLevel");
        
        glUniform3fv(partTrans,1,(float *)(&waterTransmittance));
        glUniform3fv(partLnTrans,1,(float *)(&lnWaterTransmittance));
        glUniform3fv(partEmission,1,(float *)(&emission));
        glUniform3fv(partSun,1,(float *)(&sunLight));
        glUniform1f(partSurface,surfaceLevel);
        
        envbox->shader().useShader();
        
        envTrans=envbox->shader().getUniform("waterTransmittance");
        envLnTrans=envbox->shader().getUniform("lnWaterTransmittance");
        envEmission=envbox->shader().getUniform("emission");
        envSun=envbox->shader().getUniform("sunLight");
        envSurface=envbox->shader().getUniform("surfaceLevel");
        
        glUniform3fv(envTrans,1,(float *)(&waterTransmittance));
        glUniform3fv(envLnTrans,1,(float *)(&lnWaterTransmittance));
        glUniform3fv(envEmission,1,(float *)(&emission));
        glUniform3fv(envSun,1,(float *)(&sunLight));
        glUniform1f(envSurface,surfaceLevel);
        
        
    }
    
    SceneDrawer::~SceneDrawer()
    {
        delete envbox;
        delete osd;
        delete particles;
    }
    
    void SceneDrawer::update(double dtime)
    {
        particles->update(dtime);
        
        osd->update(dtime);
        
        envbox->update(dtime);
    }
    
    void SceneDrawer::draw()
    {
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        glEnable(GL_DITHER);
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(50,ginfo->aspect,0.1,100);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        glMultMatrixf(cameramatrix.data());
        
        envbox->draw();
        
        particles->draw();
        
        osd->draw();
    }
    
    bool SceneDrawer::mouseInput(const Event &event)
    {
        switch(event.event->type)
        {
            case SDL_MOUSEMOTION:
                if(event.event->motion.state)
                {
                    cameraXRot+=event.event->motion.xrel*mouseSenseX;
                    cameraYRot+=event.event->motion.yrel*mouseSenseY;
                    
                    if(cameraYRot>90) cameraYRot=90;
                    if(cameraYRot<-90) cameraYRot=-90;
                    
                    cameramatrix=Matrixf::identityMatrix(4);
                    cameramatrix*=makeRotationMatrix4(cameraYRot,1.0f,0.0f,0.0f);
                    cameramatrix*=makeRotationMatrix4(cameraXRot,0.0f,1.0f,0.0f);
                }
                return true;
        }
        
        return false;
    }
    
    
    OSD::OSD(const GlobalInfo *globalinfo):ginfo(globalinfo),fps(42),numframes(0),cumtime(0)
    {  }
    
    void OSD::update(double dtime)
    {
        if((cumtime+=dtime)>1)
        {
            fps=numframes;
            cumtime-=1;
            numframes=1;
        }
        numframes++;
    }
    
    void OSD::draw()
    {
        // OSD
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);
        glUseProgram(0);
        glMatrixMode(GL_TEXTURE);
        glLoadIdentity();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(-ginfo->aspect,ginfo->aspect,-1,1);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        glDisable(GL_CULL_FACE);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER,0.5);
        glEnable(GL_TEXTURE_2D);
        glDisable(GL_TEXTURE_CUBE_MAP);
        glColor3f(1,1,1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(-ginfo->aspect,0.92,0);
        glScalef(0.08,0.08,1);
        text.print("FPS: "+toString(fps));
        
        glLoadIdentity();
        glTranslatef(-ginfo->aspect,-1,0);
        glScalef(0.08,0.08,1);
        text.print("Zodiac");
        
        
        glDisable(GL_ALPHA_TEST);
    }
    
    
    EnvBox::EnvBox()
    {
        timesum=0;
        
        boxlist=glGenLists(1);
        glNewList(boxlist,GL_COMPILE);
        glutSolidCube(1);
        glEndList();
        
        envshader.addShader("shaders/envshader.vert",GL_VERTEX_SHADER);
        envshader.addShader("shaders/envshader.frag",GL_FRAGMENT_SHADER);
        envshader.link();
        
        envTime=envshader.getUniform("time");
    }
    
    EnvBox::~EnvBox()
    {
        glDeleteLists(boxlist,1);
    }
    
    void EnvBox::update(double dtime)
    {timesum+=dtime;}
    
    void EnvBox::draw()
    {
        glDisable(GL_DEPTH_TEST);
        
        envshader.useShader();
        glUniform1f(envTime,timesum);
        
        glCallList(boxlist);
    }
    
    
}
