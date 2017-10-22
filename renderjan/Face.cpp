#include "stdafx.h"
#include "Face.h"
#include "Point.h"
#include <exception>
#include <iostream>


Face::Face(Vertex* v0, Vertex* v1, Vertex* v2)
{
	vertices[0] = v0;
	vertices[1] = v1;
	vertices[2] = v2;
}

void Face::calculateNormal()
{
	Point A = Point::subtr((*vertices[1]).position, (*vertices[0]).position); // edge 0
	Point B = Point::subtr((*vertices[2]).position, (*vertices[0]).position); // edge 1
	normal = Point::CrossProduct(&A, &B); // this is the triangle's normal
	try {
		Point::Normalise(&normal);
	}
	catch (std::exception& e) {
		std::cout << e.what()<<'\n';
	}
}


void Face::calculateDistance()
{
	// Calculating D, the distance from the origin to the plane, measured parallel to the planes normal.
	float D = Point::DotProduct(&normal, (*vertices[0]).position);
}

void Face::calculateNAndD()
{
	calculateNormal();
	calculateDistance();
}

const Point* const Face::getNormal() const
{
	return &normal;
}

float Face::getDistance() const
{
	return distance;
}
