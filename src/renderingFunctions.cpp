#include "renderingFunctions.h"
#include "Sphere.h"
#include "RenderViewer.h"
#include "MathFunctions.h"
#include "Scene.h"
#include "Image.h"
#include <iostream>

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
			Light l = CastRay(Ray(/*origin*/ Point(-100, y - height / 2, x - width / 2), /*direction*/ Point(1, 0, 0), /*initial IOR*/1.0), &(scene->spheres), &(scene->lights), 0, maxReflectionDepth);
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



Light CalculateOutgoingLightFromPointAtSurface(Sphere* object, Point p, Ray viewRay, vector<LightSource>* lights, vector<Sphere>* objects, int recursionDepth, int maxRecursionDepth)
{
	Light l = Light{ 0, 0, 0 };

	Point normal = object->GetNormalAtPoint(&p);

	// Labertian with shadows
	vector<LightSource>::iterator lightSource = lights->begin();
	for (lightSource; lightSource != lights->end(); lightSource++)
	{
		// Shadow ray
		Point pToLight = (lightSource->position - p);
		if (CastShadowRay(Ray( p + normal*shadowBias,  pToLight, viewRay.lastIOR ), objects))
		{
			// If the shadow ray didn't hit any object, the light is visible.
			// Lamberian
			Point viewingDirection = pToLight.GetUnitVector();
			float facingRatio = std::max(0.0f, Point::DotProduct(&viewingDirection, &normal));
			float intensityAfterFalloff = 1;
			if(lightSource->isPoint) intensityAfterFalloff = InverseSquareFalloff(lightSource->scale, pToLight.CalculateLength());
			l.r += clamp(0, 1, object->color.x * intensityAfterFalloff * facingRatio * lightSource->color.x);
			l.g += clamp(0, 1, object->color.y * intensityAfterFalloff * facingRatio * lightSource->color.y);
			l.b += clamp(0, 1, object->color.z * intensityAfterFalloff * facingRatio * lightSource->color.z);
		}
	}

	// Reflection and refraction
	if(recursionDepth < maxRecursionDepth)
	{
		Light rf = ReflectRefract(&viewRay, &p, &normal, object, lights, objects, recursionDepth, maxRecursionDepth);
		std::cout <<"r "<<rf.r<<" g "<<rf.g<<" b "<<rf.b<<std::endl;
		l+= rf;
	}

	return l;
}

/*
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



A ray comes in.
It is separated into reflection and refraction (they sum up to 1).
If refraction is not 0, a refraction ray is calculated.
 	If the refraction is a total internal reflection,
		null is returned instead of the refraction ray.
If the refraction ray is null,
	only the reflection ray is casted.
Else
 	both rays are casted
	then added depending on their share.
*/

Light ReflectRefract(Ray* incommingRay, Point* pointOnSurface, Point* normal, Sphere* surface, vector<LightSource>* lights, vector<Sphere>* objects, int recursionDepth, int maxRecursionDepth)
{
	Light l = Light{ 0, 0, 0 };
	Ray* reflectionRay = nullptr;
	Ray* refractionRay = nullptr;

	float reflectivity = surface->GetReflectivityAtPoint(pointOnSurface);
	// We only need to cast both rays, if it isn't toal reflection or total refraction.
	if(reflectivity > 0)
		reflectionRay = CreateReflectionRay(incommingRay, pointOnSurface, normal, surface->GetIORAtPoint(pointOnSurface));
	if(reflectivity < 1)
		refractionRay = CreateRefractionRay(incommingRay, pointOnSurface, normal, surface->GetIORAtPoint(pointOnSurface));
	// If there is both reflection and refraction, we cast both rays and give each the right amount of light.
	if(reflectionRay && refractionRay)
	{
		l  = CastRay(*refractionRay, objects, lights, recursionDepth, maxRecursionDepth) * (1 - reflectivity);
		l += CastRay(*reflectionRay, objects, lights, recursionDepth, maxRecursionDepth) * reflectivity;
	}
	// If it's reflection only we only cast the reflection ray.
	else if(reflectionRay)
	{
		l = CastRay(*reflectionRay, objects, lights, recursionDepth, maxRecursionDepth) * reflectivity;
	}
	// If it's refraction only we cast only the refraction ray.
	else if(refractionRay)
	{
			l  = CastRay(*refractionRay, objects, lights, recursionDepth, maxRecursionDepth) * (1 - reflectivity);
	}
	// If there should be refraction only but it is full internal reflection, we cast a reflection ray.
	else
	{
			//l = CastRay(*reflectionRay, objects, lights, recursionDepth, maxRecursionDepth) * reflectivity;
	}

	// Cleaning up
	if(reflectionRay)
		delete reflectionRay;
	if(refractionRay)
		delete refractionRay;
}




Ray* CreateReflectionRay(Ray* incommingRay, Point* reflectionPosition, Point* surfaceNormal, const float& ior)
{
	Point reflectionDirection = incommingRay->direction - (*surfaceNormal)*2*Point::DotProduct(surfaceNormal, &(incommingRay->direction));
	Ray* r = new Ray(*reflectionPosition+*surfaceNormal*shadowBias, reflectionDirection, ior);
	return r;
}




Ray* CreateRefractionRay(Ray* incommingRay, Point* reflectionPosition, Point* surfaceNormal, const float &ior)
{
	// The projection of the incomming ray onto the surface normal.
	float cosi = clamp(-1, 1, Point::DotProduct(surfaceNormal, &incommingRay->direction));

	//float etai = incommingRay->lastIOR, etat = ior;
	Point normal = *surfaceNormal;
	// If the projection of the incomming ray on the surface normal is negative we are coming from the outside of the object.
	// In this case we invert the projection, because it needs to be positive for further calculation.
	if (cosi < 0)
	{
		cosi = -cosi;
	}
	// If we are coming from the inside of the object, we negate the normal, because we need to have it pointing roughly in the direction that the ray will go.
	else
	{
	 	//std::swap(etai, etat);
		normal = -normal;
	}
	// The relativeIOR determines how the ray will be refracted.
	// It is the ratio between the IORs of the materials of the materials of the incomming and outgoing ray.
	float relativeIOR = incommingRay->lastIOR / ior;
	// The sign of k indicates whether a total internal reflection happens.
	// The square root of k is also necessary for the calculation of the outgoing direction,
	// but we can avoid unnecessary calculation by checking k first.
	float k = 1 - relativeIOR * relativeIOR * (1 - cosi * cosi);
	// If total internal reflecion happens, we return nullptr to indicate, that no refraction ray is necessary.
	if( k < 0)
	{
		return nullptr;
	}
	// Else we create a ray to be used as refraction ray.
	else
	{
		#define OUT_DIR incommingRay->direction * relativeIOR + normal * (relativeIOR * cosi - sqrtf(k))
		return new Ray(*reflectionPosition, OUT_DIR, ior);
	}
}
