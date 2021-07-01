#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#include <float.h>
#include <stdlib.h>

#include "Scene.h"
#include "RayTrace.h"
#include "Geometry.h"


// Initialize the image with the background color specified as input.
// inputで指定した背景色で画像を初期化します。
// width and height corresponds to the dimension of the image.
// width と height は画像の大きさに対応します。
static void
initImageWithBackground(Color background_color, Color** image,
			int width, int height)
{
  int i;
  int j;
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      image[i][j]._red = background_color._red;
      image[i][j]._green = background_color._green;
      image[i][j]._blue = background_color._blue;
    }
  }
}


// Clamp c's entries between m and M.
// mとMの間でcのエントリーをクランプする。
static void clamp(Color* c, float m, float M) {
  c->_red = fminf(fmaxf(c->_red, m), M);
  c->_green = fminf(fmaxf(c->_green, m), M);
  c->_blue = fminf(fmaxf(c->_blue, m), M);
}


// Complete
// Given a ray (origin, direction), check if it intersects a given sphere.
// 与えられた光線（原点、方向）が、与えられた球体と交差するかどうかをチェックします。
// Return 1 if there is an intersection, 0 otherwise.
// 交差している場合は 1 を，そうでない場合は 0 を返します．
// *t contains the distance to the closest intersection point, if any.
// *t には、最も近い交点がある場合はその距離が入ります。
static int
hitSphere(Vector3 origin, Vector3 direction, Sphere sphere, float* t)
{
    float od;
    float o2;
    //float Point_along_ray = origin + t * direction;
    computeDotProduct(origin, direction, &od);
    computeDotProduct(origin, origin, &o2);
    t = -od + -sqrtf((od * od) - o2 + sphere._radius);
    
  return 0;
}


// Check if the ray defined by (scene._camera, direction) is intersecting
// any of the spheres defined in the scene.
// (scene._camera, direction)で定義されたレイが、シーンで定義された球体のどれかと交差しているかどうかをチェックします。
// Return 0 if there is no intersection, and 1 otherwise.
// 交差していない場合は 0 を，そうでない場合は 1 を返します。
//
// If there is an intersection:
// 交差している場合。
// - the position of the intersection with the closest sphere is computed
// in hit_pos
// - 最も近い球体との交点の位置はhit_posで計算されます。
// - the normal to the surface at the intersection point in hit_normal
// 交差点における表面の法線 (hit_normal)
// - the diffuse color and specular color of the intersected sphere
// in hit_color and hit_spec
// - 交差した球体の拡散色と鏡面色をhit_colorとhit_specで指定します。
static int
hitScene(Vector3 origin, Vector3 direction, Scene scene,
	 Vector3* hit_pos, Vector3* hit_normal,
	 Color* hit_color, Color* hit_spec)
{
  Vector3 o = origin;
  Vector3 d = direction;

  float t_min = FLT_MAX;
  int hit_idx = -1;
  Sphere hit_sph;

  // For each sphere in the scene
  int i;
  for (i = 0; i < scene._number_spheres; ++i) {
    Sphere curr = scene._spheres[i];
    float t = 0.0f;
    if (hitSphere(o, d, curr, &t)) {
      if (t < t_min) {
	hit_idx = i;
	t_min = t;
	hit_sph = curr;
      }
    }
  }

  if (hit_idx == -1) return 0;

  Vector3 td;
  mulAV(t_min, d, &td);
  add(o, td, hit_pos);
  
  Vector3 n;
  sub(*hit_pos, hit_sph._center, &n);
  mulAV(1.0f / hit_sph._radius, n, hit_normal);

  // Save the color of the intersected sphere in hit_color and hit_spec
  // 交差した球体の色をhit_colorとhit_specに保存する
  *hit_color = hit_sph._color;
  *hit_spec = hit_sph._color_spec;
  
  return 1;
}


// Save the image in a raw buffer (texture)
// 画像を生のバッファ（テクスチャ）に保存する
// The memory for texture is allocated in this function. It needs to be freed in the caller.
// texture用のメモリはこの関数で割り当てられます。呼び出し元で解放される必要があります。


static void saveRaw(Color** image, int width, int height, GLubyte** texture) {
  int count = 0;
  int i;
  int j;
  *texture = (GLubyte*)malloc(sizeof(GLubyte) * 3 * width * height);
	
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      unsigned char red = (unsigned char)(image[i][j]._red * 255.0f);
      unsigned char green = (unsigned char)(image[i][j]._green * 255.0f);
      unsigned char blue = (unsigned char)(image[i][j]._blue * 255.0f);
			
      (*texture)[count] = red;
      count++;
			
      (*texture)[count] = green;
      count++;
			
      (*texture)[count] = blue;
      count++;
    }
  }
}


// Complete
// Given an intersection point (hit_pos),
// the normal to the surface at the intersection point (hit_normal),
// and the color (diffuse and specular) terms at the intersection point,
// compute the colot intensity at the point by applying the Phong
// shading model.
// 交差点(hit_pos)が与えられます。交差点における表面への法線（hit_normal），
// および交差点における色（拡散および鏡面）の項が与えられた場合，
// Phongシェーディングモデル を適用して，その点における色の強度を計算します．
// Return the color intensity in *color.
//  カラー強度を *color で返します。
static void
shade(Vector3 hit_pos, Vector3 hit_normal,
      Color hit_color, Color hit_spec, Scene scene, Color* color)
{
  // Complete
  // ambient component


   // for each light in the scene
  int l;
  for (l = 0; l < scene._number_lights; l++) {
    // Complete
    // Form a shadow ray and check if the hit point is under
    // direct illumination from the light source
    // 影の光線を形成し、ヒットポイントが光源から直接照明を受けているかどうかをチェックする


    // Complete
    // diffuse component


    // Complete
    // specular component

  }
}


static int rayTrace(Vector3 origin, Vector3 direction_normalized,
		    Scene scene, Color* color)
{
  Vector3 hit_pos;
  Vector3 hit_normal;
  Color hit_color;
  Color hit_spec;
  int hit;
    
  // does the ray intersect an object in the scene?
  hit = 
    hitScene(origin, direction_normalized, scene,
	     &hit_pos, &hit_normal, &hit_color,
	     &hit_spec);
  
  // no hit
  if (!hit) return 0; 

  color->_red = 0;
  color->_green = 0;
  color->_blue = 0;

  // otherwise, apply the shading model at the intersection point
  shade(hit_pos, hit_normal, hit_color, hit_spec, scene, color);

  return 1;
}


void rayTraceScene(Scene scene, int width, int height, GLubyte** texture) {
  Color** image;
  int i;
  int j;
	
  Vector3 camera_pos;
  float screen_scale;
  
  image = (Color**)malloc(height * sizeof(Color*));
  for (i = 0; i < height; i++) {
    image[i] = (Color*)malloc(width * sizeof(Color));
  }
	
  // Init the image with the background color
  initImageWithBackground(scene._background_color, image, width, height);
  
  // get parameters for the camera position and the screen fov
  camera_pos._x = scene._camera._x;
  camera_pos._y = scene._camera._y;
  camera_pos._z = scene._camera._z;
	
  screen_scale = scene._scale;
	
	
  // go through each pixel
  // and check for intersection between the ray and the scene
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      // Compute (x,y) coordinates for the current pixel 
      // in scene space
      float x = screen_scale * j - 0.5f * screen_scale * width;
      float y = screen_scale * i - 0.5f * screen_scale * height;

      // Form the vector camera to current pixel
      Vector3 direction;
      Vector3 direction_normalized;
      
      direction._x = x - camera_pos._x;
      direction._y = y - camera_pos._y;
      direction._z = -camera_pos._z;
			
      normalize(direction, &direction_normalized);

      Vector3 origin = scene._camera;
      Color color;
      int hit = rayTrace(origin, direction_normalized, scene, &color);
      if (hit) {
	clamp(&color, 0.f, 1.f);
	image[i][j] = color;
      }
    }
  }
	
  // save image to texture buffer
  saveRaw(image, width, height, texture);

  for (i = 0; i < height; i++) {
    free(image[i]);
  }
	
  free(image);
}
