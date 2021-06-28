#include "Geometry.h"


#ifndef SCENE_H
#define SCENE_H



// Color data-structure.
// Used in the definition of the objects in the scene
struct _Color {
  float _red;
  float _green;
  float _blue;
};

typedef struct _Color Color;


// Sphere data-structure. 
// Currently the only possible type of objects in the scene. 
struct _Sphere {
  float _radius;
  Vector3 _center;
  Color _color;
  Color _color_spec;
};

typedef struct _Sphere Sphere;


// Light data-structure.
struct _Light {
  Color _light_color;
  Vector3 _light_pos;
};

typedef struct _Light Light;


// Scene data-structure:
// Currently contains only spheres.
#define MAX_NUMBER_OBJECTS 10
#define MAX_NUMBER_LIGHTS 7
struct _Scene {
  int _number_spheres;
  Sphere _spheres[MAX_NUMBER_OBJECTS];

	
  // put background color
  Color _background_color;
	
	
  // camera information
  Vector3 _camera;
  float _scale;
	
	
  // ambient color for the scene
  Color _ambient;

	
  // Lights in the scene
  int _number_lights;
  Light _lights[MAX_NUMBER_LIGHTS];
};

typedef struct _Scene Scene;


// Create a scene - modify this function for a different scene
void createScene(Scene* scene);

#endif

