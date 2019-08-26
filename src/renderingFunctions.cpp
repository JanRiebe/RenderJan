#include "renderingFunctions.h"
#include "Sphere.h"
#include "RenderViewer.h"
#include "MathFunctions.h"
#include "Scene.h"
#include "Image.h"
#include <iostream>
#include <stack>
float tmp=0;
/*
Renders the scene and returns a pointer to the rendered image.
The caller gets ownership of the returned pointer.
*/
Image* RenderScene(Scene* scene, int width, int height, int maxReflectionDepth)
{
	width = scene->camera.imageWidth;
	height = scene->camera.imageHeight;

	cout << "width "<<width<< " height "<<height<<endl;

	Image* img = new Image();
	Pixel* pix = new Pixel[width*height];

	for (int y = height - 1; y >= 0; --y)
	{
		for (int x = 0; x < width; x++)
		{
			Pixel* p = &pix[y*width + x];
			Ray ray = scene->camera.PixelToRay(x, y);
			Light l = CastRay(ray, &(scene->spheres), &(scene->lights), 0, maxReflectionDepth);// Ray(/*origin*/ Point( x - width / 2, y - height / 2,-100), /*direction*/ Point(0,0,1), /*initial IOR*/1.0), &(scene->spheres), &(scene->lights), 0, maxReflectionDepth);
			p->r = l.r;
			p->g = l.g;
			p->b = l.b;
			p->a = 1;
		}
	}
	tmp+=0.1;
	//TODO not sure what this meant to print: cout << (1000000*sin(tmp)) <<endl;

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
	{
		// Returning background colors
		float checkerMult = 1.0f;
		if (((int)(r.direction.x*100) % 10) > 5 )
			checkerMult = 0.5f;
		return{ r.direction.x/2 +0.5f * checkerMult, r.direction.y /2 +0.5f * checkerMult, r.direction.z /2 +0.5f *checkerMult};
	}
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
		if (CastShadowRay(Ray( p + normal*shadowBias,  pToLight, viewRay.lastIOR, viewRay.relativeIOR ), objects))
		{
			// If the shadow ray didn't hit any object, the light is visible.
			// Lamberian
			Point viewingDirection = pToLight.GetUnitVector();
			float facingRatio = std::max(0.0f, Point::DotProduct(&viewingDirection, &normal));
			float intensityAfterFalloff = lightSource->scale;
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
		l+= rf;
	}
	else
		l = {1,0,1};

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
	Ray* reflectionRay = nullptr;
	Ray* refractionRay = nullptr;


	float reflectivity = surface->GetReflectivityAtPoint(pointOnSurface);
	float translucency = surface->GetTranslucencyAtPoint(pointOnSurface);

	// We only need to cast both rays, if it isn't toal reflection or total refraction.
	if(reflectivity > 0)
		reflectionRay = CreateReflectionRay(incommingRay, pointOnSurface, normal);//, iorOfThisSide);

	// Calculating refraction if the object is not totally reflecting.
	if(translucency > 0)
	{
		float iorOfThisSide = incommingRay->lastIOR;
		float iorOfSurface = surface->GetIORAtPoint(pointOnSurface);
		float relativeIOR;
		float nextRaysIOR;

		// Projection of the incomming ray onto the surface normal.
		float cosi = clamp(-1, 1, Point::DotProduct(normal, &incommingRay->direction));
		// Entering
		if (cosi < 0)
		{
			cosi = -cosi;
			relativeIOR = iorOfThisSide / iorOfSurface;
			nextRaysIOR = iorOfSurface;
		}
		// Leaving
		else
		{
			normal->flip();

			relativeIOR = iorOfSurface / 1; //nextRaysIOR;
			nextRaysIOR = 1;// iorOfSurface * incommingRay->relativeIOR; //1;
		}

		// Changing the reflectivity depending on fresnel.
		translucency *=(1 - Fresnel(incommingRay, pointOnSurface, normal, iorOfThisSide, relativeIOR, cosi, iorOfSurface)*surface->GetFresnelAtPoint(pointOnSurface));
		//reflectivity = 0.0f;	//DEBUG

		// Creating the refraction ray.
		refractionRay = CreateRefractionRay(incommingRay, pointOnSurface, normal, relativeIOR, cosi, nextRaysIOR);
	}

	// Casting rays that have been created.
	Light reflectedLight = {0,0,0};
	Light refractedLight = {0,0,0};
	if(reflectionRay)
	{
		reflectedLight = CastRay(*reflectionRay, objects, lights, recursionDepth, maxRecursionDepth);
	}
	if(refractionRay)
	{
		refractedLight  = CastRay(*refractionRay, objects, lights, recursionDepth, maxRecursionDepth);
	}
	Light l = refractedLight * translucency; // reflectedLight * reflectivity * (1-translucency) + refractedLight * translucency;

	// Cleaning up
	if(reflectionRay)
		delete reflectionRay;
	if(refractionRay)
		delete refractionRay;

	// Returning the light that was gathered at this point.
	return l;
}




Ray* CreateReflectionRay(Ray* incommingRay, Point* reflectionPosition, Point* surfaceNormal)//, const float& ior)
{
	Point reflectionDirection = incommingRay->direction - (*surfaceNormal)*2*Point::DotProduct(surfaceNormal, &(incommingRay->direction));
	Ray* r = new Ray(*reflectionPosition+*surfaceNormal*shadowBias, reflectionDirection, incommingRay->lastIOR, incommingRay->relativeIOR);
	return r;
}


//std::stack<float> iorStack;
/*
How the ior stack is to be used:
The current ray carries the ior of the material we are in.
When coming from the outside of an object this value is put on the stack,
because it is the ior of the surrounding object.
When leaving an object, it is popped off the stack and used as the ior
of the other side.
*/
/*
Point RefractDirection(const Point &incomming, const Point &surfaceNormal, const float &ior)
{
    float cosi = clamp(-1, 1, Point::DotProduct(surfaceNormal, incomming));
    float etai = 1, etat = ior;
    Point n = surfaceNormal;
    if (cosi < 0) { cosi = -cosi; } else { std::swap(etai, etat); n= -surfaceNormal; }	//TODO it might be that the swapping of the surface normal is missing
    float eta = etai / etat;
    float k = 1 - eta * eta * (1 - cosi * cosi);
    return k < 0 ? 0 : eta * I + (eta * cosi - sqrtf(k)) * n;
}
*/
Ray* CreateRefractionRay(Ray* incommingRay, Point* reflectionPosition, Point* surfaceNormal, float relativeIOR, float cosi, float nextRaysIOR)
{
	// The sign of k indicates whether a total internal reflection happens.
	// The square root of k is also necessary for the calculation of the outgoing direction,
	// but we can avoid unnecessary calculation by checking k first.
	float k = 1 - relativeIOR * relativeIOR * (1 - cosi * cosi);
	//std::cout << "k = "<<k<<std::endl;
	// If total internal reflecion happens, we return nullptr to indicate, that no refraction ray is necessary.
	if( k < 0)
	{
		return nullptr;
	}
	// Else we create a ray to be used as refraction ray.
	else
	{
		#define OUT_DIR incommingRay->direction * relativeIOR + (*surfaceNormal) * (relativeIOR * cosi - sqrtf(k))
		return new Ray(*reflectionPosition - (*surfaceNormal)*shadowBias, OUT_DIR, nextRaysIOR, relativeIOR);
	}
}


float Fresnel(Ray* incommingRay, Point* reflectionPosition, Point* surfaceNormal, const float &iorOfThisSide, const float& relativeIOR, const float& cosi, const float&iorOfOtherSide)
{
	float etai = incommingRay->lastIOR, etat = iorOfOtherSide;

	//float iorOfOtherSide = ior;
	/*
	if (cosi < 0)
	{
		//std::swap(etai, etat);
			cosi = -cosi;
	}
	else
	{
		if(!iorStack.empty())
		{
			iorOfOtherSide = iorStack.top();
		}
	}
	float relativeIOR = incommingRay->lastIOR / iorOfOtherSide;
	*/
	// Compute sini using Snell's law
	//float sint = 1 - relativeIOR * relativeIOR * (1 - cosi * cosi);
	float sint = relativeIOR * sqrtf(std::max(0.f, 1 - cosi * cosi));
	//float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
	// Total internal reflection
	//std::cout << "sint = "<<sint<<" -sint** "<<(-sint*sint)<<std::endl;
	if (sint < 0) {
		return 1;
	}
	else {
		float cost = sqrtf(std::max(0.f, 1 - sint * sint));
		//cosi = fabsf(cosi);		//conputes absolute value. unnecessary because already done above
		float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
		float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
		return (Rs * Rs + Rp * Rp) / 2;
	}
	// As a consequence of the conservation of energy, transmittance is given by:
	// kt = 1 - kr;
}
/*
void Common(Ray* incommingRay, Point* surfaceNormal, float &iorOfThisSide, float& relativeIOR, float& cosi, float&iorOfOtherSide)
{
	cosi = clamp(-1, 1, Point::DotProduct(surfaceNormal, &incommingRay->direction));

	// The ior is a quality of the material on the other side of the surface.
	// If we enter an object, it is the quality of the material of the object.
	// If we leave the object, it is the quality of the material of its surroundings.
	iorOfThisSide = incommingRay->lastIOR;

	// If the projection of the incomming ray on the surface normal is negative we are coming from the outside of the object.
	// In this case we invert the projection, because it needs to be positive for further calculation.
	// We also put the previous ior on the iorStack, so that we can revert to it when we are leaving the object.
	if (cosi < 0)
	{
		cosi = -cosi;
		//iorStack.push(incommingRay->lastIOR);
	}

	else
	{
		iorOfOtherSide = surface
		/*
		if(!iorStack.empty())
		{
			iorOfOtherSide = iorStack.top();
			//iorStack.pop();
		}
		*//*
	}

	relativeIOR = iorOfThisSide / iorOfOtherSide;

/*
		this method should return:
		relativeIOR
		cosi
		incommingRay->direction
		iorOfOtherSide
		incommingRay->lastIOR
*/
//}


/*
Working implementation from website. Recalculates a lot what I do in CreateRefractionRay.
void fresnel(Ray* incommingRay, Point* reflectionPosition, Point* surfaceNormal, const float &ior, float &kr)
{
	float cosi = clamp(-1, 1, Point::DotProduct(surfaceNormal, &incommingRay->direction));
	float etai = 1, etat = ior;
	if (cosi > 0) { std::swap(etai, etat); }
	// Compute sini using Snell's law
	float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
	// Total internal reflection
	if (sint >= 1) {
		kr = 1;
	}
	else {
		float cost = sqrtf(std::max(0.f, 1 - sint * sint));
		cosi = fabsf(cosi);
		float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
		float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
		kr = (Rs * Rs + Rp * Rp) / 2;
	}
	// As a consequence of the conservation of energy, transmittance is given by:
	// kt = 1 - kr;
}
*/
