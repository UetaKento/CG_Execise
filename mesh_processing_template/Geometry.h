#ifndef GEOMETRY_H
#define GEOMETRY_H

struct _Vector3 {
  float _x;
  float _y;
  float _z;
};

typedef struct _Vector3 Vector3;


void add(Vector3 v1, Vector3 v2, Vector3* result);
void sub(Vector3 v1, Vector3 v2, Vector3* result);
void mulAV(float a, Vector3 v, Vector3* result);
void mulVA(Vector3 v, float a, Vector3* result);
void computeDotProduct(Vector3 v1, Vector3 v2, float* result);
void computeCrossProduct(Vector3 v1, Vector3 v2, Vector3* result);
void computeNorm(Vector3 v, float* result);
void normalize(Vector3 v, Vector3* result);


#endif
