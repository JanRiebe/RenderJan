#pragma once

#include "DataDefinitions.h";
#include "MathFunctions.h"
#include "LightSource.h"
#include <algorithm>

#include <iostream>
using namespace std;

struct Sphere
{

	Point position;

	float radius;

	Point albedo;


	Light Intersection(Ray r, vector<LightSource>* lights)
	{
		// Step 1: finding the position of the hit point, posHitPoint


		//Explanation https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
		float t0, t1; // solutions for t if the ray intersects
					  // analytic solution
		Point L = r.origin - position;
		float a = Point::DotProduct(&r.direction, &r.direction);
		float b = 2 * Point::DotProduct(&r.direction, &L);
		float c = Point::DotProduct(&L, &L) - radius*radius;
		if (!solveQuadratic(a, b, c, t0, t1)) return Light{ -1, -1, -1 };	// No solution found. No hit.

		if (t0 > t1) std::swap(t0, t1);

		if (t0 < 0) {
			t0 = t1; // if t0 is negative, let's use t1 instead 
			if (t0 < 0) return Light{ 0,0,0 }; // both t0 and t1 are negative 
		}

		// Use t0 to calculate the point of intersection.
		Point hitPoint = r.origin + r.direction*t0;



		//cout << "Ray hits at distance " << t0 << endl;

		// Assigning the color to the returned light.
		Light l = GetColorAtPoint(hitPoint, r, lights);
		// Assigning the distance to the returned light.
		l.z = t0;	

		return l;
	}

	Point GetNormalAtPoint(Point p)
	{
		Point normal = (p - position);
		Point::Normalise(&normal);
		return normal;
	}

	Light GetColorAtPoint(Point p, Ray viewRay, vector<LightSource>* lights)
	{
		/*
		// Normals as color
		Point normal = GetNormalAtPoint(p);
		return{ normal.x / 2 + 0.5f, normal.y / 2 + 0.5f, normal.z / 2 + 0.5f };
		*/
		/*
		// Headlight diffuse
		Point normal = GetNormalAtPoint(p);
		Point viewingDirection = -viewRay.direction;
		float facingRatio = std::max(0.0f, Point::DotProduct(&normal, &viewingDirection));
		return Light{ facingRatio, facingRatio, facingRatio, -1 };
		*/

		// Labertian
		Light l = Light{ 0, 0, 0, -1 };
		vector<LightSource>::iterator lightSource = lights->begin();
		for (lightSource; lightSource != lights->end(); lightSource++)
		{
			Point normal = GetNormalAtPoint(p);
			Point viewingDirection = (p - lightSource->position).Normalise();
			float facingRatio = std::max(0.0f, Point::DotProduct(&normal, &viewingDirection));
			l.r += max(0.0, albedo.x/PI * lightSource->intensity * facingRatio * lightSource->color.x);
			l.g += max(0.0, albedo.y/PI * lightSource->intensity * facingRatio * lightSource->color.y);
			l.b += max(0.0, albedo.z/PI *lightSource->intensity * facingRatio * lightSource->color.z);
		}
		return l;

		//TODO continue here
		// http://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/ligth-and-shadows
	}

};

