#include "Sphere.h"
#include "DataDefinitions.h"
#include "MathFunctions.h"
#include "LightSource.h"
#include "renderingFunctions.h"
#include <algorithm>

#include <iostream>
using namespace std;


Sphere::Sphere(Point pos, float scale, Point color):SceneElement(pos, scale, color){};

bool Sphere::Intersection(Ray* r, Point* outPointOfIntersection, float* outDistance)
{
  // Step 1: finding the position of the hit point, posHitPoint


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

  /*

  //cout << "Ray hits at distance " << t0 << endl;

  // Assigning the color to the returned light.
  Light l = GetColorAtPoint(hitPoint, r, lights);
  // Assigning the distance to the returned light.
  l.z = t0;

  return l;
  */
}

Point Sphere::GetNormalAtPoint(Point* p)
{
  Point normal = (*p - position);
  Point::Normalise(&normal);
  return normal;
}
