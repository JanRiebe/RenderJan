#include "stdafx.h"
#include <vector>
#include "Point.h"
#include "Face.h"

using namespace std;


struct Object
{
	vector<Point> points;
	vector<Vertex> vertecies;
	vector<Face> faces;
};