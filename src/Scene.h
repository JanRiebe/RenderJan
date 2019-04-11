#pragma once

#include <vector>
#include "LightSource.h"
#include "Sphere.h"
#include "Camera.h"

using namespace std;

struct Scene
{
	vector<LightSource> lights;
  vector<Sphere> spheres;
	Camera camera;

	Scene();
};
