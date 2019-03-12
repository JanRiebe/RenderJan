#pragma once
#include "Point.h"
#include "SceneElement.h"

struct LightSource : SceneElement
{
public:
	bool isPoint;	//else directional

	LightSource(const Point pos, const Point color, const float intensity, const bool isPoint);
	virtual ~LightSource();

	/*
	// Returns the normal of the light in direction of p.
	Point GetDirection(const Point* p);

	void LightSource::GetDirectionAndIntensity(const Point* P, Point* lightDir, Point* lightIntensity);
	*/
};
