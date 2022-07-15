#include "Geometry.h"


#ifndef TRIANGLE_MESH_H
#define TRIANGLE_MESH_H


struct _Triple {
  int _v0;
  int _v1;
  int _v2;
};

typedef struct _Triple Triple;

struct _TriangleMesh {
  int _number_vertices;
  int _number_triangles;
	
  Vector3* _vertices;
  Triple* _triangles;
	
  Vector3* _triangle_normals;
  Vector3* _vertex_normals;

  // vertex - vertex adjacency
  int* _num_adj_vertices;
  int** _adj_vertices;
};

typedef struct _TriangleMesh TriangleMesh;


// Read a triangle mesh stored in a file using the OFF file format
void readOFF(const char* filename, TriangleMesh* tri_mesh);

// Translate a triangle mesh such that its center of mass is aligned
// with the origin of the coordinate system
void centerTriangleMesh(TriangleMesh* tri_mesh);

// Compute triangle mesh bounding box
void computeBounds(TriangleMesh* tri_mesh, Vector3* lower_bound, Vector3* upper_bound);

// Compute center of mass of the vertices
void computeCenterMass(TriangleMesh* tri_mesh, Vector3* center);

// Rescale a triangle mesh such that it has a bounding
// box with unit length diagonal
void normalizeTriangleMesh(TriangleMesh* tri_mesh);

// Precompute normal to each triangle
void computeTriangleNormals(TriangleMesh* tri_mesh);

// Precompute normal to the surface at each vertex
void computeVertexNormals(TriangleMesh* tri_mesh);

// Access the coordinates of the three vertices of the i-th triangle
void getTriangleVertices(TriangleMesh* tri_mesh, int i, Vector3 coordinates[3]);

// Access the normals to each of the three vertices of the i-th triangle
void getTriangleVertexNormals(TriangleMesh* tri_mesh, int i, Vector3 normals[3]);

// Access the (pre-computed) normal to the i-th triangle
void getTriangleNormal(TriangleMesh* tri_mesh, int i, Vector3* normal);

// Get number of triangles in the mesh
void getNumberTriangles(TriangleMesh* tri_mesh, int* num_tris);

// Get the number of vertices in the mesh
void getNumberVertices(TriangleMesh* tri_mesh, int* num_verts);

// Free memory used by the triangle mesh
void freeTriangleMeshStructures(TriangleMesh* tri_mesh);

// Build vertex-vertex adjacency map
void computeAdjacencyMap(TriangleMesh* tri_mesh);

// Get the number of adjacent vertices to the vertex i
int getNumberAdjacentVertices(TriangleMesh* tri_mesh, int i);

// Get the j-th adjacent vertex to vertex i
int getAdjacentVertex(TriangleMesh* tri_mesh, int i, int j);

// Apply one step of heat diffusion to the triangle mesh
void heatStep(TriangleMesh* tri_mesh);

#endif


