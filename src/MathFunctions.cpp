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



float* MatMult(float a[16], float b[16])
{
	const unsigned int size = 4;
  static float ret[size*size];
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

float* HomogMat(float m[16])
{
	const unsigned int size = 4;

	static float ret[size] = {0,0,0,0};

	// Avoiding devision by 0 error.
	if(m[size-1] == 0)
		return ret;

	for(int i=0; i<size*size; i++)
  {
    ret[i] = m[i] / m[size-1];
  }
  return ret;
}



float* VecMatMult(float vec[4], float mat[16])
{
	const unsigned int size = 4;
	static float ret[size];
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



float* HomogVec(float vec[4])
{
	const unsigned int size = 4;

	static float ret[size] = {0,0,0,0};

	// Avoiding devision by 0 error.
	if(vec[size-1] == 0)
		return ret;

  for(int c=0; c<size; c++)
  {
    ret[c] = vec[c] / vec[size-1];
  }
  return ret;
}
