#include <GL/gl.h>
#include <GL/glu.h>
#include "CameraStuff.h"

namespace megadodo
{

  void ProjectionLens::apply()
  {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov,aspect,nearplane,farplane);
  }

  void OrthogonalLens::apply()
  {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(left,right,bottom,top,nearplane,farplane);
  }

}
