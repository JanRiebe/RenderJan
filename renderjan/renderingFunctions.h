#pragma once

#include <vector>
#include "DataDefinitions.h"

using namespace std;

class Sphere;
class LightSource;
class RenderViewer;

void RenderTestScene();

void RenderPixelRenderViewer(vector<Sphere>* spheres, vector<LightSource>* lights, const int resolutionX, const int resolutionY, RenderViewer* renderViewer);

Light CastRay(Ray r, vector<Sphere>* objects, vector<LightSource>* lights);

// Casts a ray and returns whether it did not hit an object.
// Returns true if no object was hit and false, if an object was hit.
bool CastShadowRay(Ray r, vector<Sphere>* objects);


Light CalculateOutgoingLightFromPointAtSurface(Sphere* object, Point p, Ray viewRay, vector<LightSource>* lights, vector<Sphere>* objects);