#include "Scene.h"


// Assume that memory for scene has already been allocated
void createScene(Scene* scene) {
  // Set the number of objects
  scene->_number_spheres = 3;
	
  // Set property of each object:
	
  // Plane 1: 
  // (Simulate a plane by a sphere with a "very" large radius)
  (scene->_spheres[0])._radius = 10000.0f;
	
  (scene->_spheres[0])._center._x = 0.0f;
  (scene->_spheres[0])._center._y = -10005.0f;
  (scene->_spheres[0])._center._z = 0.0f;
	
  (scene->_spheres[0])._color._red = 0.25f;
  (scene->_spheres[0])._color._green = 0.25f;
  (scene->_spheres[0])._color._blue = 0.25f;

  (scene->_spheres[0])._color_spec._red = 0.0f;
  (scene->_spheres[0])._color_spec._green = 0.0f;
  (scene->_spheres[0])._color_spec._blue = 0.0f;


  // Sphere 2:
  (scene->_spheres[1])._radius = 3.0f;
	
  (scene->_spheres[1])._center._x = 4.0f;
  (scene->_spheres[1])._center._y = 2.1f;
  (scene->_spheres[1])._center._z = -4.0f;
	
  (scene->_spheres[1])._color._red = 0.9f;
  (scene->_spheres[1])._color._green = 0.5f;
  (scene->_spheres[1])._color._blue = 0.2f;

  (scene->_spheres[1])._color_spec._red = 1.0f;
  (scene->_spheres[1])._color_spec._green = 1.0f;
  (scene->_spheres[1])._color_spec._blue = 1.0f;


  // Sphere 3:
  (scene->_spheres[2])._radius = 4.f;
	
  (scene->_spheres[2])._center._x = -4.0f;
  (scene->_spheres[2])._center._y = 3.1f;
  (scene->_spheres[2])._center._z = -4.5f;
	
  (scene->_spheres[2])._color._red = 0.2f;
  (scene->_spheres[2])._color._green = 0.9f;
  (scene->_spheres[2])._color._blue = 0.5f;

  (scene->_spheres[2])._color_spec._red = 1.0f;
  (scene->_spheres[2])._color_spec._green = 1.0f;
  (scene->_spheres[2])._color_spec._blue = 1.0f;


  // Background color
  scene->_background_color._red = 1.0f;
  scene->_background_color._green = 1.0f;
  scene->_background_color._blue = 1.0f;
	
	
  // camera information
  scene->_camera._x = 0.0f;
  scene->_camera._y = 0.0f;
  scene->_camera._z = 10.0f;
	
  scene->_scale = 0.03f;
	
	
  // ambient color
  scene->_ambient._red = 0.2f;
  scene->_ambient._green = 0.2f;
  scene->_ambient._blue = 0.2f;
	

  // lights information
  scene->_number_lights = 2;
	
  (scene->_lights[0])._light_pos._x = -5.0f;
  (scene->_lights[0])._light_pos._y = 10.0f;
  (scene->_lights[0])._light_pos._z = -2.0f;
	
  (scene->_lights[0])._light_color._red = 1.f;
  (scene->_lights[0])._light_color._green = 1.f;
  (scene->_lights[0])._light_color._blue = 1.f;

  (scene->_lights[1])._light_pos._x = 5.0f;
  (scene->_lights[1])._light_pos._y = 10.0f;
  (scene->_lights[1])._light_pos._z = -2.0f;

  (scene->_lights[1])._light_color._red = 1.f;
  (scene->_lights[1])._light_color._green = 1.f;
  (scene->_lights[1])._light_color._blue = 1.f;
}

