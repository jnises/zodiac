#version 110

/**
 * @file envshader.frag
 * @author Joel Nises
 */

uniform vec3 waterTransmittance;
uniform vec3 lnWaterTransmittance;

uniform vec3 emission;
uniform vec3 sunLight;

uniform float time;

varying vec4 eye;
varying vec3 direction;

void main()
{
  vec3 normdir=normalize(direction);
  float distance=eye.y/normdir.y;

  /*float noise=(mod((( floor(gl_FragCoord.x)*4273.0 + floor(gl_FragCoord.y)*5483.0) 
                    //+ floor(time*1000.0) * 1223.0 
                    ),37.0)/37.0-0.5)*0.01;*/

  float noise=(
               mod(floor((gl_FragCoord.x+gl_FragCoord.y*9.0
                          //+time*0.01
                          )*4269.0),37.0) //+ mod(floor(gl_FragCoord.y)*5483.0,37.0)
               //+ floor(time*1000.0) * 1223.0 
               /37.0-0.5
               )*0.005;

  /*  float noiseseed=mod((sin(gl_FragCoord.x)+sin(gl_FragCoord.y))*time,37.0);
   
  noiseseed+=0.272727273-floor(noiseseed);
  //noiseseed-=floor(noiseseed);
  float noiseb=noiseseed=noiseseed*noiseseed;
  float noise=noiseb-floor(noiseb);

  //noise=pow(noise-0.5,2.0);*/

  /*float noise=mod((sin(gl_FragCoord.x*0.1)+cos(gl_FragCoord.y*0.1))*time*40.0,103.0)/103.0;
  noise-=0.5;
  noise*=0.1;*/
  


  if(direction.y>0.0) // up, towards the surface
    {
      // the color to the surface
      vec3 pathColor=sunLight*emission*pow(waterTransmittance,vec3(eye.y))*(pow(waterTransmittance,distance*vec3(-normdir.y+1.0))-1.0)/((-normdir.y+1.0)*lnWaterTransmittance);

      // the color after the refraction in the surface
      vec3 infColor=-sunLight*emission/(vec3((normdir.y+vec3(1.0)))*lnWaterTransmittance);

      // the surface refraction vector, will be (0,0,0) in case of total internal reflection.
      vec3 refractVec=refract(normdir,vec3(0.0,1.0,0.0),1.33);
      
      //if(any(greaterThan(refractVec,vec3(0.0))))
      //  refractVec=normalize(refractVec);
      
      //vec3 refractVec=-normdir; // this one probably looks a bit better, but isn't very correct

      // when there's a total internal reflection refractVec will be (0,0,0), falloff will in that case be 0.
      float falloff=dot(refractVec,vec3(0.0,-1.0,0.0)); 
      gl_FragColor=vec4(pathColor
                        +mix(infColor,vec3(-refractVec.y)*sunLight*2.0,falloff)*pow(waterTransmittance,vec3(distance))
                        ,1.0)+noise;
    }
  else // when looking downward we don't have to bother with the surface.
  {
    gl_FragColor=vec4(-sunLight*emission*pow(waterTransmittance,vec3(eye.y))/(vec3((-normdir.y+vec3(1.0)))*lnWaterTransmittance),1.0)+noise;
  }

  // add some noise
  //gl_FragColor+=(float((((int(gl_FragCoord.x)*4273)+(int(gl_FragCoord.y))*5483)+(int(time*1000.0)*1223))%37)/37.0-0.5)*0.01;
  //gl_FragColor+=vec4((mod((( floor(gl_FragCoord.x)*4273.0 + floor(gl_FragCoord.y)*5483.0) + floor(time*1000.0) * 1223.0 ),37.0)/37.0-0.5)*0.01);

}
