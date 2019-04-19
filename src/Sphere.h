#pragma once

#include "DataDefinitions.h"
#include "SceneElement.h"

class Point;
class Ray;

class Sphere : public SceneElement
{
private:
	float ior;
	float reflectivity;

public:
	Sphere(Point pos, float scale, Point color, float reflectivity = 0.5, float IOR = 1.4f);

	bool Intersection(Ray* r, Point* outPointOfIntersection = nullptr, float* outDistance = nullptr);

	Point GetNormalAtPoint(Point* p);

	float GetReflectivityAtPoint(Point* p);

	float GetIORAtPoint(Point* p);
};
