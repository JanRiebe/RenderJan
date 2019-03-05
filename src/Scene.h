#pragma once

#include <vector>
#include "LightSource.h"
#include "Sphere.h"

using namespace std;

struct Scene
{
	vector<LightSource> lights;
  vector<Sphere> spheres;
};
