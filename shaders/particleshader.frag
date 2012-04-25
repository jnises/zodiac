#version 110

/**
 * @file particleshader.frag
 * @author Joel Nises
 */

uniform vec3 sunLight;

//uniform vec3 waterTransmittance;
//uniform vec3 lnWaterTransmittance;

//uniform vec3 emission;

//varying vec3 position;
//varying vec3 eye;
//varying float distance;


varying vec3 direction;

varying vec3 updir;
varying vec3 rightdir;

varying vec3 positionColor;
varying vec3 pathColor;

varying vec3 transparancyColor;

const float PI=3.1415;

void main()
{
  vec2 centerpos=(gl_TexCoord[0].xy-vec2(0.5,0.5))*2.0;
  if(length(centerpos)>1.0) discard; 

  vec3 normdir=direction;//normalize(direction);
  
  /*vec2 sphereWeight=vec2(sign(centerpos.x)*pow((centerpos.x),2.0),sign(centerpos.y)*pow(centerpos.y,2.0));
    vec3 sphereNormal=normalize(rightdir*sphereWeight.x+updir*sphereWeight.y+(1.0-abs(sphereWeight.x)-abs(sphereWeight.y))*-normdir);*/

  vec3 sphereNormal=normalize(rightdir*centerpos.x+updir*centerpos.y+sqrt((1.0-centerpos.x*centerpos.x-centerpos.y*centerpos.y))*-normdir);

  vec3 ambient=vec3(0.55);

  float falloff=max(dot(sphereNormal,-normdir),0.0);
  
  vec3 reflectionVector=normalize(reflect(direction,sphereNormal));
  //vec3 refractionVector=normalize(refract(-direction,sphereNormal,0.8)); // should be another refraction when the ray leaves the sphere
  vec3 sillyReflectionVector=normalize(reflect(-direction,sphereNormal));

  gl_FragColor=vec4((pathColor+
                     positionColor*
                     (
                      pow(max(reflectionVector.y,0.0),6.0)*(0.3+0.7*(1.0-falloff))*1.2 // reflection
                      +max(sphereNormal.y,0.0)*0.25 // diffuse
                      +ambient // ambient
                      +0.1*(1.0-falloff) // fallof, makes the sphere lighter towards the edges
                      //+min(0.05*-sunLight*emission*pow(waterTransmittance,vec3(position.y))/(vec3((-refractionVector.y+vec3(1.0)))*lnWaterTransmittance)*clamp((10.0-distance)/10.0,0.0,1.0),0.2)*(max(dot(sphereNormal,-normdir),0.0)) // refraction
                      +pow(max(sillyReflectionVector.y,0.0),4.0)*falloff*0.2 // silly reflection, but it looks quite good
                      +0.1*transparancyColor // transparency
                      ))*sunLight,1.0);

}
