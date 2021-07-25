varying vec4 oNormal;
varying vec4 oPosition;


// ----------------------------------------------------------------------------
// Pseudo random noise function
float hash(vec3 p)
{
  p = fract(p*0.3183099+.1);
  p *= 17.0;
  return fract(p.x*p.y*p.z*(p.x+p.y+p.z));
}

float noise(in vec3 x)
{
  vec3 i = floor(x);
  vec3 f = fract(x);
  f = f*f*(3.0-2.0*f);
	
  return mix(mix(mix(hash(i+vec3(0,0,0)), 
		     hash(i+vec3(1,0,0)),f.x),
		 mix(hash(i+vec3(0,1,0)), 
		     hash(i+vec3(1,1,0)),f.x),f.y),
	     mix(mix(hash(i+vec3(0,0,1)), 
		     hash(i+vec3(1,0,1)),f.x),
		 mix(hash(i+vec3(0,1,1)), 
		     hash(i+vec3(1,1,1)),f.x),f.y),f.z);
}

vec3 noisemap(in vec3 p) {
  vec3 q = 16.0*p;
  float fx = 0.25000*noise(q);
  q = q*2.0; 
  float fy = fx + 0.12500*noise(q);
  q = q*2.0; 
  float fz = fy + 0.06250*noise(q);
  q = q*2.0;
  return p+vec3(fx,fy,fz);
}
// ----------------------------------------------------------------------------


void main(){
  // light position in eye space
  vec4 light2 = vec4(10.0, 5.0, 1.0, 1.0);

  // vertex in eye space
  vec4 V = oPosition;

  // normal in eye space
  vec4 N = oNormal;

  // apply a (pseudo-random) perturbation to the normal
  N.xyz = noisemap(N.xyz);
  N.w = 0.0;
  N = normalize(N);

  // material
  vec4 amb = vec4(0.1, 0.1, 0.1, 1.0);
  vec4 diff = vec4(1.4, 0.7, 0.6, 1.0);
  vec4 spec = vec4(1.0, 1.0, 1.0, 1.0);
  float shiny = 16.0;
  
  // light color
  vec4 lcol = vec4(1.0, 1.0, 1.0, 1.0);
  
  // Complete
  // Phong shading model
  // 

  // Complete
  // compute the vector vertex (V) to light direction

  // Complete
  // reflected light direction

  // Complete
  // Apply the Phong lighting model

  // Complete
  // Save the final color in gl_FragColor
  gl_FragColor = vec4(0.5, 0.5, 0.5, 1.0);
}
