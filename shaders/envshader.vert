#version 110

/**
 * @file envshader.vert
 * @author Joel Nises
 */

uniform float surfaceLevel;

varying vec4 eye;
varying vec3 direction;

void main()
{
  gl_Position=ftransform();

  // gl_Vertex can be used as direction since the modelview matrix currently only includes a rotation.
  direction=gl_Vertex.xyz;

  eye=vec4(-gl_ModelViewMatrix[3][0],-gl_ModelViewMatrix[3][1]+surfaceLevel,-gl_ModelViewMatrix[3][2],1.0);

}
