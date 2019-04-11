#include "MathFunctions.h"

#include <vector>
#include <cmath>
#include "DataDefinitions.h"

//From: https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1)
{
	float discr = b * b - 4 * a * c;
	if (discr < 0) return false;
	else if (discr == 0) x0 = x1 = -0.5 * b / a;
	else {
		float q = (b > 0) ?
			-0.5 * (b + sqrt(discr)) :
			-0.5 * (b - sqrt(discr));
		x0 = q / a;
		x1 = c / q;
	}
	if (x0 > x1) std::swap(x0, x1);

	return true;
}


double InverseSquareFalloff(double intensity, double distance)
{
	return intensity / (4 * PI * distance*distance);
}



float clamp(float floor, float ceil, float in)
{
	if(in<floor)
		return floor;
	else if (in>ceil)
		return ceil;
	return in;
}



Matrix4x4 MatMult(Matrix4x4 a, Matrix4x4 b)
{
	const unsigned int size = 4;
  Matrix4x4 ret;
  for(int r=0; r<size; r++)
  {
    for(int c=0; c<size; c++)
    {
      ret[c + r*size] = 0;
      for(int i=0;i<size;i++)
      {
          ret[c + r*size] += a[i+r*size]*b[c+i*size];
      }
    }
  }
  return ret;
}

Matrix4x4 HomogMat(Matrix4x4 m)
{
	const unsigned int size = 4;

	Matrix4x4 ret;

	// Avoiding devision by 0 error.
	if(m[size-1] == 0)
		return ret;

	for(int i=0; i<size*size; i++)
  {
    ret[i] = m[i] / m[size-1];
  }
  return ret;
}



Vec4 VecMatMult(Vec4 vec, Matrix4x4 mat)
{
	const unsigned int size = 4;
	Vec4 ret;
  for(int c=0; c<size; c++)
  {
    ret[c] = 0;
    for(int i=0;i<size;i++)
    {
        ret[c] += vec[i]*mat[c+i*size];
    }
  }
  return ret;
}



Vec4 HomogVec(Vec4 vec)
{
	const unsigned int size = 4;
	// Avoiding devision by 0 error.
	if(vec[size-1] == 0)
		return vec;

  for(int c=0; c<size; c++)
  {
    vec[c] /= vec[size-1];
  }
  return vec;
}


Matrix4x4 GetRotationMatrix(float a, float b, float c)
{
	float sinA = sin(a);
	float sinB = sin(b);
	float sinC = sin(c);
	float cosA = cos(a);
	float cosB = cos(b);
	float cosC = cos(c);

	Matrix4x4 ret =
	{
			cosA*cosB, 									sinA*cosB, 									-sinB,				0,
			cosA*sinB*sinC-sinA*cosC,		sinA*sinB*sinC+cosA*cosC,		cosB*sinC, 		0,
			cosA*sinB*cosC+sinA*sinC,		sinA*sinB*cosC-cosA*sinC,		cosB*cosC,		0,
			0,													0,													0,						1
	};
	return ret;
}
