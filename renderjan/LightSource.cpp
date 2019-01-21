#include "stdafx.h"
#include "LightSource.h"

LightSource::LightSource(const Point pos, const Point color, const float intensity): position(pos), color(color), intensity(intensity)
{
}

LightSource::~LightSource()
{
}
