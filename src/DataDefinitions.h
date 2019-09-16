#pragma once

#include "Point.h"
#include "LightSource.h"

const double PI = 3.1415926535897932384626433832795028841971693993751058209749445923078164062;

const float shadowBias = 1e-4;

struct Light
{
	float r, g, b;
	Light(float r, float g, float b):r(r),g(g),b(b){};
	void operator+=(const Light l2)
	{
		r += l2.r;
		g += l2.g;
		b += l2.b;
	}
	Light operator*(float f)
	{
		return Light(r*f, g*f, b*f);
	}
	Light operator+(Light other)
	{
		return Light(r+other.r, g+other.g, b+other.b);
	}
};


struct Ray
{
	Point origin;
	Point direction;
	float relativeIOR;
	Ray(Point origin, Point direction, float relativeIOR): origin(origin), direction(direction), relativeIOR(relativeIOR){};
};

/*
struct Object
{
	vector<Point> points;
	vector<Vertex> vertecies;
	vector<Face> faces;
};
*/
