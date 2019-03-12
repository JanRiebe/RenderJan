#pragma once

#include "Point.h"

struct SceneElement
{
  Point position;

	float scale;

	Point color;

  SceneElement(Point pos, float scale, Point color);
};
