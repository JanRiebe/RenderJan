#include "renderingFunctions.h"
#include "Sphere.h"
#include "RenderViewer.h"
#include "MathFunctions.h"
#include "Scene.h"
#include "Image.h"

/*
Renders the scene and returns a pointer to the rendered image.
The caller gets ownership of the returned pointer.
*/
Image* RenderScene(Scene* scene, int width, int height, int maxReflectionDepth)
{
	Image* img = new Image();
	Pixel* pix = new Pixel[width*height];

	for (int y = height - 1; y >= 0; --y)
	{
		for (int x = 0; x < width; x++)
		{
			Pixel* p = &pix[y*width + x];
			Light l = CastRay(Ray{ Point(-100, y - height / 2, x - width / 2), Point(1, 0, 0) }, &(scene->spheres), &(scene->lights), 0, maxReflectionDepth);
			p->r = l.r;
			p->g = l.g;
			p->b = l.b;
			p->a = 1;
		}
	}

	img->setPixel(pix, width, height);
	return img;
}






Light CastRay(Ray r, vector<Sphere>* objects, vector<LightSource>* lights, int recursionDepth, int maxRecursionDepth)
{
	float closest = 100000.0f;
	vector<Sphere>::iterator closestObject = objects->end();
	Point closestPoint;
	vector<Sphere>::iterator iter = objects->begin();
	for (iter; iter != objects->end(); iter++)
	{
		Point intersection;
		float distance;
		if (iter->Intersection(&r, &intersection, &distance))
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
		return CalculateOutgoingLightFromPointAtSurface(&(*closestObject), closestPoint, r, lights, objects, ++recursionDepth, maxRecursionDepth);
	else
		return{ 1, 0, 0 };
}

// Casts a ray and returns whether it did not hit an object.
// Returns true if the lamp is visible.
bool CastShadowRay(Ray r, vector<Sphere>* objects)
{
	vector<Sphere>::iterator iter = objects->begin();
	for (iter; iter != objects->end(); iter++)
	{
		if (iter->Intersection(&r))
		{
			return false;
		}
	}
	return true;
}



Light CalculateOutgoingLightFromPointAtSurface(Sphere* object, Point p, Ray viewRay, vector<LightSource>* lights, vector<Sphere>* objects, int recutsionDepth, int maxRecursionDepth)
{
	Light l = Light{ 0, 0, 0 };

	Point normal = object->GetNormalAtPoint(&p);
	float reflectivity = object->GetReflectivityAtPoint(&p);
	float refractivity = object->GetRefractivityAtPoint(&p);

		// Labertian with shadows
	vector<LightSource>::iterator lightSource = lights->begin();
	for (lightSource; lightSource != lights->end(); lightSource++)
	{
		// Shadow ray
		Point pToLight = (lightSource->position - p);
		if (CastShadowRay(Ray{ p + normal*shadowBias,  pToLight }, objects))
		{
			// If the shadow ray didn't hit any object, the light is visible.
			// Lamberian
			Point viewingDirection = pToLight.GetUnitVector();
			float facingRatio = std::max(0.0f, Point::DotProduct(&viewingDirection, &normal));
			float intensityAfterFalloff = 1;
			if(lightSource->isPoint) intensityAfterFalloff = InverseSquareFalloff(lightSource->scale, pToLight.CalculateLength());
			l.r += min(1.0f, max(0.0f, object->color.x * intensityAfterFalloff * facingRatio * lightSource->color.x));
			l.g += min(1.0f, max(0.0f, object->color.y * intensityAfterFalloff * facingRatio * lightSource->color.y));
			l.b += min(1.0f, max(0.0f, object->color.z * intensityAfterFalloff * facingRatio * lightSource->color.z));
		}
	}

	// Reflection
	if(recutsionDepth < maxRecursionDepth)
	{
		Light reflection = CastReflectionRay(&viewRay, &p, &normal, lights, objects, recutsionDepth, maxRecursionDepth);
		l += reflection * reflectivity;
	}

	// Refraction
	if(recutsionDepth < maxRecursionDepth)
	{
		Light refraction = CastRefractionRay(&viewRay, &p, &normal, lights, objects, recutsionDepth, maxRecursionDepth);
		l += refraction * refractivity;
	}

	return l;
}


Light CastReflectionRay(Ray* incommingRay, Point* reflectionPosition, Point* surfaceNormal, vector<LightSource>* lights, vector<Sphere>* objects, int recutsionDepth, int maxRecursionDepth)
{

	Point reflectionDirection = incommingRay->direction - (*surfaceNormal)*2*Point::DotProduct(surfaceNormal, &(incommingRay->direction));
	Ray reflectionRay(*reflectionPosition+*surfaceNormal*shadowBias, reflectionDirection);
	return CastRay(reflectionRay, objects, lights, recutsionDepth, maxRecursionDepth);
}

/*
Reference from http://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel

Vec3f refract(const Vec3f &I, const Vec3f &N, const float &ior)
{
float cosi = clamp(-1, 1, dotProduct(I, N));
float etai = 1, etat = ior;
Vec3f n = N;
if (cosi < 0) { cosi = -cosi; } else { std::swap(etai, etat); n= -N; }
float eta = etai / etat;
float k = 1 - eta * eta * (1 - cosi * cosi);
return k < 0 ? 0 : eta * I + (eta * cosi - sqrtf(k)) * n;
}

Some thoughts:
Would it make sense to track outside the function what the IOR of the current medium is and pass it in with the IOR of the next medium?
Maybe the ray could carry the IOR with it.

Is a total internal reflection different from a reflection?
In this case I could check whether a total internal reflection will occur and then call either reflect or refract.
Basically using the same mechanism for all surfaces.
Though, is a total internal reflection really just a reflection? Yes.

How should I determine how much reflectivity and refractivity there is?
I would need to send two rays anyway, one for reflection one for refraction.
Unless there is a total internal reflection, in which case no refraction happens.


*/



Light CastRefractionRay(Ray* incommingRay, Point* reflectionPosition, Point* surfaceNormal, vector<LightSource>* lights, vector<Sphere>* objects, float ior, float iorPrevious, int recutsionDepth, int maxRecursionDepth)
{


	Point normal = *surfaceNormal;
	float normalDotIncomming = surfaceNormal->DotProduct(incommingRay);

	if(normalDotIncomming < 0)
	{
		// The ray is comming from outside the object with the highest ior.
		// Reversing normalDotIncomming because it needs to be positive.
		normalDotIncomming = -normalDotIncomming;
	}
	else
	{
		// The ray is comming from inside the object.
		// Reversing the normal direction.
		normal = -normal;
		// Swaping around the idices of refraction.
		float tmp = ior;
		ior = iorPrevious;
		iorPrevious = tmp;
	}

	float iorRelative = iorPrevious / ior;

	float k = 1-iorRelative * iorRelative * (1-)

	Point reflectionDirection = incommingRay->direction - (*surfaceNormal)*2*Point::DotProduct(surfaceNormal, &(incommingRay->direction));
	Ray reflectionRay(*reflectionPosition+*surfaceNormal*shadowBias, reflectionDirection);
	return CastRay(reflectionRay, objects, lights, recutsionDepth, maxRecursionDepth);
}
