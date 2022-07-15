#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "Scene.h"


#ifndef RAYTRACE_H
#define RAYTRACE_H


// Ray-trace the given scene.
// Write the output (the ray-traced image) on a texture of size 
// width by height pixels. 
void rayTraceScene(Scene scene, int width, int height, GLubyte** texture);

#endif

