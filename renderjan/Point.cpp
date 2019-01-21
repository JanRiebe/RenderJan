#include "stdafx.h"
#include "Point.h"
#include <math.h>
#include <exception>



Point::Point(float X, float Y, float Z) :
	x(X), y(Y), z(Z)
{}


Point Point::operator*(float f)
{
	return Point(x*f, y*f, z*f);
}

Point Point::operator/(double f)
{
	return Point(x/f, y/f, z/f);
}


Point Point::operator+(const Point const p2)
{
	return Point(x + p2.x, y + p2.y, z + p2.z);
}


Point Point::operator-(const Point const p2)
{
	return Point(x - p2.x, y - p2.y, z - p2.z);
}


Point Point::operator-()
{
	return Point(-x, -y, -z);
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

	double length = CalculateLength(pointToBeChanged);
	
	pointToBeChanged->x /= length;
	pointToBeChanged->y /= length;
	pointToBeChanged->z /= length;
}

Point Point::Normalise()
{
	return *this / CalculateLength(this);
}

double Point::CalculateLength(Point* const p)
{
	return sqrt((p->x * p->x) +
				(p->y * p->y) +
				(p->z * p->z));
}


double Point::CalculateLength()
{
	return sqrt(x*x + y*y + z*z);
}