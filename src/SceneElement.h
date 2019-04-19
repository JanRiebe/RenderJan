#pragma once

#include "Point.h"

class SceneElement
{
public:
  Point position;

	float scale;

	Point color;

  SceneElement(Point pos, float scale, Point color);
};
