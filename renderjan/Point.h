#pragma once
class Point
{
public:
	float x, y, z;

	Point(float x = 0, float y = 0, float z = 0);
	static Point mult(const Point* p, float f);
	static Point add(const Point* const p1, const Point* const p2);
	static Point subtr(const Point* const p1, const Point* const p2);
	static Point CrossProduct(const Point* v1, const Point* v2);
	static float DotProduct(const Point* const v1, const Point* const v2);
	static void Normalise(Point* const pointToBeChanged);
};

