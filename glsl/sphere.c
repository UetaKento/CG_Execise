#include <stdlib.h>
#include <stdio.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <assert.h>

#include <glew.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


#ifdef __APPLE__
#undef glGenVertexArrays
#define glGenVertexArrays glGenVertexArraysAPPLE
#undef glBindVertexArray
#define glBindVertexArray glBindVertexArrayAPPLE
#endif


// Types
typedef struct {
  GLfloat x;
  GLfloat y;
  GLfloat z;
  GLfloat w;
} vec4;

typedef struct {
  GLuint projection;
  GLuint modelview;
  GLuint normal_matrix;
} Uniforms;

typedef struct {
  // Transform matrices
  GLfloat projection[4][4];
  GLfloat modelview[4][4];
  GLfloat normal_matrix[4][4];

  // Geometry information (vertex position, color and normal):
  GLint number_vertices;
  vec4 *vertex_position;
  vec4 *vertex_normal;
  vec4 *vertex_color;
} SceneData;


// Viewport size
const int VIEWPORT_WIDTH = 600;
const int VIEWPORT_HEIGHT = 600;

// Shader program handle
static GLuint g_program;

// Locations of attributes in the shaders
enum {POSITION_LOCATION, NORMAL_LOCATION};

// Location of uniform variables
static Uniforms g_uniforms;

// The array object that will hold the data
static GLuint g_vao;

// Scene data
static SceneData g_scene_data;



// Functions
static void ComputeTranslationMatrix(GLfloat a[4][4], GLfloat x,
                                     GLfloat y, GLfloat z);

static void MultiplyMatrices(GLfloat result[4][4], GLfloat lhs[4][4],
                             GLfloat rhs[4][4]);



// Index used when constructing the buffers
static GLint g_vbo_index = 0;

// Form a quad, defined by vertices (v1,v2,v3,v4) and associated normals, as two triangles.  Store the vertex positions, colors and normals in the buffer variables.
static void MakeQuad(GLfloat v1[3], GLfloat v2[3],
                     GLfloat v3[3], GLfloat v4[3],
                     GLfloat n1[3], GLfloat n2[3],
                     GLfloat n3[3], GLfloat n4[3]) {


  // Triangle 1
  g_scene_data.vertex_position[g_vbo_index].x = v1[0];
  g_scene_data.vertex_position[g_vbo_index].y = v1[1];
  g_scene_data.vertex_position[g_vbo_index].z = v1[2];
  g_scene_data.vertex_position[g_vbo_index].w = 1.0;

  g_scene_data.vertex_normal[g_vbo_index].x = n1[0];
  g_scene_data.vertex_normal[g_vbo_index].y = n1[1];
  g_scene_data.vertex_normal[g_vbo_index].z = n1[2];
  g_scene_data.vertex_normal[g_vbo_index].w = 0.0;

  g_vbo_index++;


  g_scene_data.vertex_position[g_vbo_index].x = v2[0];
  g_scene_data.vertex_position[g_vbo_index].y = v2[1];
  g_scene_data.vertex_position[g_vbo_index].z = v2[2];
  g_scene_data.vertex_position[g_vbo_index].w = 1.0;

  g_scene_data.vertex_normal[g_vbo_index].x = n2[0];
  g_scene_data.vertex_normal[g_vbo_index].y = n2[1];
  g_scene_data.vertex_normal[g_vbo_index].z = n2[2];
  g_scene_data.vertex_normal[g_vbo_index].w = 0.0;

  g_vbo_index++;
	

  g_scene_data.vertex_position[g_vbo_index].x = v3[0];
  g_scene_data.vertex_position[g_vbo_index].y = v3[1];
  g_scene_data.vertex_position[g_vbo_index].z = v3[2];
  g_scene_data.vertex_position[g_vbo_index].w = 1.0;

  g_scene_data.vertex_normal[g_vbo_index].x = n3[0];
  g_scene_data.vertex_normal[g_vbo_index].y = n3[1];
  g_scene_data.vertex_normal[g_vbo_index].z = n3[2];
  g_scene_data.vertex_normal[g_vbo_index].w = 0.0;

  g_vbo_index++;


  // Triangle 2

  g_scene_data.vertex_position[g_vbo_index].x = v1[0];
  g_scene_data.vertex_position[g_vbo_index].y = v1[1];
  g_scene_data.vertex_position[g_vbo_index].z = v1[2];
  g_scene_data.vertex_position[g_vbo_index].w = 1.0;

  g_scene_data.vertex_normal[g_vbo_index].x = n1[0];
  g_scene_data.vertex_normal[g_vbo_index].y = n1[1];
  g_scene_data.vertex_normal[g_vbo_index].z = n1[2];
  g_scene_data.vertex_normal[g_vbo_index].w = 0.0;

  g_vbo_index++;


  g_scene_data.vertex_position[g_vbo_index].x = v3[0];
  g_scene_data.vertex_position[g_vbo_index].y = v3[1];
  g_scene_data.vertex_position[g_vbo_index].z = v3[2];
  g_scene_data.vertex_position[g_vbo_index].w = 1.0;

  g_scene_data.vertex_normal[g_vbo_index].x = n3[0];
  g_scene_data.vertex_normal[g_vbo_index].y = n3[1];
  g_scene_data.vertex_normal[g_vbo_index].z = n3[2];
  g_scene_data.vertex_normal[g_vbo_index].w = 0.0;

  g_vbo_index++;


  g_scene_data.vertex_position[g_vbo_index].x = v4[0];
  g_scene_data.vertex_position[g_vbo_index].y = v4[1];
  g_scene_data.vertex_position[g_vbo_index].z = v4[2];
  g_scene_data.vertex_position[g_vbo_index].w = 1.0;

  g_scene_data.vertex_normal[g_vbo_index].x = n4[0];
  g_scene_data.vertex_normal[g_vbo_index].y = n4[1];
  g_scene_data.vertex_normal[g_vbo_index].z = n4[2];
  g_scene_data.vertex_normal[g_vbo_index].w = 0.0;

  g_vbo_index++;
}


// Approximate a sphere given its center and radius by using 
// number_subdivisions in each u- and v- directions. 
static void 
CreateSphereData(GLfloat center[], GLfloat radius, GLint number_subdivisions) {
  GLfloat*** vertex = 
    (GLfloat***)malloc(number_subdivisions * sizeof(GLfloat*));
  GLfloat*** normal = 
    (GLfloat***)malloc(number_subdivisions * sizeof(GLfloat*));
  int i, j;

  for (i = 0; i < number_subdivisions; i++) {
    vertex[i] = (GLfloat**)malloc(number_subdivisions * sizeof(GLfloat*));
    normal[i] = (GLfloat**)malloc(number_subdivisions * sizeof(GLfloat*));
  }

  for (i = 0; i < number_subdivisions; i++) {
    for (j = 0; j < number_subdivisions; j++) {
      vertex[i][j] = (GLfloat*)malloc(3 * sizeof(GLfloat));
      normal[i][j] = (GLfloat*)malloc(3 * sizeof(GLfloat));
    }
  }

  for (i = 0; i < number_subdivisions; i++) {
    GLfloat u = (GLfloat)M_PI * (GLfloat)i/(number_subdivisions-1) - 0.5f * (GLfloat)M_PI;
    for (j = 0; j < number_subdivisions; j++) {
      GLfloat v = 2.0f * (GLfloat)M_PI * (GLfloat)j/(number_subdivisions-1);

      normal[i][j][0] = cosf(v)*cosf(u);
      normal[i][j][1] = sinf(v)*cosf(u);
      normal[i][j][2] = sinf(u);

      vertex[i][j][0] = radius * normal[i][j][0] + center[0];
      vertex[i][j][1] = radius * normal[i][j][1] + center[1];
      vertex[i][j][2] = radius * normal[i][j][2] + center[2];
    }
  }

  
  // Allocate memory for the scene data:
  //  * (number_subdivisions-1)*(number_subdivisions-1) quads (see below)
  //  * 2*(number_subdivisions-1)*(number_subdivisions-1) triangles 
  // (one quad is made of two triangles, see MakeQuad())
  //  * 3*2*(number_subdivisions-1)*(number_subdivisions-1) vertices (one 
  // triangle has three vertices)
  g_scene_data.number_vertices = 
    3*2*(number_subdivisions-1)*(number_subdivisions-1);
  g_scene_data.vertex_position = 
    (vec4*)malloc(g_scene_data.number_vertices*sizeof(vec4));
  g_scene_data.vertex_color = 
    (vec4*)malloc(g_scene_data.number_vertices*sizeof(vec4));
  g_scene_data.vertex_normal = 
    (vec4*)malloc(g_scene_data.number_vertices*sizeof(vec4));

  // Create quads: one quad is made of two triangles
  for (i = 0; i < number_subdivisions-1; i++) {
    for (j = 0; j < number_subdivisions-1; j++) {
      MakeQuad(vertex[i][j], vertex[i][j+1],
               vertex[i+1][j+1], vertex[i+1][j],
               normal[i][j], normal[i][j+1],
               normal[i+1][j+1], normal[i+1][j]);
    }
  }


  for (i = 0; i < number_subdivisions; i++) {
    for (j = 0; j < number_subdivisions; j++) {
      free(vertex[i][j]);
      free(normal[i][j]);
    }
  }

  for (i = 0; i < number_subdivisions; i++) {
    free(vertex[i]);
    free(normal[i]);
  }

  free(vertex);
  free(normal);
}


static GLuint BuildShader(const char* source, GLenum shader_type) {
  GLuint shaderHandle = glCreateShader(shader_type);
  GLint success;

  glShaderSource(shaderHandle, 1, &source, 0);
  glCompileShader(shaderHandle);

  glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE) {
    fprintf(stderr, "Error in compiling shader\n");
    exit(1);
  }

  return shaderHandle;
}


// Read a shader source.
// Return a char* that needs to be freed by the caller after
// the shader is compiled.
static char* GetShaderSource(const char* filename) {
  FILE* file_handle = fopen(filename, "r");
  if (!file_handle) {
    return NULL;
  }

  fseek(file_handle, 0L, SEEK_END);
  long size = ftell(file_handle);

  fseek(file_handle, 0L, SEEK_SET);
  char* source = (char *)malloc((size+1) * sizeof(char));
  fread(source, 1, size, file_handle);

  source[size] = '\0';

  fclose(file_handle);
  return source;
}


// Create a valid shader program given the file names of a vertex shader and a 
// fragment shader.
static GLuint BuildProgram(const char* vs_filename, const char* fs_filename) {
  char* vsString = GetShaderSource(vs_filename);
  GLuint vsHandle;
  char* fsString = GetShaderSource(fs_filename);
  GLuint fsHandle;
  GLuint program;
  GLint success;
  
  if (!vsString) {
    fprintf(stderr, "Error: Can not load vertex shader\n");
    exit(1);
  }
  vsHandle = BuildShader(vsString, GL_VERTEX_SHADER);
  free(vsString);
  
  if (!fsString) {
    fprintf(stderr, "Error: Can not load fragment shader\n");
    exit(1);
  }
  fsHandle = BuildShader(fsString, GL_FRAGMENT_SHADER);
  free(fsString);
  
  program = glCreateProgram();
  if (program == 0) {
    fprintf(stderr, "Error when creating program.\n");
    exit(1);
  }
  glAttachShader(program, vsHandle);
  glAttachShader(program, fsHandle);
  glBindAttribLocation(program, POSITION_LOCATION, "vPosition");
  glBindAttribLocation(program, NORMAL_LOCATION, "vNormal");
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (success == GL_FALSE) {
    fprintf(stderr, "Error: Can not link shaders\n");
    exit(1);
  } else {
    printf("Shaders loaded and linked\n");
  }
  
  return program;
}


static void BuildGeometry(void) {
  GLuint vbo;
  GLvoid* normal_offset;
  GLfloat center[] = {0.0, 0.0, 0.0};
  GLfloat radius = 1.0;
  GLint number_subdivisions = 64;
  GLsizeiptr all_buffer_size, position_buffer_size,
  normal_buffer_size;
  GLintptr position_buffer_offset, normal_buffer_offset;


  // Create vertices (with normals and colors) on the sphere and fill 
  // the corresponding arrays in g_scene_data.
  CreateSphereData(center, radius, number_subdivisions);


  // Generate a vertex array object name and activate it
  glGenVertexArrays(1, &g_vao);
  glBindVertexArray(g_vao);


  // Generate a vertex buffer object name, activate it and initialize it
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  
  all_buffer_size = g_scene_data.number_vertices*
    (sizeof(*g_scene_data.vertex_position) + 
     sizeof(*g_scene_data.vertex_normal));
  glBufferData(GL_ARRAY_BUFFER, all_buffer_size, NULL, GL_STATIC_DRAW);
  
  position_buffer_size = g_scene_data.number_vertices *
      sizeof(*g_scene_data.vertex_position);
  position_buffer_offset = 0;
  glBufferSubData(GL_ARRAY_BUFFER, position_buffer_offset,
                  position_buffer_size, g_scene_data.vertex_position);
 
  normal_buffer_size = g_scene_data.number_vertices *
      sizeof(*g_scene_data.vertex_normal);
  normal_buffer_offset = g_scene_data.number_vertices *
      (sizeof(*g_scene_data.vertex_position));
  glBufferSubData(GL_ARRAY_BUFFER, normal_buffer_offset,
                  normal_buffer_size, g_scene_data.vertex_normal);


  glEnableVertexAttribArray(POSITION_LOCATION);
  glVertexAttribPointer(POSITION_LOCATION, 4, GL_FLOAT, GL_FALSE,
                        sizeof(vec4), 0);

  normal_offset = (GLvoid*)(
      (g_scene_data.number_vertices) * 
      (sizeof(*g_scene_data.vertex_position)));

  glEnableVertexAttribArray(NORMAL_LOCATION);
  glVertexAttribPointer(NORMAL_LOCATION, 4, GL_FLOAT, GL_FALSE,
                        sizeof(vec4), normal_offset);
}


static void LoadShaders(void) {
  g_program = BuildProgram("noise.vs.glsl", "noise.fs.glsl");

  // Get locations of uniform variables
  g_uniforms.projection = glGetUniformLocation(g_program, "PMatrix");
  g_uniforms.modelview = glGetUniformLocation(g_program, "MVMatrix");
  g_uniforms.normal_matrix = 
    glGetUniformLocation(g_program, "NormalMatrix");

  glUseProgram(g_program);
}


static void TransposeMatrix(GLfloat t_a[4][4], GLfloat a[4][4]) {
  t_a[0][0] = a[0][0];
  t_a[1][1] = a[1][1];
  t_a[2][2] = a[2][2];
  t_a[3][3] = a[3][3];

  t_a[1][0] = a[0][1];
  t_a[2][0] = a[0][2];
  t_a[3][0] = a[0][3];

  t_a[0][1] = a[1][0];
  t_a[2][1] = a[1][2];
  t_a[3][1] = a[1][3];

  t_a[0][2] = a[2][0];
  t_a[1][2] = a[2][1];
  t_a[3][2] = a[2][3];

  t_a[0][3] = a[3][0];
  t_a[1][3] = a[3][1];
  t_a[2][3] = a[3][2];
}


// Compute the invese of the 4x4 matrix a and store it in 
// inverse_a
static void InverseMatrix(GLfloat inverse_a[4][4], GLfloat a[4][4]) {
  GLfloat determinant_a = 
    (a[0][3]*a[1][2]*a[2][1]*a[3][0] - a[0][2]*a[1][3]*a[2][1]*a[3][0] - 
     a[0][3]*a[1][1]*a[2][2]*a[3][0] + a[0][1]*a[1][3]*a[2][2]*a[3][0] + 
     a[0][2]*a[1][1]*a[2][3]*a[3][0] - a[0][1]*a[1][2]*a[2][3]*a[3][0] - 
     a[0][3]*a[1][2]*a[2][0]*a[3][1] + a[0][2]*a[1][3]*a[2][0]*a[3][1] + 
     a[0][3]*a[1][0]*a[2][2]*a[3][1] - a[0][0]*a[1][3]*a[2][2]*a[3][1] - 
     a[0][2]*a[1][0]*a[2][3]*a[3][1] + a[0][0]*a[1][2]*a[2][3]*a[3][1] + 
     a[0][3]*a[1][1]*a[2][0]*a[3][2] - a[0][1]*a[1][3]*a[2][0]*a[3][2] - 
     a[0][3]*a[1][0]*a[2][1]*a[3][2] + a[0][0]*a[1][3]*a[2][1]*a[3][2] + 
     a[0][1]*a[1][0]*a[2][3]*a[3][2] - a[0][0]*a[1][1]*a[2][3]*a[3][2] - 
     a[0][2]*a[1][1]*a[2][0]*a[3][3] + a[0][1]*a[1][2]*a[2][0]*a[3][3] + 
     a[0][2]*a[1][0]*a[2][1]*a[3][3] - a[0][0]*a[1][2]*a[2][1]*a[3][3] - 
     a[0][1]*a[1][0]*a[2][2]*a[3][3] + a[0][0]*a[1][1]*a[2][2]*a[3][3]);
  
  // First column:
  inverse_a[0][0] = (-(a[1][3]*a[2][2]*a[3][1]) + a[1][2]*a[2][3]*a[3][1] + 
		     a[1][3]*a[2][1]*a[3][2] - a[1][1]*a[2][3]*a[3][2] - 
		     a[1][2]*a[2][1]*a[3][3] + a[1][1]*a[2][2]*a[3][3]) 
    / determinant_a;
     
  inverse_a[1][0] = (a[1][3]*a[2][2]*a[3][0] - a[1][2]*a[2][3]*a[3][0] - 
		     a[1][3]*a[2][0]*a[3][2] + a[1][0]*a[2][3]*a[3][2] + 
		     a[1][2]*a[2][0]*a[3][3] - a[1][0]*a[2][2]*a[3][3]) 
    / determinant_a;
     
  inverse_a[2][0] = (-(a[1][3]*a[2][1]*a[3][0]) + a[1][1]*a[2][3]*a[3][0] + 
		     a[1][3]*a[2][0]*a[3][1] - a[1][0]*a[2][3]*a[3][1] - 
		     a[1][1]*a[2][0]*a[3][3] + a[1][0]*a[2][1]*a[3][3]) 
    / determinant_a;
     
  inverse_a[3][0] = (a[1][2]*a[2][1]*a[3][0] - a[1][1]*a[2][2]*a[3][0] - 
		     a[1][2]*a[2][0]*a[3][1] + a[1][0]*a[2][2]*a[3][1] + 
		     a[1][1]*a[2][0]*a[3][2] - a[1][0]*a[2][1]*a[3][2]) 
    / determinant_a;
  
  // Second column:
  inverse_a[0][1] = (a[0][3]*a[2][2]*a[3][1] - a[0][2]*a[2][3]*a[3][1] - 
		     a[0][3]*a[2][1]*a[3][2] + a[0][1]*a[2][3]*a[3][2] + 
		     a[0][2]*a[2][1]*a[3][3] - a[0][1]*a[2][2]*a[3][3]) 
    / determinant_a;
     
  inverse_a[1][1] = (-(a[0][3]*a[2][2]*a[3][0]) + a[0][2]*a[2][3]*a[3][0] + 
		     a[0][3]*a[2][0]*a[3][2] - a[0][0]*a[2][3]*a[3][2] - 
		     a[0][2]*a[2][0]*a[3][3] + a[0][0]*a[2][2]*a[3][3]) 
    / determinant_a;
     
  inverse_a[2][1] = (a[0][3]*a[2][1]*a[3][0] - a[0][1]*a[2][3]*a[3][0] - 
		     a[0][3]*a[2][0]*a[3][1] + a[0][0]*a[2][3]*a[3][1] + 
		     a[0][1]*a[2][0]*a[3][3] - a[0][0]*a[2][1]*a[3][3]) 
    / determinant_a;
     
  inverse_a[3][1] = (-(a[0][2]*a[2][1]*a[3][0]) + a[0][1]*a[2][2]*a[3][0] + 
		     a[0][2]*a[2][0]*a[3][1] - a[0][0]*a[2][2]*a[3][1] - 
		     a[0][1]*a[2][0]*a[3][2] + a[0][0]*a[2][1]*a[3][2]) 
    / determinant_a;

  // Third column:
  inverse_a[0][2] = (-(a[0][3]*a[1][2]*a[3][1]) + a[0][2]*a[1][3]*a[3][1] + 
		     a[0][3]*a[1][1]*a[3][2] - a[0][1]*a[1][3]*a[3][2] - 
		     a[0][2]*a[1][1]*a[3][3] + a[0][1]*a[1][2]*a[3][3]) 
    / determinant_a;
     
  inverse_a[1][2] = (a[0][3]*a[1][2]*a[3][0] - a[0][2]*a[1][3]*a[3][0] - 
		     a[0][3]*a[1][0]*a[3][2] + a[0][0]*a[1][3]*a[3][2] + 
		     a[0][2]*a[1][0]*a[3][3] - a[0][0]*a[1][2]*a[3][3]) 
    / determinant_a;
          
  inverse_a[2][2] = (-(a[0][3]*a[1][1]*a[3][0]) + a[0][1]*a[1][3]*a[3][0] + 
		     a[0][3]*a[1][0]*a[3][1] - a[0][0]*a[1][3]*a[3][1] - 
		     a[0][1]*a[1][0]*a[3][3] + a[0][0]*a[1][1]*a[3][3]) 
    / determinant_a;
     
  inverse_a[3][2] = (a[0][2]*a[1][1]*a[3][0] - a[0][1]*a[1][2]*a[3][0] - 
		     a[0][2]*a[1][0]*a[3][1] + a[0][0]*a[1][2]*a[3][1] + 
		     a[0][1]*a[1][0]*a[3][2] - a[0][0]*a[1][1]*a[3][2]) 
    / determinant_a;
     
     
  // Fourth column:
  inverse_a[0][3] = (a[0][3]*a[1][2]*a[2][1] - a[0][2]*a[1][3]*a[2][1] - 
		     a[0][3]*a[1][1]*a[2][2] + a[0][1]*a[1][3]*a[2][2] + 
		     a[0][2]*a[1][1]*a[2][3] - a[0][1]*a[1][2]*a[2][3]) 
    / determinant_a;
     
  inverse_a[1][3] = (-(a[0][3]*a[1][2]*a[2][0]) + a[0][2]*a[1][3]*a[2][0] + 
		     a[0][3]*a[1][0]*a[2][2] - a[0][0]*a[1][3]*a[2][2] - 
		     a[0][2]*a[1][0]*a[2][3] + a[0][0]*a[1][2]*a[2][3]) 
    / determinant_a;
     
  inverse_a[2][3] = (a[0][3]*a[1][1]*a[2][0] - a[0][1]*a[1][3]*a[2][0] - 
		     a[0][3]*a[1][0]*a[2][1] + a[0][0]*a[1][3]*a[2][1] + 
		     a[0][1]*a[1][0]*a[2][3] - a[0][0]*a[1][1]*a[2][3]) 
    / determinant_a;
     
  inverse_a[3][3] = (-(a[0][2]*a[1][1]*a[2][0]) + a[0][1]*a[1][2]*a[2][0] + 
		     a[0][2]*a[1][0]*a[2][1] - a[0][0]*a[1][2]*a[2][1] - 
		     a[0][1]*a[1][0]*a[2][2] + a[0][0]*a[1][1]*a[2][2]) 
    / determinant_a;
}


// Fill the variable matrix with I_4
static void LoadIdentity(GLfloat matrix[4][4]) {
  int i, j;
  for (i=0; i<4; ++i) {
    for (j=0; j<4; ++j) {
      matrix[i][j] = 0.0;
    }
  }

  for (i=0; i<4; ++i) {
    matrix[i][i] = 1.0;
  }
}


// Form the matrix corresponding to a perspective projection and store 
// the result in the output variable matrix.
static void Frustum(GLfloat matrix[4][4],
                    GLfloat left, GLfloat right,
                    GLfloat bottom, GLfloat top,
                    GLfloat zNear, GLfloat zFar) {

  GLfloat sum_rl, sum_tb, sum_nf,
    inv_rl, inv_tb, inv_nf, n2;

  sum_rl = right + left;
  sum_tb = top + bottom;
  sum_nf = zNear + zFar;
  inv_rl = 1.0f / (right - left);
  inv_tb = 1.0f / (top - bottom);
  inv_nf = 1.0f / (zNear - zFar);
  n2 = zNear + zNear;

  // First column
  matrix[0][0] = n2 * inv_rl;
  matrix[1][0] = 0.0f;
  matrix[2][0] = 0.0f;
  matrix[3][0] = 0.0f;

  // Second column
  matrix[0][1] = 0.0f;
  matrix[1][1] = n2 * inv_tb;
  matrix[2][1] = 0.0f;
  matrix[3][1] = 0.0f;

  // Third column
  matrix[0][2] = sum_rl * inv_rl;
  matrix[1][2] = sum_tb * inv_tb;
  matrix[2][2] = sum_nf * inv_nf;
  matrix[3][2] = -1.0f;

  // Fourth column
  matrix[0][3] = 0.0f;
  matrix[1][3] = 0.0f;
  matrix[2][3] = n2 * inv_nf * zFar;
  matrix[3][3] = 0.0f;
}


// Init all the transform matrices (projection and modelview) needed by the 
// application.
static void InitMatrices() {
  GLfloat x = 0.6f;
  GLfloat y = x * (GLfloat)VIEWPORT_HEIGHT / (GLfloat)VIEWPORT_WIDTH;
  GLfloat left = -x, right = x;
  GLfloat bottom = -y, top = y;
  GLfloat zNear = 1.0, zFar = 100.0;

  GLfloat identity[4][4];
  GLfloat translation[4][4];
  GLfloat inverse[4][4];
  
  // Init the model view matrix 
  LoadIdentity(identity);
  ComputeTranslationMatrix(translation, 0.0f, 0.0f, -5.0f);
  MultiplyMatrices(g_scene_data.modelview, identity, translation);

  // Init the inverse of the model view matrix
  InverseMatrix(inverse, g_scene_data.modelview);
  TransposeMatrix(g_scene_data.normal_matrix, inverse);
  
  // Init the projection matrix to a perspective projection matrix
  Frustum(g_scene_data.projection, left, right, bottom, top, zNear, zFar);
}


static void InitGL(void) {
  glClearColor(1.0, 1.0, 1.0, 1.0);

  BuildGeometry();
  LoadShaders();
  
  InitMatrices();
  
  glEnable(GL_DEPTH_TEST);
}


void HandleDisplay(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBindVertexArray(g_vao);

  glUniformMatrix4fv(g_uniforms.projection, 1,
                     GL_TRUE, &g_scene_data.projection[0][0]);
  glUniformMatrix4fv(g_uniforms.modelview, 1,
                     GL_TRUE, &g_scene_data.modelview[0][0]);
  glUniformMatrix4fv(g_uniforms.normal_matrix, 1, 
		     GL_TRUE, &g_scene_data.normal_matrix[0][0]);

  glDrawArrays(GL_TRIANGLES, 0, g_scene_data.number_vertices);
  
  glutSwapBuffers();
}


// Compute in a the matrix rotation about the z-axis in counter clockwise
// order, by an angle theta, in degrees.
static void ComputeRotationZMatrix(GLfloat a[4][4], GLfloat theta) {
  int i, j;
  GLfloat t = theta * (GLfloat)M_PI / 180.0f; // t is in radian
  
  for(i=0; i<4; ++i) {
    for (j=0; j<4; ++j) {
      a[i][j] = 0.0;
    }
  }

  // Homogeneous coordinates
  a[3][3] = 1.0f;

  // Invariance along z
  a[2][2] = 1.0f;

  a[0][0] = cosf(t);
  a[1][0] = sinf(t);
  a[0][1] = -sinf(t);
  a[1][1] = cosf(t);
}


// Compute the transform matrix (4x4) corresponding to a translation by the 
// vector {tx, ty, tz}.
static void ComputeTranslationMatrix(GLfloat a[4][4], GLfloat tx,
                                     GLfloat ty, GLfloat tz) {
  LoadIdentity(a);
  a[0][3] = tx;
  a[1][3] = ty;
  a[2][3] = tz;
}


// Compute the multiplication of the matrices lhs and rhs: lhs * rhs and store 
// the result in the variable result. All matrices are 4x4.
static void MultiplyMatrices(GLfloat result[4][4],
                             GLfloat lhs[4][4],
                             GLfloat rhs[4][4]) {
  int i,j,k;

  for (i=0; i<4; ++i) {
    for (j=0; j<4; ++j) {
      GLfloat result_ij = 0.0;
      for (k=0; k<4; ++k) {
        result_ij += lhs[i][k] * rhs[k][j];
      }

      result[i][j] = result_ij;
    }
  }
}


static void HandleKeyboardEvents(unsigned char key, int x, int y) {
  switch(key) {
  case 27:
  case 'q':
  case 'Q':
    exit(EXIT_SUCCESS);
    break;
  }
}


int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
  glutInitWindowSize(VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
  glutCreateWindow("Phong illumination");

  
//#ifndef __APPLE__
  glewInit();
//#endif

  
  InitGL();

  glutDisplayFunc(HandleDisplay);
  glutKeyboardFunc(HandleKeyboardEvents);

  glutMainLoop();
  return EXIT_SUCCESS;
}

