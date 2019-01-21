#pragma once
#include "Point.h"

class LightSource
{
public:
	Point position;
	Point color;
	float intensity;

	LightSource(const Point pos, const Point color, const float intensity);
	virtual ~LightSource();
};

