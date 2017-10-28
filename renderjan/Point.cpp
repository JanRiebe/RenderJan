#include "stdafx.h"
#include "Point.h"
#include <math.h>
#include <exception>



Point::Point(float X, float Y, float Z) :
	x(X), y(Y), z(Z)
{}


Point* Point::mult(const Point* const p, float f)
{
	return new Point((*p).x*f, (*p).y*f, (*p).z*f);
}


Point* Point::add(const Point* const p1, const Point* const p2)
{
	return new Point((*p1).x + (*p2).x, (*p1).y + (*p2).y, (*p1).z + (*p2).z);
}


Point* Point::subtr(const Point* const p1, const Point* const p2)
{
	return new Point((*p1).x - (*p2).x, (*p1).y - (*p2).y, (*p1).z - (*p2).z);
}

Point* Point::CrossProduct(const Point* const v1, const Point* const v2)
{
	return new Point(
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
	if ((*pointToBeChanged).x == 0 && (*pointToBeChanged).y == 0 && (*pointToBeChanged).z == 0)
		throw std::exception("Can't normalise a Point that has no length.");

	double length = sqrt(((*pointToBeChanged).x * (*pointToBeChanged).x) + 
		((*pointToBeChanged).y * (*pointToBeChanged).y) + 
		((*pointToBeChanged).z * (*pointToBeChanged).z));
	(*pointToBeChanged).x /= length;
	(*pointToBeChanged).y /= length;
	(*pointToBeChanged).z /= length;
}