#version 110

/**
 * @file particleshader.vert
 * @author Joel Nises
 */


const float particleSize=200.0;

uniform float surfaceLevel;

uniform vec3 sunLight;

// how much of the rgb components aren't absorbed
uniform vec3 waterTransmittance;

// natural logarithm of waterTransmittance
uniform vec3 lnWaterTransmittance; 

// emission per lenght unit for the water
uniform vec3 emission;

//varying vec4 position;
//varying float distance;
//varying vec4 eye;

// the direction of the particle, in worldspace
varying vec3 direction;

// a vector pointing up in the screen
varying vec3 updir;

// a vector pointing right in the screen
varying vec3 rightdir;

// the light that reaches the particle from the sun
varying vec3 positionColor;

// the color of the water from the viewer to the particle
varying vec3 pathColor;

// the color from the particle to infinity
varying vec3 transparancyColor;


// this function works like the main function in envshader.frag
vec3 envColor(float distance,vec3 normdir,vec3 eye)
{
  if(normdir.y>0.0) // up, towards the surface
    {
      vec3 pathColor=sunLight*emission*pow(waterTransmittance,vec3(eye.y))*(pow(waterTransmittance,distance*vec3(-normdir.y+1.0))-1.0)/((-normdir.y+1.0)*lnWaterTransmittance);
      vec3 infColor=-sunLight*emission/(vec3((normdir.y+vec3(1.0)))*lnWaterTransmittance);
      vec3 refractVec=refract(normdir,vec3(0.0,1.0,0.0),1.33);
      //vec3 refractVec=-normdir; // this one looks better

      float falloff=dot(refractVec,vec3(0.0,-1.0,0.0));
      return pathColor
        +mix(infColor,vec3(-refractVec.y)*sunLight*2.0,falloff)*pow(waterTransmittance,vec3(distance));
    }
  else
  {
    return -sunLight*emission*pow(waterTransmittance,vec3(eye.y))/(vec3((-normdir.y+vec3(1.0)))*lnWaterTransmittance);
  }
}


void main()
{
  vec4 position;
  float distance;
  vec4 eye;

  gl_Position=ftransform();
  distance=length((gl_ModelViewMatrix*gl_Vertex).xyz);
  gl_PointSize=max(particleSize/distance,1.0);

  position=gl_Vertex;
  position.y=-position.y+surfaceLevel;

  eye=vec4(-gl_ModelViewMatrix[3][0],-gl_ModelViewMatrix[3][1]+surfaceLevel,-gl_ModelViewMatrix[3][2],1.0);

  direction=normalize(gl_Vertex.xyz);

  updir=normalize((gl_ModelViewMatrixInverse*vec4(0.0,-1.0,0.0,0.0)).xyz);

  rightdir=normalize((gl_ModelViewMatrixInverse*vec4(1.0,0.0,0.0,0.0)).xyz);

  positionColor=pow(waterTransmittance,vec3(position.y+distance));
  
  pathColor=emission*pow(waterTransmittance,vec3(eye.y))*(pow(waterTransmittance,distance*vec3(-direction.y+1.0))-1.0)/((-direction.y+1.0)*lnWaterTransmittance);

  //transparancyColor=clamp(-sunLight*emission*pow(waterTransmittance,vec3(position.y))/(vec3((-direction.y+vec3(1.0)))*lnWaterTransmittance),0.0,3.0);
  transparancyColor=envColor(distance,direction,eye.xyz);

}
