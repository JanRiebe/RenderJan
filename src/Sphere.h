#pragma once

#include "DataDefinitions.h"

class Point;
class Ray;

struct Sphere
{

	Point position;

	float radius;

	Point albedo;


	bool Intersection(Ray* r, Point* outPointOfIntersection = nullptr, float* outDistance = nullptr);

	Point GetNormalAtPoint(Point* p);

	/*
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
	/*

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
				float intensityAfterFalloff = InverseSquareFalloff(lightSource->intensity, pToLight.CalculateLength());
				l.r += max(0.0, albedo.x / PI * intensityAfterFalloff * facingRatio * lightSource->color.x);
				l.g += max(0.0, albedo.y / PI * intensityAfterFalloff * facingRatio * lightSource->color.y);
				l.b += max(0.0, albedo.z / PI * intensityAfterFalloff * facingRatio * lightSource->color.z);
			}
		}
		return l;

	}
*/

};
