#pragma once
#include "Point.h"

class LightSource
{
public:
	Point position;
	Point color;
	float intensity;
	bool isPoint;	//else directional

	LightSource(const Point pos, const Point color, const float intensity, const bool isPoint);
	virtual ~LightSource();

	/*
	// Returns the normal of the light in direction of p.
	Point GetDirection(const Point* p);

	void LightSource::GetDirectionAndIntensity(const Point* P, Point* lightDir, Point* lightIntensity);
	*/
};

