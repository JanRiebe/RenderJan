#include "stdafx.h"
#include "Point.h"
#include <math.h>



Point::Point(float X, float Y, float Z) :
	x(X), y(Y), z(Z)
{}

// Returns a new Point with the multiplied values.
Point Point::mult(const Point* const p, float f)
{
	return Point((*p).x*f, (*p).y*f, (*p).z*f);
}

// Returns a new Point with the values of the two points added together.
Point Point::add(const Point* const p1, const Point* const p2)
{
	return Point((*p1).x + (*p2).x, (*p1).y + (*p2).y, (*p1).z + (*p2).z);
}


// Returns a new Point with the values of p2 subracted from p1.
Point Point::subtr(const Point* const p1, const Point* const p2)
{
	return Point((*p1).x - (*p2).x, (*p1).y - (*p2).y, (*p1).z - (*p2).z);
}

Point Point::CrossProduct(const Point* const v1, const Point* const v2)
{
	return Point(
		(*v1).y * (*v2).z - (*v1).z * (*v2).y,
		(*v1).z * (*v2).x - (*v1).x * (*v2).z,
		(*v1).x * (*v2).y - (*v1).y * (*v2).x);

}

float Point::DotProduct(const Point* const v1, const Point* const v2)
{
	return (*v1).x * (*v2).x + (*v1).y * (*v2).y + (*v1).z * (*v2).z;
}


void Point::Normalise(Point* const pointToBeChanged)
{
	double length = sqrt(((*pointToBeChanged).x * (*pointToBeChanged).x) + 
		((*pointToBeChanged).y * (*pointToBeChanged).y) + 
		((*pointToBeChanged).z * (*pointToBeChanged).z));
	(*pointToBeChanged).x /= length;
	(*pointToBeChanged).y /= length;
	(*pointToBeChanged).z /= length;
}