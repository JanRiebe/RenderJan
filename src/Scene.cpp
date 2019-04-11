#include "Scene.h"
#include "DataDefinitions.h"

Scene::Scene()
{
  camera = Camera(Point(0,0,0), Point(0,0,0), PI/2, 300, 300);
}
