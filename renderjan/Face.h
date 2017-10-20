#pragma once
#include <vector>
#include "Point.h"
#include "Vertex.h"


class Face
{
public:
	Vertex* vertices[3];
	Point normal;
	float distance;		// The distance of the plane going through the face, measured parallel to the normal.

	Face(Vertex* v0, Vertex* v1, Vertex* v2);
	void calculateNormal();
	void calculateDistance();
	void calculateNAndD();
};

