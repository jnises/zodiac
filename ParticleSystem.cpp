/*
 * @author Joel Nises
 */

#include "glee/GLee.h"
#include "ParticleSystem.h"
#include <algorithm>
#include "simplexnoise1234.h"
#include <cmath>
#include "Utils.h"
#include <SDL_opengl.h>

using namespace std;
using namespace mhvl;

namespace megadodo
{
    ////////////////////////
    // NoiseNoiseParticleList methods
    /////////////////////////
    NoiseParticleList::NoiseParticleList(int _maxParticles):maxParticles(_maxParticles),deadPointer(0),initializePointer(0)
    {
        alivelist=new bool[maxParticles];
        fill_n(alivelist,maxParticles,false);

        positionlist=new Vector4f[maxParticles];
        velocitylist=new Vector3f[maxParticles];
        birthlist=new double[maxParticles];
        initializedlist=new bool[maxParticles];
    }

    NoiseParticleList::~NoiseParticleList()
    {
        delete [] birthlist;
        delete [] positionlist;
        delete [] velocitylist;
        delete [] alivelist;
        delete [] initializedlist;
    }

    void NoiseParticleList::kill(int index)
    {
        if(alivelist[index])
        {
            deadPointer--;
            initializePointer--;
            this->swap(deadPointer,index);
            alivelist[deadPointer]=false;
        }
    }

    int NoiseParticleList::spawn()
    {
        if(maxParticles>deadPointer)
        {
            alivelist[deadPointer]=true;
            initializedlist[deadPointer]=false;
            deadPointer++;
            return deadPointer-1;
        }
        else return LIST_FULL;
    }

    int NoiseParticleList::initializeNext()
    {
        if(initializePointer>=maxParticles) return LIST_FULL;
        if(alivelist[initializePointer])
        {
            initializedlist[initializePointer]=true;
            initializePointer++;
            return initializePointer-1;
        }
        else return NO_LIVE_PARTICLES;
    }

    void NoiseParticleList::swap(int index0,int index1)
    {
        std::swap(alivelist[index0],alivelist[index1]);
        std::swap(positionlist[index0],positionlist[index1]);
        std::swap(velocitylist[index0],velocitylist[index1]);
        std::swap(birthlist[index0],birthlist[index1]);
        std::swap(initializedlist[index0],initializedlist[index1]);
    }



    //////////////////////////////////////////
    // VolumeWindowNoiseParticleEmitter methods
    ////////////////////////////////////////////

    VolumeWindowNoiseParticleEmitter::VolumeWindowNoiseParticleEmitter(NoiseParticleList *_list,ParticleSystem *_owner):owner(_owner),list(_list)
    {
        radius=1;
        spawnWidth=-1;
        center=makeVector4f(0,0,0,1);
        surfaceLevel=10;
    }


    void VolumeWindowNoiseParticleEmitter::fill()
    {
        int currentparticle;
        while((currentparticle=list->spawn())!=NoiseParticleList::LIST_FULL)
        {
            list->birth(currentparticle)=owner->getTimeSinceStart();
            //list->position(currentparticle)=homAdd(makeVector4f((float)((double)rand()/RAND_MAX-0.5)*radius*2,(float)((double)rand()/RAND_MAX-0.5)*radius*2,(float)((double)rand()/RAND_MAX-0.5)*radius*2,1.0),center);
            list->position(currentparticle)=getRandomPositionInSphere();
        }
    }

    /** Respawn all dead particles at the edge of the emitter sphere. */
    void VolumeWindowNoiseParticleEmitter::update(double dtime)
    {
        if(list->isFull()) return;

        int currentparticle;
        while((currentparticle=list->spawn())!=NoiseParticleList::LIST_FULL)
        {
            list->position(currentparticle)=getRandomPosition();

            list->birth(currentparticle)=owner->getTimeSinceStart();
        }
    }

    Vector4f VolumeWindowNoiseParticleEmitter::getRandomPosition()
    {
        double newSpawnWidth=(spawnWidth<0?(radius*0.1):spawnWidth);

        float u=(float)((double)rand()/RAND_MAX*2*M_PI);
        float v=asinf((2*(float)((double)rand())/RAND_MAX)-1);
        float w=(float)(((double)rand())/RAND_MAX*newSpawnWidth+radius-newSpawnWidth);

        return homAdd(makeVector4f(cosf(u)*cosf(v)*w,w*sinf(v),sinf(u)*cosf(v)*w,1.0),center);
    }

    Vector4f VolumeWindowNoiseParticleEmitter::getRandomPositionInSphere()
    {
        float u=(float)((double)rand()/RAND_MAX*2*M_PI);
        float v=asinf((2*(float)((double)rand())/RAND_MAX)-1);
        float w=(float)pow(((double)rand()/RAND_MAX),1/3.0)*radius;
        //cout<<"here"<<endl;

        return homAdd(makeVector4f(cosf(u)*cosf(v)*w,w*sinf(v),sinf(u)*cosf(v)*w,1.0),center);
    }

    bool VolumeWindowNoiseParticleEmitter::isParticleInsideVolume(const mhvl::Vector4f &position) const
    {
        if(position[1]>surfaceLevel||homNorm(position-center)>radius) return false;
        return true;
    }

    ///////////////////////
    // NoiseParticleForce
    ///////////////////////

    NoiseParticleForce::NoiseParticleForce(NoiseParticleList *_list,ParticleSystem *_owner):list(_list),owner(_owner)
    {
        timeError=0;

        geomscale=0.02;
        timescale=0.02;
        forcescale=0.51;
        geombias=makeVector4f(0,0,0);
        xbias=makeVector4f(0,0.43,0.89);
        ybias=makeVector4f(0,0,0);
        zbias=makeVector4f(0.99,0,0.5);
        xyztimebias=makeVector3f(0,0.1231,0.8889);

        geomscale2=0.08;
        timescale2=0.08;
        forcescale2=0.25;
        geombias2=makeVector4f(0.3,0.99,1.3);
        xbias2=makeVector4f(0,-0.43,0.89);
        ybias2=makeVector4f(0.1,0,0);
        zbias2=makeVector4f(-0.99,0,0.5);
        xyztimebias2=makeVector3f(0,-0.1231,0.8889);

        noiseUpdateTimeError=0;

    }

    mhvl::Vector3f NoiseParticleForce::potential(mhvl::Vector3f pos) const
    {
        auto position = (pos + Vector3f(geombias)) * geomscale;
        auto position2 = (pos + Vector3f(geombias)) * geomscale2;
        return makeVector3f(snoise4(position[0]+xbias[0],position[1]+xbias[1],position[2]+xbias[2],(owner->getTimeSinceStart()+xyztimebias[0])*timescale)*forcescale+
                            snoise4(position2[0]+xbias2[0],position2[1]+xbias2[1],position2[2]+xbias2[2],(owner->getTimeSinceStart()+xyztimebias2[0])*timescale2)*forcescale2,

                            snoise4(position[0]+ybias[0],position[1]+ybias[1],position[2]+ybias[2],(owner->getTimeSinceStart()+xyztimebias[1])*timescale)*forcescale+
                            snoise4(position2[0]+ybias2[0],position2[1]+ybias2[1],position2[2]+ybias2[2],(owner->getTimeSinceStart()+xyztimebias2[1])*timescale2)*forcescale2,

                            snoise4(position[0]+zbias[0],position[1]+zbias[1],position[2]+zbias[2],(owner->getTimeSinceStart()+xyztimebias[2])*timescale)*forcescale+
                            snoise4(position2[0]+zbias2[0],position2[1]+zbias2[1],position2[2]+zbias2[2],(owner->getTimeSinceStart()+xyztimebias2[2])*timescale2)*forcescale2);

    }

    mhvl::Vector3f NoiseParticleForce::curl(mhvl::Vector3f pos) const
    {
        const float e = 1e-4f;
        auto dx = mhvl::makeVector3f(e, 0.f, 0.f);
        auto dy = mhvl::makeVector3f(0.f, e, 0.f);
        auto dz = mhvl::makeVector3f(0.f, 0.f, e);
        
        auto p = potential(pos);
        auto pdx = potential(pos + dx);
        auto pdy = potential(pos + dy);
        auto pdz = potential(pos + dz);
        

        float x = pdy[2] - p[2]
            - pdz[1] + p[1];

        float y = pdz[0] - p[0]
            - pdx[2] + p[2];

        float z = pdx[1] - p[1]
            - pdy[0] + p[0];

        return mhvl::makeVector3f(x, y, z) / e;
    }

    /**
     Updates particle index using a two-level noise function. The
     noisefunctions use the particle position and time as input.
     */
    void NoiseParticleForce::updateParticle(int index)
    {
        list->velocity(index) = curl(mhvl::Vector3f(list->position(index)));
    }

    void NoiseParticleForce::update(double dtime)
    {
        noiseUpdateTimeError+=dtime;

        // initialize all the new particles
        while(true)
        {
            int nextIndex=list->initializeNext();
            if(nextIndex<0) break;

            updateParticle(nextIndex);
        }

        // if instead of while since there is no use evaluating the noise functions multiple times for the same position.
        if(noiseUpdateTimeError>stepTime)
        {
            for(int loop0=0;loop0<list->getAliveParticles();loop0++)
            {
                updateParticle(loop0);
            }
            noiseUpdateTimeError=0;
        }
    }

    /////////////////////////////
    // ParticleSystem methods
    /////////////////////////////

    ParticleSystem::ParticleSystem(int maxparticles)
    {
        particles=new NoiseParticleList(maxparticles);
        timeSinceStart=0;
        emitter=new VolumeWindowNoiseParticleEmitter(particles,this);
        emitter->setSurfaceLevel(10);
        emitter->setRadius(20);
        emitter->fill();

        force=new NoiseParticleForce(particles,this);
        force->setStepTime(0.1);

        particleShader.addShader("shaders/particleshader.vert",GL_VERTEX_SHADER);
        particleShader.addShader("shaders/particleshader.frag",GL_FRAGMENT_SHADER);
        particleShader.link();

        //maxParticleSize=10;
    }

    ParticleSystem::~ParticleSystem()
    {
        delete force;
        delete emitter;
        delete particles;
    }


    void ParticleSystem::update(double dtime)
    {
        timeSinceStart+=dtime;

        emitter->update(dtime);
        force->update(dtime);

        // Particle position update
        for(int loop0=0;loop0<particles->getAliveParticles();loop0++)
        {
            particles->position(loop0)+=Vector4f(particles->velocity(loop0)*(float)dtime);
        }

        // Kill the particles that are outside the bounding sphere, and above surface level.
        unsigned int loop0=0;
        while(true)
        {
            if(!particles->isAlive(loop0)) break;

            Vector4f position=particles->position(loop0);

            if(!emitter->isParticleInsideVolume(position))
            {
                particles->kill(loop0);
            }
            else
                loop0++;
        }
    }

    void ParticleSystem::draw()
    {
        glEnableClientState(GL_VERTEX_ARRAY);

        glColor3f(1,1,1);
        glDisable(GL_BLEND);
        glDisable(GL_ALPHA_TEST);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_DEPTH_TEST);

        glEnable(GL_POINT_SPRITE);
        glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
        glEnable(GL_DEPTH_TEST);

        glTexEnvi(GL_POINT_SPRITE,GL_COORD_REPLACE,GL_TRUE);

        particleShader.useShader();

        glVertexPointer(4,GL_FLOAT,0,particles->getPositionPointer());
        glDrawArrays(GL_POINTS,0,particles->getAliveParticles());

        glDisable(GL_POINT_SPRITE);
        glDisable(GL_DEPTH_TEST);
    }


}
