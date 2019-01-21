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