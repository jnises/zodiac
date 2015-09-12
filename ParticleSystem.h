/**
 * @file ParticleSystem.h
 * @author Joel Nises
 */

#ifndef PARTICLESYSTEM_H_
#define PARTICLESYSTEM_H_

#include <vector>
#include "Mhvl.h"
#include "ShaderProgram.h"

namespace megadodo
{
    class ParticleSystem;

    class SimulatorBase
    {
    protected:
        double stepTime;
    public:
        SimulatorBase():stepTime(0.01){}
        virtual ~SimulatorBase(){}

        /** Each simulator should be able to have it's own separate internal
            steptime. */
        virtual void setStepTime(double stepTime){this->stepTime=stepTime;}
        //virtual void update(double dtime)=0;
    };

    /** Contains arrays of all the particle attributes. */
    class NoiseParticleList
    {
        int maxParticles;
        int deadPointer;
        int initializePointer;

        bool *alivelist;
        bool *initializedlist;
        mhvl::Vector4f *positionlist;
        mhvl::Vector3f *velocitylist;
        double *birthlist;
    public:
        static const int LIST_FULL=-1,
            NO_LIVE_PARTICLES=-2;


        NoiseParticleList(int maxParticles);
        ~NoiseParticleList();

        bool isAlive(int index) const {return alivelist[index];}

        /** Note! this method replaces the particle at index with another
            particle. */
        void kill(int index);

        /** Spawn a particle and return it's index. */
        int spawn();

        /** Initialize a particle and return it's index. This method only
            sets the initialized flag for the particle, and assumes the caller
            initialize the rest of the attributes. 
        */
        int initializeNext();
    
        mhvl::Vector4f &position(int index){return positionlist[index];}
        mhvl::Vector3f &velocity(int index){return velocitylist[index];}
        double &birth(int index){return birthlist[index];}
        //bool &initialized(int index){return initializedlist[index];}
        bool isInitilized(int index) const {return initializedlist[index];}

        void swap(int index0,int index1);

        bool isFull() const {return deadPointer>=maxParticles;}
        int getMaxParticles() const {return maxParticles;}
        int getAliveParticles() const {return deadPointer;}

        /** Returns the pointer to the position array, for use by glArrayPointer
            and such.
        */
        const mhvl::Vector4f *getPositionPointer() const {return positionlist;}
    };

    /** Particle emitter. */
    class VolumeWindowNoiseParticleEmitter
    {
        ParticleSystem *owner;

        NoiseParticleList *list;
    
        mhvl::Vector4f center;
        float radius,volume;
        mhvl::Vector3f movement;
        float surfaceLevel;
        float spawnWidth;

        mhvl::Vector4f getRandomPosition();
        mhvl::Vector4f getRandomPositionInSphere();
    public:
        VolumeWindowNoiseParticleEmitter(NoiseParticleList *list,ParticleSystem *owner);

        void setCenter(const mhvl::Vector4f &center){this->center=center;}
        const mhvl::Vector4f &getCenter(){return center;}

        void setRadius(float radius){this->radius=radius;volume=radius*radius*radius*M_PI*1.33333;}
        float getRadius(){return radius;}

        /** Fill the emitter sphere. */
        void fill();
    
        void moveCenter(const mhvl::Vector3f &movement){this->movement=movement;}

        void setSurfaceLevel(float surfaceLevel){this->surfaceLevel=surfaceLevel;}
        float getSurfaceLevel(){return surfaceLevel;}

        void update(double dtime);

        bool isParticleInsideVolume(const mhvl::Vector4f &position) const;
    };

    /** A noisebased force. */
    class NoiseParticleForce:public SimulatorBase
    {
        NoiseParticleList *list;
        ParticleSystem *owner;
        double timeError;
        float geomscale ///< scale factor for the position.
            ,timescale ///< scale factor for the time
            ,forcescale; ///< scale factor for the output
        mhvl::Vector4f geombias ///< geometry translation
            ,xbias ///< position translation for the x component
            ,ybias ///< position translation for the y component
            ,zbias; ///< position translation for the z component
        mhvl::Vector3f xyztimebias; ///< time bias for the time component

        float geomscale2,timescale2,forcescale2;
        mhvl::Vector4f geombias2,xbias2,ybias2,zbias2;
        mhvl::Vector3f xyztimebias2;

        double noiseUpdateTimeError;

        void updateParticle(int index);
    public:
        NoiseParticleForce(NoiseParticleList *list,ParticleSystem *owner);

        void update(double dtime);
    };

    /** The main particle system class. */
    class ParticleSystem:public SimulatorBase
    {
        NoiseParticleList *particles;
        //float *particleDistance;

        double timeSinceStart;

        VolumeWindowNoiseParticleEmitter *emitter;
        NoiseParticleForce *force;

    
        ShaderProgram particleShader;

        ParticleSystem(const ParticleSystem &system);
        ParticleSystem &operator=(const ParticleSystem &system);
    public:
        ParticleSystem(int maxparticles);
        ~ParticleSystem();

        void update(double dtime);
        void draw();

        double getTimeSinceStart() const {return timeSinceStart;}

        ShaderProgram &shader(){return particleShader;}

        //void resize(int newSize);
    };
}

#endif
