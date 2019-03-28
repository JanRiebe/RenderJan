#include "MathFunctions.h"

#include <vector>
#include <cmath>
#include "DataDefinitions.h"

//From: https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1)
{
	float discr = b * b - 4 * a * c;
	if (discr < 0) return false;
	else if (discr == 0) x0 = x1 = -0.5 * b / a;
	else {
		float q = (b > 0) ?
			-0.5 * (b + sqrt(discr)) :
			-0.5 * (b - sqrt(discr));
		x0 = q / a;
		x1 = c / q;
	}
	if (x0 > x1) std::swap(x0, x1);

	return true;
}


double InverseSquareFalloff(double intensity, double distance)
{
	return intensity / (4 * PI * distance*distance);
}

float clamp(float floor, float ceil, float in)
{
	if(in<floor)
		return floor;
	else if (in>ceil)
		return ceil;
	return in;
}
