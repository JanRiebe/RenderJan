#pragma once

#include "DataDefinitions.h"
#include "SceneElement.h"

class Point;
class Ray;
class Material;

class Sphere : public SceneElement
{
private:
	Material* material;

public:
	Sphere(Point pos, float scale, Material* material);

	bool Intersection(Ray* r, Point* outPointOfIntersection = nullptr, float* outDistance = nullptr);

	Point GetNormalAtPoint(Point* p);

	float GetReflectivityAtPoint(Point* p);

	float GetIORAtPoint(Point* p);
};
