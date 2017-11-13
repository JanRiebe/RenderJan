// renderjan.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include "Point.h"
#include "3DDataStructs.cpp"
#include "Face.h"
#include <iostream>
#include "BMPWriter.h"

using namespace std;



/// <summary>Determines whether a given ray hits a given Face.
/// <param name="origin">origin: A pointer to the Point that defines where the ray starts.</param>
/// <param name="direction">direction: A pointer to the Point that defines the direction of the ray.</param>
/// <param name="triangle">
/// triangle: A pointer to the Face that is checked for the collision.
/// Important: If the triangle has changed since the last cast,
/// the variables "normal" and "distance" should be updated before the raycast,
/// by calling theFaceObject.calculateNAndD().
/// </param>
/// <returns>returns: true = the ray hits the triangle, false = the ray doesn't hit the triangle.</returns>  
/// </summary>  
float RayCast(const Point* const origin, const Point* const direction , const Face* const triangle);

void RenderBoolBMPOutput(const Face* const triangle, const int resolutionX, const int resolutionY, const char* filename);

void RenderFloatBMPOutput(const Face* const triangle, const int resolutionX, const int resolutionY, const char* filename);

void RenderIntTextOutput(const std::vector<int> * const values, const int WIDTH, const int HEIGHT);

int main()
{
	
	cout << "Welcome to Renderjan!\n\n";
	cout << "So far the system can only calculate whether a ray hits a triangle.\n\n";
	char input;
	/*
	cout << "Do you want to continue? y/n\n";
	cin >> input;

	while (input != 'n')
	{
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

		int resolutionX, resolutionY;

		cout << "\nPlease enter x resolution: ";
		cin >> resolutionX;
		cout << "\nPlease enter y resolution: ";
		cin >> resolutionY;

		cout << endl;

		string filename = "C:\\Users\\Jan\\Desktop\\renderjan_test\\test.bmp";
		
		RenderFloatBMPOutput(&face, resolutionX, resolutionY, filename.c_str());
		

	}
		cout << "Do you want to continue? y/n\n";
		cin >> input;
		
		*/
	/*
	string filename = "C:\\Users\\Jan\\Desktop\\renderjan_test\\test.bmp";
	int width = 100;
	int height = 100;
	
	vector<bool>* values = new vector<bool>();
	for (int w = 0; w < width; w++)
	{
		for (int h = 0; h < height; h++)
		{
			if (w % 2 == 0)
				values->push_back(true);		//!! having width instead of w, might be important in other places too
			else
				values->push_back(false);
		}
	}
	BMPWriter::WriteBoolMap(values,width,height,filename.c_str());
	//RenderIntTextOutput(values, width, height);
	*/


	Point p0(1.0f, 30.0f, 0.0f);
	Point p1(1.3f, -30.0f, 30.0f);
	Point p2(1.5f, -30.0f, -30.0f);
	Vertex v0(&p0);
	Vertex v1(&p1);
	Vertex v2(&p2);
	Face face(&v0, &v1, &v2);
	face.calculateNAndD();
	int resolutionX = 100;
	int resolutionY = 100;
	string filename = "C:\\Users\\Jan\\Desktop\\renderjan_test\\test.bmp";

	RenderFloatBMPOutput(&face, resolutionX, resolutionY, filename.c_str());



	system("pause");

	return 0;
}




float RayCast(const Point* const origin, const Point* const direction, const Face* const triangle)
{
	/*
	The collision testing has to main steps:
		1: Finding the point at which the ray collides with the edgeless plane defined by the triangle.
		2: Determining whether this point lies within the triangle.
	
	Several conditions will show us allready during step 1 that the ray doesn't hit.
	In that case step 2 doesn't have to be executed.
		If the triangle and the ray are parallel.
		If the distance at which a collision occurs is negative.
			This occurs since the ray is a linear equasion and as such goes forwards and backwards at the same time.
	*/

	// Step 1: finding the position of the hit point, posHitPoint
	
	// Calculating whether ray and plane are paralel. In that case the intersection can't be calculated.
	float dot = Point::DotProduct(triangle->getNormal(), direction);
	if (dot == 0)
		return false; // No intersection because the triangle and the ray are parallel.

	// Calculating t, the distance to the hit point.
	float t = -(Point::DotProduct(triangle->getNormal(), origin) + triangle->getDistance()) / dot;
	if (t < 0)
		return false; // No intersection because the triangle is behind the camera.

	// Calculating P, the position of the hit point.
	const Point * const posHitPoint = Point::add(origin, Point::mult(direction, t));


	// Step 2: inside-outside test
	Point* C; // vector perpendicular to triangle's plane

	Point* v0 = (*(*triangle).vertices[0]).position;
	Point* v1 = (*(*triangle).vertices[1]).position;
	Point* v2 = (*(*triangle).vertices[2]).position;

	bool rayHitsTriangle = false;

	// edge 0
	Point* edge0 = Point::subtr(v1,v0);
	Point* vp0 = Point::subtr(posHitPoint, v0);
	C = Point::CrossProduct(edge0,vp0);

	delete edge0;
	edge0 = nullptr;
	delete vp0;
	vp0 = nullptr;
	
	if (!(Point::DotProduct(triangle->getNormal(), C) < 0)) {			// The hit point is on the correct side of the edge.


		// edge 1
		Point* edge1 = Point::subtr(v2, v1);
		Point* vp1 = Point::subtr(posHitPoint, v1);
		C = Point::CrossProduct(edge1, vp1);

		delete edge1;
		edge1 = nullptr;
		delete vp1;
		vp1 = nullptr;

		if (!(Point::DotProduct(triangle->getNormal(), C) < 0)) {		// The hit point is on the correct side of the edge.

			// edge 2
			Point* edge2 = Point::subtr(v0, v2);
			Point* vp2 = Point::subtr(posHitPoint, v2);
			C = Point::CrossProduct(edge2, vp2);

			delete edge2;
			edge2 = nullptr;
			delete vp2;
			vp2 = nullptr;

			if (!(Point::DotProduct(triangle->getNormal(), C) < 0))		// The hit point is on the correct side of the edge.
				rayHitsTriangle = true;
		}
	}

	if (rayHitsTriangle)
		cout << "Ray hits at distance " << t << endl;

	delete C;
	delete posHitPoint;
	if (rayHitsTriangle)
		return t;
	else
		return 0;
}












void RenderBoolBMPOutput(const Face* const triangle, const int resolutionX, const int resolutionY, const char* filename)
{
	vector<bool> values(resolutionX*resolutionY);
	
	for (int y = resolutionY-1; y >=0 ; --y)
	{
		for (int x = 0; x < resolutionX; x++)
		{
			bool pixel = RayCast(new Point(0, y - resolutionY / 2, x - resolutionX / 2), new Point(1, 0, 0), triangle);
			//values.push_back(pixel);
			values[x + resolutionX*y] = pixel;
			//if (pixel)
			//	cout << pixel <<" at " << y << " " << x << endl;
		}
	}


	BMPWriter::WriteBoolMap(&values, resolutionX, resolutionY, filename);

	cout << "\nRender complete\n";
}


void RenderFloatBMPOutput(const Face* const triangle, const int resolutionX, const int resolutionY, const char* filename)
{
	vector<float> values(resolutionX*resolutionY);

	for (int y = resolutionY - 1; y >= 0; --y)
	{
		for (int x = 0; x < resolutionX; x++)
		{
			float pixel = RayCast(new Point(0, y - resolutionY / 2, x - resolutionX / 2), new Point(1, 0, 0), triangle);
			//values.push_back(pixel);
			values[x + resolutionX*y] = pixel;
			//if (pixel)
				//cout << pixel <<" at " << y << " " << x << endl;
		}
	}


	BMPWriter::WriteFloatMap(&values, resolutionX, resolutionY, filename);

	cout << "\nRender complete\n";
}





/*
I'm mainly following the instructions given here:
https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/ray-triangle-intersection-geometric-solution

Also reading:
https://www.ics.uci.edu/~gopi/CS211B/RayTracing%20tutorial.pdf
*/




void RenderIntTextOutput(const std::vector<int> * const values, const int WIDTH, const int HEIGHT)
{
	for (int x = 0; x < WIDTH; ++x)
	{
		cout << "-";
	}
	cout << endl;

	for (int y = 0; y < HEIGHT; ++y)
	{
		cout << "|";
		for (int x = 0; x < WIDTH; x++)
		{
			//if ((*values)[x*y])
				cout << (*values)[x+WIDTH*y]<<" ";
			//else
				//cout << " ";
		}
		cout << "|\n";
	}


	for (int x = 0; x < WIDTH; ++x)
	{
		cout << "-";
	}
	cout << endl;
}
