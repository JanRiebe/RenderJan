// renderjan.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include "Point.h"
#include "3DDataStructs.cpp"
#include "Face.h"
#include <iostream>

using namespace std;

// Determines whether a given ray hits a given triangle.
bool RayCast(const Point* const origin, const Point* const direction , const Face* const triangle);




int main()
{
	cout << "Welcome to Renderjan!\n\n";
	cout << "So far the system can only calculate whether a ray hits a triangle.\n\n";
	cout << "Please enter the coordinates of the triangle.\n";
	cout << "Vertex 0:\n";
	cout << "x: ";
	float x0;
	cin >> x0;
	cout << "y: ";
	float y0;
	cin >> y0;
	cout << "z: ";
	float z0;
	cin >> z0;
	cout << "\n";
	cout << "Vertex 1:\n";
	cout << "x: ";
	float x1;
	cin >> x1;
	cout << "y: ";
	float y1;
	cin >> y1;
	cout << "z: ";
	float z1;
	cin >> z1;
	cout << "\n";
	cout << "Vertex 2:\n";
	cout << "x: ";
	float x2;
	cin >> x2;
	cout << "y: ";
	float y2;
	cin >> y2;
	cout << "z: ";
	float z2;
	cin >> z2;
	cout << "\n";
	Point p0(x0, y0, z0);
	Point p1(x1, y1, z1);
	Point p2(x2, y2, z2);
	Vertex v0(&p0);
	Vertex v1(&p1);
	Vertex v2(&p2);
	Face face(&v0, &v1, &v2);
	face.calculateNAndD();

	cout << "Please enter the origin of the ray.\n";
	cout << "x: ";
	float xo;
	cin >> xo;
	cout << "y: ";
	float yo;
	cin >> yo;
	cout << "z: ";
	float zo;
	cin >> zo;
	cout << "\n";
	Point origin(xo, yo, zo);

	cout << "Please enter the direction of the ray.\n";
	cout << "x: ";
	float xd;
	cin >> xd;
	cout << "y: ";
	float yd;
	cin >> yd;
	cout << "z: ";
	float zd;
	cin >> zd;
	cout << "\n";
	Point direction(xd, yd, zd);

	if (RayCast(&origin, &direction, &face))
		cout << ":)   /+\    The ray hits the triangle!\n\n";
	else
		cout << ":(   +/\    The ray does not hit the triangle.\n\n";


	system("pause");
	return 0;
}




bool RayCast(const Point* const origin, const Point* const direction, const Face* const triangle)
{
	/*
	Note that this technique can be made faster if the triangle's normal as well as the value D from the plane equation are precomputed and stored in memory for each triangle of the scene.
	*/

	// Step 1: finding P
	
	// Calculating whether ray and plane are paralel. In that case the intersection can't be calculated.

	float dot = Point::DotProduct(&((*triangle).normal), direction);
	if (dot == 0)
		return false; // No intersection because the triangle and the ray are parallel.

	// Calculating t, the distance to the hit point.
	float t = -(Point::DotProduct(&((*triangle).normal), origin) + (*triangle).distance) / dot;
	if (t < 0)
		return false; // No intersection because the triangle is behind the camera.

	// Calculating P, the position of the hit point.
	const Point * const P = &(Point::add(origin, &(Point::mult(direction, t))));

	// Step 2: inside-outside test
	Point C; // vector perpendicular to triangle's plane

	Point* v0 = (*(*triangle).vertices[0]).position;
	Point* v1 = (*(*triangle).vertices[1]).position;
	Point* v2 = (*(*triangle).vertices[2]).position;

	// edge 0
	Point edge0 = Point::subtr(v1,v0);
	Point vp0 = Point::subtr(P,v0);
	C = Point::CrossProduct(&edge0,&vp0);
	if (Point::DotProduct(&((*triangle).normal),&C) < 0) 
		return false; // P is outside of the triangle

	// edge 1
	Point edge1 = Point::subtr(v2,v1);
	Point vp1 = Point::subtr(P,v1);
	C = Point::CrossProduct(&edge1, &vp1);
	if (Point::DotProduct(&((*triangle).normal), &C) < 0)
		return false; // P is outside of the triangle

	// edge 2
	Point edge2 = Point::subtr(v0,v2);
	Point vp2 = Point::subtr(P,v2);
	C = Point::CrossProduct(&edge2, &vp2);
	if (Point::DotProduct(&((*triangle).normal), &C) < 0)
		return false; // P is outside of the triangle

	return true; // this ray hits the triangle 
}







/*
I'm mainly following the instructions given here:
https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/ray-triangle-intersection-geometric-solution
*/