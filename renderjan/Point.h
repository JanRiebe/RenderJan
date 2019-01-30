#pragma once

/// <summary>A point acts like a mathematical 3D vector. </summary>
class Point
{
public:
#pragma region public variables
	// The coordinates of the point.
	float x, y, z;
#pragma endregion

#pragma region public methods
	/// <summary>Constructor for a Point.
	/// <param name="x">x: The x coordinates of the point. Default value 0.</param>
	/// <param name="y">y: The y coordinates of the point. Default value 0.</param>
	/// <param name="z">y: The z coordinates of the point. Default value 0.</param>
	/// </summary>  
	Point(float x = 0, float y = 0, float z = 0);

	/// <summary>Multiplies x, y, z of a point by a factor.
	/// <param name="p">p: A pointer to the Point which values should be used in the multiplication.</param>
	/// <param name="f">f: The factor by which the values of p should be multiplied.</param>
	/// <returns>returns: A pointer to a new Point object on the heap with the multiplied values.</returns>  
	/// </summary>  
	Point operator*(float f);

	Point operator/(double f);

	/// <summary>Adds x, y, z of two points.
	/// <param name="p1">p1: A pointer to the first Point which values should be used in the addition.</param>
	/// <param name="p2">p2: A pointer to the second Point which values should be used in the addition.</param>
	/// <returns>returns: A pointer to a new Point object on the heap with the added values.</returns>  
	/// </summary>  
	Point operator+(const Point const p2);

	/// <summary>Subtracts x, y, z of one point from those of another.
	/// <param name="p1">p1: A pointer to the Point which holds the initial values.</param>
	/// <param name="p2">p2: A pointer to the Point which holds the values that will be subtracted from p1.</param>
	/// <returns>returns: A pointer to a new Point object on the heap with the subtracted values, p1-p2.</returns>  
	/// </summary>  
	Point operator-(const Point const p2);
	Point operator-();

	/// <summary>Calculates the cross product of two points.
	/// <param name="p1">p1: A pointer to a Point.</param>
	/// <param name="p2">p2: A pointer to a Point.</param>
	/// <returns>returns: A pointer to a new Point object on the heap that represents the cross product of p1 and p2.</returns>  
	/// </summary>
	static Point* CrossProduct(const Point* v1, const Point* v2);

	/// <summary>Calculates the dot product of two points.
	/// <param name="p1">p1: A pointer to a Point.</param>
	/// <param name="p2">p2: A pointer to a Point.</param>
	/// <returns>returns: A float value that represents the dot product of p1 and p2.</returns>  
	/// </summary>
	static float DotProduct(const Point* const v1, const Point* const v2);

	/// <summary>Normalises a point. Scales it's x, y, z values so that the direct distance from 0, 0, 0 is 1.
	/// <param name="pointToBeChanged">pointToBeChanged: A pointer to a Point. The velues of the point will be changed. Should not be a point which x, y, z values are all 0.</param>
	/// <exception>Throws exception if x, y, z of pointToBeChanged are all 0.</exception>
	/// </summary>
	static void Normalise(Point* const pointToBeChanged);
	Point GetUnitVector();
#pragma endregion

	static double CalculateLength(Point* const p);
	double CalculateLength();

	
};

