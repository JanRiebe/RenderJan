#pragma once
#include "Point.h"

/// <summary>Vertecies are the fundamental building block of geometry.
/// Several triangles can share a vertex.
/// Several vertecies can share a point.
/// Vertecies are meant to hold additional data then just a position, like a normal ect.</summary>
class Vertex
{
public:
#pragma region public variables
	// A pointer to a point that defines the position of this vertex. Ideally the point should be part of the same geometry object as the vertex.
	Point* position;
#pragma endregion

#pragma region public methods

	/// <summary>Constructor for a Vertex.
	/// <param name="pos">pos: A pointer to a point that defines the position of this vertex.</param>
	/// </summary>
	Vertex(Point* pos);
#pragma endregion


};

