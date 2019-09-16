#pragma once

#include "Point.h"

class SceneElement
{
public:
  Point position;

	float scale;

	Point color;

  Point rotation;

  SceneElement(Point pos, Point rot, float scale, Point color);
};
