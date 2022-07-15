#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>


#include "Scene.h"
#include "RayTrace.h"


GLint g_width = 512;
GLint g_height = 512;
GLuint g_tex_id;


void initGL() 
{
  // Load the ray traced image as a texture
  GLubyte* texture;
		
  // Store a scene created in createScene()
  Scene scene;
	
	
  glClearColor(1,1,1,1);

  glViewport(0, 0, g_width, g_height);
	
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	
	
  glMatrixMode(GL_MODELVIEW);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	
  // Create a scene
  createScene(&scene);
		
  // Ray-trace the scene and save it in the texture
  rayTraceScene(scene, g_width, g_height, &texture);
	
	
  glGenTextures(1, &g_tex_id);
  glBindTexture(GL_TEXTURE_2D, g_tex_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
    
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glEnable(GL_TEXTURE_2D);
	
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, g_width, g_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);


  free(texture);
}


void reshape(int width, int height)
{}


void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
	
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, g_tex_id);
	
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
	
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(-1.0f, -1.0f);
	
  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(1.0f, -1.0f);
	
  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(1.0f, 1.0f);
	
  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(-1.0f, 1.0f);
	
  glEnd();
	
  glutSwapBuffers();
}


void handleKeyEvents(unsigned char key, int x, int y)
{
  if (key == 27) exit(0);
}


int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
  glutInitWindowSize(g_width, g_height);
  glutCreateWindow("ray casting");
	
  initGL();
	
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutKeyboardFunc(handleKeyEvents);
	
  glutMainLoop();
	
  return 0;
}

