#include "LightSource.h"
#include "MathFunctions.h"

LightSource::LightSource(const Point pos, const Point color, const float intensity, const bool isPoint): SceneElement(pos, pos, intensity, color), isPoint(isPoint)
{
}

LightSource::~LightSource()
{
}

/*
Point LightSource::GetDirection(const Point* p)
{
	return (position - *p).Normalise();
}




void LightSource::GetDirectionAndIntensity(const Point* P, Point* lightDir, Point* lightIntensity)
{
	*lightDir = position - *P; // compute light direction
	float r2 = lightDir->CalculateLength();
	*lightDir = lightDir->Normalise();
	*lightIntensity = color * InverseSquareFalloff(intensity, r2);
}
*/
