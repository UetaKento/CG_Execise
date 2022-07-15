#include <math.h>

#include "Geometry.h"


void add(Vector3 v1, Vector3 v2, Vector3* result) {
  result->_x = v1._x + v2._x;
  result->_y = v1._y + v2._y;
  result->_z = v1._z + v2._z;
}

void sub(Vector3 v1, Vector3 v2, Vector3* result) {
  result->_x = v1._x - v2._x;
  result->_y = v1._y - v2._y;
  result->_z = v1._z - v2._z;	
}

void mulAV(float a, Vector3 v, Vector3* result) {
  result->_x = a * v._x;
  result->_y = a * v._y;
  result->_z = a * v._z;	
}

void mulVA(Vector3 v, float a, Vector3* result) {
  mulAV(a, v, result);
}

void computeDotProduct(Vector3 v1, Vector3 v2, float* result) {
  *result = v1._x * v2._x + v1._y * v2._y + v1._z * v2._z;
}

void computeCrossProduct(Vector3 v1, Vector3 v2, Vector3* result) {
  result->_x = v1._y * v2._z - v1._z * v2._y;
  result->_y = v2._x * v1._z - v1._x * v2._z;
  result->_z = v1._x * v2._y - v2._x * v1._y;
}

void computeNorm(Vector3 v, float* result) {
  *result = sqrtf(v._x * v._x + v._y * v._y + v._z * v._z);
}

void normalize(Vector3 v, Vector3* result) {
  float norm = 0.0f;
  computeNorm(v, &norm);
	
  result->_x = v._x;
  result->_y = v._y;
  result->_z = v._z;
	
  if (norm == 0.0f) return;
	
  mulAV(1.0f/norm, v, result);
}

