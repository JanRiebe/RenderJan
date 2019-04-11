#include "Point.h"
#include <math.h>
#include <exception>



Point::Point(float X, float Y, float Z) :
	x(X), y(Y), z(Z)
{}


Vec4 Point::ToVec4()
{
	return {x, y, z, 1};
}


Matrix4x4 Point::ToTranslationMatrix()
{
	return
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		x, y, z, 1
	};
}


Point Point::operator*(float f)
{
	return Point(x*f, y*f, z*f);
}


Point Point::operator/(double f)
{
	return Point(x/f, y/f, z/f);
}


Point Point::operator+(const Point p2)
{
	return Point(x + p2.x, y + p2.y, z + p2.z);
}


Point Point::operator-(const Point p2)
{
	return Point(x - p2.x, y - p2.y, z - p2.z);
}


Point Point::operator-()
{
	return Point(-x, -y, -z);
}


Point* Point::CrossProduct(const Point* v1, const Point* v2)
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
		return;//throw std::exception("Can't normalise a Point that has no length.");

	double length = CalculateLength(pointToBeChanged);

	pointToBeChanged->x /= length;
	pointToBeChanged->y /= length;
	pointToBeChanged->z /= length;
}

Point Point::GetUnitVector()
{
	double len = CalculateLength(this);
	return{ this->x / (float)len, this->y / (float)len,this->z / (float)len };
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
