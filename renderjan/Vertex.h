#pragma once
#include "Point.h"

class Vertex
{
public:
	Point* position;
	Point* normal;

	Vertex(Point* pos);
	~Vertex();
};

