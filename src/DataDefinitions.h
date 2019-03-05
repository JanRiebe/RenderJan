#pragma once

#include "Point.h"
#include "LightSource.h"

const double PI = 3.1415926535897932384626433832795028841971693993751058209749445923078164062;

const float shadowBias = 1e-4;

struct Light
{
	float r, g, b;
};


struct Ray
{
	Point origin;
	Point direction;
};

/*
struct Object
{
	vector<Point> points;
	vector<Vertex> vertecies;
	vector<Face> faces;
};
*/
