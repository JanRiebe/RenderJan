#pragma once

#include "DataDefinitions.h";
#include "MathFunctions.h"
#include "LightSource.h"
#include "renderingFunctions.h"
#include <algorithm>

#include <iostream>
using namespace std;

struct Sphere
{

	Point position;

	float radius;

	Point albedo;


	bool Intersection(Ray r, Point* outPointOfIntersection = nullptr, float* outDistance = nullptr)
	{
		// Step 1: finding the position of the hit point, posHitPoint


		//Explanation https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
		float t0, t1; // solutions for t if the ray intersects
					  // analytic solution
		Point L = r.origin - position;
		float a = Point::DotProduct(&r.direction, &r.direction);
		float b = 2 * Point::DotProduct(&r.direction, &L);
		float c = Point::DotProduct(&L, &L) - radius*radius;
		if (!solveQuadratic(a, b, c, t0, t1)) return false;	// No solution found. No hit.

		if (t0 > t1) std::swap(t0, t1);

		if (t0 < 0) {
			t0 = t1; // if t0 is negative, let's use t1 instead 
			if (t0 < 0) return false; // both t0 and t1 are negative
		}

		// Use t0 to calculate the point of intersection.
		if(outPointOfIntersection != nullptr)
			*outPointOfIntersection = r.origin + r.direction*t0;
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

	Point GetNormalAtPoint(Point p)
	{
		Point normal = (p - position);
		Point::Normalise(&normal);
		return normal;
	}

	//TODO put this outside of sphere, and take in sphere and call getNormal, getColor, getTexture
	Light GetColorAtPoint(Point p, Ray viewRay, vector<LightSource>* lights, vector<Sphere>* objects)
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

		// Labertian with shadows
		Light l = Light{ 0, 0, 0 };
		vector<LightSource>::iterator lightSource = lights->begin();
		for (lightSource; lightSource != lights->end(); lightSource++)
		{
			// Shadow ray
			Point normal = GetNormalAtPoint(p);
			Point pToLight = (p - lightSource->position);
			if (!CastShadowRay(Ray{ p+normal*shadowBias,  pToLight }, objects))
			{
				// If the shadow ray didn't hit any object, the light is visible.
				// Lamberian				
				Point viewingDirection = pToLight.Normalise();
				float facingRatio = std::max(0.0f, Point::DotProduct(&normal, &viewingDirection));
				l.r += max(0.0, albedo.x / PI * lightSource->intensity * facingRatio * lightSource->color.x);
				l.g += max(0.0, albedo.y / PI * lightSource->intensity * facingRatio * lightSource->color.y);
				l.b += max(0.0, albedo.z / PI *lightSource->intensity * facingRatio * lightSource->color.z);
			}
		}
		return l;

	}


};

