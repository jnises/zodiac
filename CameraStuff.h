#ifndef CAMERASTUFF_H_
#define CAMERASTUFF_H_

namespace megadodo
{
    class CameraLens
    {
    protected:
        float nearplane,farplane;
    public:
        CameraLens(float _nearplane,float _farplane):nearplane(_nearplane),farplane(_farplane)
        {}
        virtual ~CameraLens(){}

        virtual void apply()=0;

        virtual void setClipPlanes(float nearplane,float farplane){this->nearplane=nearplane;this->farplane=farplane;}
    };

    class ProjectionLens:public CameraLens
    {
        float aspect;
        float fov;
    public:
        ProjectionLens(float _fov,float _aspect,float _nearplane,float _farplane):CameraLens(_nearplane,_farplane),aspect(_aspect),fov(_fov)
        {}

        virtual void setAspect(float aspect){this->aspect=aspect;}
        void setFov(float fov)
        {this->fov=fov;}

        virtual void apply();
    };

    class OrthogonalLens:public CameraLens
    {
        float left,right,bottom,top;
    public:
        OrthogonalLens(float _left,float _right,float _bottom,float _top):CameraLens(-1,1),left(_left),right(_right),bottom(_bottom),top(_top)
        {}
    
        virtual void apply();
    };
}

#endif
