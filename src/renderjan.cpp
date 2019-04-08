// renderjan.cpp : Defines the entry point for the console application.
//

#include <iostream>

#include "RenderViewer.h"

#include "MathFunctions.h"

using namespace std;




int main(int argc, char* args[])
{

	cout << "Welcome to Renderjan!\n\n";


		int size = 4;

	Matrix4x4 mat_a =
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
	mat_a =  GetRotationMatrix(PI/2,0,0);
	Matrix4x4 mat_b =
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};

	float vec[size] = {1,2,3,1};
	for(int c=0; c<size; c++)
	{
		cout << vec[c]<<" ";
	}
		cout<<endl;


	Matrix4x4 mat_c;
	mat_c = MatMult(mat_a, mat_b);

	for(int r=0; r<size; r++)
  {
    for(int c=0; c<size; c++)
    {
			cout << mat_c[c+r*size]<<" ";
		}
		cout << endl;
	}

		float * vec_c;
		vec_c = VecMatMult(vec, mat_c);
		vec_c = HomogVec(vec_c);

		cout<<endl;
	for(int c=0; c<size; c++)
	{
		cout << vec_c[c]<<" ";
	}

	cout<<endl;





	RenderViewer rendView;

	return 0;
}




//TODO continue here
// http://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/shading-spherical-light
