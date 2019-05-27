#include "Sphere.h"
#include "DataDefinitions.h"
#include "MathFunctions.h"
#include "LightSource.h"
#include "renderingFunctions.h"
#include <algorithm>
#include "Material.h"

#include <iostream>
using namespace std;


Sphere::Sphere(Point pos, float scale, Material* material): SceneElement(pos, scale, material->baseColor), material(material){std::cout<<"Creating sphere with reflection "<<material->reflectivity<<std::endl;};

bool Sphere::Intersection(Ray* r, Point* outPointOfIntersection, float* outDistance)
{
  //Explanation https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
  float t0, t1; // solutions for t if the ray intersects
          // analytic solution
  Point L = r->origin - position;
  float a = Point::DotProduct(&(r->direction), &(r->direction));
  float b = 2 * Point::DotProduct(&(r->direction), &L);
  float c = Point::DotProduct(&L, &L) - scale*scale;
  if (!solveQuadratic(a, b, c, t0, t1)) return false;	// No solution found. No hit.

  if (t0 > t1) std::swap(t0, t1);

  if (t0 < 0) {
    t0 = t1; // if t0 is negative, let's use t1 instead
    if (t0 < 0) return false; // both t0 and t1 are negative
  }

  // Use t0 to calculate the point of intersection.
  if(outPointOfIntersection != nullptr)
    *outPointOfIntersection = r->origin + r->direction*t0;
  if (outDistance != nullptr)
    *outDistance = t0;

  // This was a hit, so we return true.
  return true;
}

Point Sphere::GetNormalAtPoint(Point* p)
{
  Point normal = (*p - position);
  Point::Normalise(&normal);
  return normal;
}

float Sphere::GetReflectivityAtPoint(Point* p)
{
  return material->reflectivity;
}


float Sphere::GetIORAtPoint(Point* p)
{
  return material->ior;
}
