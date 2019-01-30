#include "stdafx.h"
#include "renderingFunctions.h"
#include "Sphere.h"
#include "RenderViewer.h"
#include "MathFunctions.h"


void RenderTestScene()
{
	int resolutionX = 500;
	int resolutionY = 300;

	vector<LightSource> lights;
	//lights.push_back(LightSource(Point(0.0f, 0.0f, -100.0f), Point(1.0f, 0.0f, 0.0f), 1.0f));
	//lights.push_back(LightSource(Point(-100.0f, 100.0f, -100.0f), Point(0.0f, 1.0f, 0.0f), 1.0f));
	//lights.push_back(LightSource(Point(-50.0f, -100.0f, -100.0f), Point(0.0f, 0.0f, 1.0f), 1.0f));
	lights.push_back(LightSource(Point(-65.0f, -45.0f, -35.0f), Point(1.0f, 1.0f, 1.0f), 30000.0f, false));

	vector<Sphere> spheres;
	spheres.push_back(Sphere{ Point(0.0f,0.0f,0.0f), 40.0f, Point(1.0f, 1.0f, 1.0f) });
	spheres.push_back(Sphere{ Point(0.0f,10.0f,20.0f), 30.0f, Point(1.0f, 1.0f, 0.0f) });
	spheres.push_back(Sphere{ Point(0.0f,-10.0f,-10.0f), 30.0f, Point(0.0f, 1.0f, 1.0f) });
	spheres.push_back(Sphere{ Point(-50.0f,-30.0f,20.0f), 10.0f, Point(1.0f, 0.0f, 0.0f) });
	//spheres.push_back(Sphere{ Point(.0f,.0f,.0f), 500.0f, Point(0.18f, 0.18f, 0.18f) });


	RenderViewer rendView;
	RenderPixelRenderViewer(&spheres, &lights, resolutionX, resolutionY, &rendView);

}





void RenderPixelRenderViewer(vector<Sphere>* spheres, vector<LightSource>* lights, const int resolutionX, const int resolutionY, RenderViewer* renderViewer)
{
	Image* img = new Image();
	Pixel* pix = new Pixel[resolutionX*resolutionY];

	for (int y = resolutionY - 1; y >= 0; --y)
	{
		for (int x = 0; x < resolutionX; x++)
		{
			Pixel* p = &pix[y*resolutionX + x];
			Light l = CastRay(Ray{ Point(-100, y - resolutionY / 2, x - resolutionX / 2), Point(1, 0, 0) }, spheres, lights);
			p->r = l.r;
			p->g = l.g;
			p->b = l.b;
			p->a = 1;
		}
	}

	img->setPixel(pix, resolutionX, resolutionY);
	renderViewer->Show(img);
}


Light CastRay(Ray r, vector<Sphere>* objects, vector<LightSource>* lights)
{
	float closest = 100000.0f;
	vector<Sphere>::iterator closestObject = objects->end();
	Point closestPoint;
	vector<Sphere>::iterator iter = objects->begin();
	for (iter; iter != objects->end(); iter++)
	{
		Point intersection;
		float distance;
		if (iter->Intersection(r, &intersection, &distance))
		{
			// If this ray hit something we test whether it is closer than all objects before.		 
			if (distance < closest && distance > 0)
			{
				// Storing the values of the closest object.
				closestObject = iter;
				closestPoint = intersection;
				closest = distance;
			}
		}
		
	}
	// Calculating the light at the closest object.
	if (closestObject != objects->end())
		return CalculateOutgoingLightFromPointAtSurface(&(*closestObject), closestPoint, r, lights, objects);
	else
		return{ 0, 0, 0 };
}

// Casts a ray and returns whether it did not hit an object.
// Returns true if the lamp is visible.
bool CastShadowRay(Ray r, vector<Sphere>* objects)
{
	vector<Sphere>::iterator iter = objects->begin();
	for (iter; iter != objects->end(); iter++)
	{
		if (iter->Intersection(r))
		{
			return false;
		}
	}
	return true;
}



Light CalculateOutgoingLightFromPointAtSurface(Sphere* object, Point p, Ray viewRay, vector<LightSource>* lights, vector<Sphere>* objects)
{
	// Labertian with shadows
	Light l = Light{ 0, 0, 0 };
	vector<LightSource>::iterator lightSource = lights->begin();
	for (lightSource; lightSource != lights->end(); lightSource++)
	{
		// Shadow ray
		Point normal = object->GetNormalAtPoint(p);
		Point pToLight = (lightSource->position - p);
		if (CastShadowRay(Ray{ p + normal*shadowBias,  pToLight }, objects))
		{
			// If the shadow ray didn't hit any object, the light is visible.
			// Lamberian				
			Point viewingDirection = pToLight.GetUnitVector();
			float facingRatio = std::max(0.0f, Point::DotProduct(&viewingDirection, &normal));
			float intensityAfterFalloff = 1;
			if(lightSource->isPoint) intensityAfterFalloff = InverseSquareFalloff(lightSource->intensity, pToLight.CalculateLength());
			l.r += min(1.0f, max(0.0f, object->albedo.x * intensityAfterFalloff * facingRatio * lightSource->color.x));
			l.g += min(1.0f, max(0.0f, object->albedo.y * intensityAfterFalloff * facingRatio * lightSource->color.y));
			l.b += min(1.0f, max(0.0f, object->albedo.z * intensityAfterFalloff * facingRatio * lightSource->color.z));
		}
	}
	return l;

}