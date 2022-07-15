#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>

#include "TriangleMesh.h"


// Global parameters
//
// Window size
GLint g_width = 512;
GLint g_height = 512;

// Flat or smooth shading (initially flat)
GLboolean g_flat = 1;

// Model transformation
struct _ModelTransformation {
  float _x_translation;
  float _y_translation;
  float _z_translation;

  // rotation about x, y and z (in degrees)
  float _x_rotation;
  float _y_rotation;
  float _z_rotation;
};
struct _ModelTransformation g_transformation;

// type of transformation
enum Mode { ROTATION=1, TRANSLATION=3 } g_mode;

// Store the triangle mesh
TriangleMesh g_tri_mesh;


// init the mesh data-structure from the specified (as argument)
// mesh file
void initData(const char* mesh_filename) 
{
  // Read the triangle mesh data using readOFF (from TriangleMesh.{h,c})
  readOFF(mesh_filename, &g_tri_mesh);

  // center the model at the origin
  centerTriangleMesh(&g_tri_mesh);
	
  // rescale the model such that the diagonal of its bounding box
  // has unit length
  normalizeTriangleMesh(&g_tri_mesh);

  // Precompute normals (they will be used later for rendering)
  computeTriangleNormals(&g_tri_mesh);
  computeVertexNormals(&g_tri_mesh);

  // Build adjacency map
  computeAdjacencyMap(&g_tri_mesh);

  // Initialize the model transformation
  g_transformation._x_translation = 0.f;
  g_transformation._y_translation = 0.f;
  g_transformation._z_translation = -0.5f;

  g_transformation._x_rotation = 0.f;
  g_transformation._y_rotation = 0.f;
  g_transformation._z_rotation = 0.f;
}


void initGL() 
{
  GLfloat tri_mesh_diffuse[3] = {0.6f, 0.6f, 0.9f};
  GLfloat tri_mesh_specular[3] = {1.0f, 1.0f, 1.0f};
	
  GLfloat light_diffuse[3] = {1.0f, 1.0f, 1.0f};
  GLfloat light_specular[3] = {1.0f, 1.0f, 1.0f};
	
  GLfloat scene_ambient[3] = {0.2f, 0.2f, 0.2f};
	
	
  glClearColor(1,1,1,1);
	
	
  glViewport(0, 0, g_width, g_height);
	
	
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // the mesh is rescaled to have unit length diagonal for its bounding box
  // so small values for zFar are ok
  gluPerspective(60.0, (GLdouble)g_width / (GLdouble)g_height, 0.1, 5.0);
	
	
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
	
	
  // specify light and material properties
  glMaterialfv(GL_FRONT, GL_DIFFUSE, tri_mesh_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, tri_mesh_specular);
  glMateriali(GL_FRONT, GL_SHININESS, 64);
	
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
  // use default for the rest
	
	
  // specify light model
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, scene_ambient);
	
	
  // specify shading model
  glShadeModel(GL_FLAT); // default is flat shading
	
	
  // enable lights
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
	
	
  // enable depth buffer
  glEnable(GL_DEPTH_TEST);
}


// Currently this is not implemented so the window size 
// stays at the same size
void reshape(int width, int height)
{
  g_width = width;
  g_height = height;

  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // the mesh is rescaled to have unit length diagonal for its bounding box
  // so small values for zFar are ok
  gluPerspective(60.0, (GLdouble)g_width / (GLdouble)g_height, 0.1, 5.0);

  glMatrixMode(GL_MODELVIEW);
}


// Complete
// draw the triangle mesh on the screen
void display(void)
{
  GLfloat light0_position[4] = {1.f, 1.f, 1.f, 0.f};
  GLfloat light1_position[4] = {-1.f, -1.f, -1.f, 0.f};

	
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

	
  // specify light positions (object is centered with unit diagonal)
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
  glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
 
  
  // specify the transformations to be applied to the object
  glTranslatef(g_transformation._x_translation, 
	       g_transformation._y_translation, 
	       g_transformation._z_translation);
    
  glRotatef(g_transformation._x_rotation, 1.f, 0.f, 0.f);
  glRotatef(g_transformation._y_rotation, 0.f, 1.f, 0.f);
  glRotatef(g_transformation._z_rotation, 0.f, 0.f, 1.f);


  // Complete
  // draw the triangle mesh in either flat shading mode
  // or smooth shading mode, 
  // depending on the value of the flag g_flat
    /*
     フラグg_flatの値に応じて、フラットシェーディングモードまたは
     スムースシェーディングモードで三角形のメッシュを描画する。
     */
    if (g_flat) {
        
        // Complete
        // draw the triangle mesh in flat shading mode
        // フラットシェーディングモードで三角形のメッシュを描く
        
        int i;
        int triangles = g_tri_mesh._number_triangles;
        
        for(i = 0; i < triangles; ++i){
            
            int v0, v1, v2;
            Vector3 p0, p1, p2;
            Vector3 n0, n1, n2;
            
            
            v0 = g_tri_mesh._triangles[i]._v0;
            v1 = g_tri_mesh._triangles[i]._v1;
            v2 = g_tri_mesh._triangles[i]._v2;
            
            n0 = g_tri_mesh._vertex_normals[v0];
            n1 = g_tri_mesh._vertex_normals[v1];
            n2 = g_tri_mesh._vertex_normals[v2];
            
            p0 = g_tri_mesh._vertices[v0];
            p1 = g_tri_mesh._vertices[v1];
            p2 = g_tri_mesh._vertices[v2];
            
            glShadeModel(GL_FLAT);
            glBegin(GL_TRIANGLES);
            glNormal3f(n0._x, n0._y, n0._z);
            glVertex3f(p0._x, p0._y, p0._z);
            glNormal3f(n1._x, n1._y, n1._z);
            glVertex3f(p1._x, p1._y, p1._z);
            glNormal3f(n2._x, n2._y, n2._z);
            glVertex3f(p2._x, p2._y, p2._z);
            glEnd();
        }
        
    } else {
            
            // Complete
            // draw the triangle mesh in smooth shading mode
        int i;
        int triangles = g_tri_mesh._number_triangles;


        for(i = 0; i < triangles; ++i){


          int v0, v1, v2;
          Vector3 p0, p1, p2;
          Vector3 n0, n1, n2;

          v0 = g_tri_mesh._triangles[i]._v0;
          v1 = g_tri_mesh._triangles[i]._v1;
          v2 = g_tri_mesh._triangles[i]._v2;

          n0 = g_tri_mesh._vertex_normals[v0];
          n1 = g_tri_mesh._vertex_normals[v1];
          n2 = g_tri_mesh._vertex_normals[v2];

          p0 = g_tri_mesh._vertices[v0];
          p1 = g_tri_mesh._vertices[v1];
          p2 = g_tri_mesh._vertices[v2];

          glShadeModel(GL_SMOOTH);
          glBegin(GL_TRIANGLES);
          glNormal3f(n0._x, n0._y, n0._z);
          glVertex3f(p0._x, p0._y, p0._z);
          glNormal3f(n1._x, n1._y, n1._z);
          glVertex3f(p1._x, p1._y, p1._z);
          glNormal3f(n2._x, n2._y, n2._z);
          glVertex3f(p2._x, p2._y, p2._z);
          glEnd();
            
        }
        
        //glutSwapBuffers();
    }
    glutSwapBuffers();
}


void handleKeyEvents(unsigned char key, int x, int y)
{
  switch (key) {
  case 27:
    exit(0);
    break;

  case 'f':
    // toggle between flat and smooth shading
    g_flat = 1 - g_flat;
    glutPostRedisplay();
    break;

  case 't':
    g_mode = TRANSLATION;
    break;

  case 'r':
    g_mode = ROTATION;
    break;

  case 'x':
    if (g_mode == TRANSLATION) {
      g_transformation._x_translation += 0.05f;
    }
    if (g_mode == ROTATION) {
      g_transformation._x_rotation += 5.0f;
    }
    break;

  case 'X':
    if (g_mode == TRANSLATION) {
      g_transformation._x_translation -= 0.05f;
    }
    if (g_mode == ROTATION) {
      g_transformation._x_rotation -= 5.0f;
    }
    break;

  case 'y':
    if (g_mode == TRANSLATION) {
      g_transformation._y_translation += 0.05f;
    }
    if (g_mode == ROTATION) {
      g_transformation._y_rotation += 5.0f;
    }
    break;

  case 'Y':
    if (g_mode == TRANSLATION) {
      g_transformation._y_translation -= 0.05f;
    }
    if (g_mode == ROTATION) {
      g_transformation._y_rotation -= 5.0f;
    }
    break;

  case 'z':
    if (g_mode == TRANSLATION) {
      g_transformation._z_translation += 0.05f;
    }
    if (g_mode == ROTATION) {
      g_transformation._z_rotation += 5.0f;
    }
    break;

  case 'Z':
    if (g_mode == TRANSLATION) {
      g_transformation._z_translation -= 0.05f;
    }
    if (g_mode == ROTATION) {
      g_transformation._z_rotation -= 5.0f;
    }
    break;

  case 'h':
    // one step of heat diffusion
    heatStep(&g_tri_mesh);
    break;

  default:
    break;
  }
  
  glutPostRedisplay();
}


void displayUsage(const char* prog_name) {
  printf("Usage: \n");
  printf("%s mesh_file.off\n", prog_name);
}


// Clean the mesh data-structure for the loaded mesh
void cleanMesh(void) {
  freeTriangleMeshStructures(&g_tri_mesh);
}


int main(int argc, char** argv)
{
  if (argc != 2) {
    displayUsage(argv[0]);
    return 1;
  }
	

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(g_width, g_height);
  glutCreateWindow("Mesh Viewer");
	
  // Create mesh data-structures
  initData(argv[1]);
	
  // Init GL status
  initGL();
	
	
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutKeyboardFunc(handleKeyEvents);
	
	
  // Make sure that the mesh data-structure is cleaned at exit
  atexit(cleanMesh);


  glutMainLoop();
	
  return 0;
}

