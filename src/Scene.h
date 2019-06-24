#pragma once

#include <vector>
#include <map>
#include "LightSource.h"
#include "Sphere.h"
#include "Camera.h"
#include "Material.h"

using namespace std;

struct Scene
{
	vector<LightSource> lights;
  vector<Sphere> spheres;
	Camera camera;
	map<string, Material> materials;

	Scene();
};
