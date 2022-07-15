#ifndef GEOMETRY_H
#define GEOMETRY_H


// Data-structure to represent a vector in 3D
struct _Vector3 {
  float _x;
  float _y;
  float _z;
};

typedef struct _Vector3 Vector3;


// result = v1 + v2
void add(Vector3 v1, Vector3 v2, Vector3* result);

// result = v1 - v2
void sub(Vector3 v1, Vector3 v2, Vector3* result);

// result = a * v where:
// a is a scalar
// v is a vector (in 3D)
void mulAV(float a, Vector3 v, Vector3* result);

// result = v * a where:
// v is a vector
// a is a scalar
void mulVA(Vector3 v, float a, Vector3* result);

// result = v1 . v2 (dot product aka scalar product aka inner product)
void computeDotProduct(Vector3 v1, Vector3 v2, float* result);

// result = v1 x v2 (cross product aka outer product)
void computeCrossProduct(Vector3 v1, Vector3 v2, Vector3* result);

// result = ||v|| where ||.|| is the L2 norm
void computeNorm(Vector3 v, float* result);

// result = v / ||v|| where ||.|| is the L2 norm
void normalize(Vector3 v, Vector3* result);


#endif

