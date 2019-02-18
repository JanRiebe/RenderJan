#pragma once
#include <vector>
#include "Point.h"
#include "Vertex.h"

/// <summary>Faces are the visible elements of geometry.</summary>
class Face
{

private:
#pragma region private variables

	// The normal of the face. Caluclated by calling calculateNormal() or calculateNAndD().
	Point normal;

	// The distance of the plane going through the face, measured parallel to the normal.
	float distance;
#pragma endregion

public:
#pragma region public variables
	// Pointers to the 3 vertecies that make up the face.
	Vertex* vertices[3];

#pragma endregion


#pragma region public methods

	/// <summary>Constructor for a Face.
	/// <param name="v0">v0: The first vertex in the triangle.</param>
	/// <param name="v1">v1: The second vertex in the triangle.</param>
	/// <param name="v2">v2: The third vertex in the triangle.</param>
	/// </summary>  
	Face(Vertex* v0, Vertex* v1, Vertex* v2);

	/// <summary>Calculates and sets the normal variable for this face.</summary>  
	void calculateNormal();

	/// <summary>Calculates and sets the distance variable for this face.</summary>  
	void calculateDistance();

	/// <summary>Calculates and sets both the normal and the distance variable for this face,
	/// by calling calculateNormal() and calculateDistance().
	/// </summary>  
	void calculateNAndD();

	/// <summary>Accessor for the normal variable.
	/// <returns>returns: A constant pointer to the normal variable held by this object.</returns>  
	/// </summary>
	const Point* const getNormal() const;

	/// <summary>Accessor for the distance variable.
	/// <returns>returns: A the value of the distance variable.</returns>  
	/// </summary>
	float getDistance() const;
#pragma endregion

};

