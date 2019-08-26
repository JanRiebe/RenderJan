#pragma once

#include <vector>
#include "DataDefinitions.h"
#include "LightSource.h"
#include "Sphere.h"

using namespace std;

class RenderViewer;
class Scene;
class Image;

void RenderPixelRenderViewer(vector<Sphere>* spheres, vector<LightSource>* lights, const int resolutionX, const int resolutionY, RenderViewer* renderViewer);

Light CastRay(Ray r, vector<Sphere>* objects, vector<LightSource>* lights, int recutsionDepth, int maxRecursionDepth);

// Casts a ray and returns whether it did not hit an object.
// Returns true if no object was hit and false, if an object was hit.
bool CastShadowRay(Ray r, vector<Sphere>* objects);


Light CalculateOutgoingLightFromPointAtSurface(Sphere* object, Point p, Ray viewRay, vector<LightSource>* lights, vector<Sphere>* objects, int recursionDepth, int maxRecursionDepth);

Light ReflectRefract(Ray* incommingRay, Point* pointOnSurface, Point* normal, Sphere* surface, vector<LightSource>* lights, vector<Sphere>* objects, int recursionDepth, int maxRecursionDepth);
Ray* CreateRefractionRay(Ray* incommingRay, Point* reflectionPosition, Point* surfaceNormal, float relativeIOR, float cosi, float nextRaysIOR);
Ray* CreateReflectionRay(Ray* incommingRay, Point* reflectionPosition, Point* surfaceNormal);//, const float& ior);

Image* RenderScene(Scene* scene, int width, int height, int reflectionDepth);

float Fresnel(Ray* incommingRay, Point* reflectionPosition, Point* surfaceNormal, const float &ior, const float& relativeIOR, const float& cosi, const float&iorOfOtherSide);

//void Common(Ray* incommingRay, Point* surfaceNormal, float &ior, float& relativeIOR, float& cosi);//, float&iorOfOtherSide);
