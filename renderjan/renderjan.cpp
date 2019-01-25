// renderjan.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include "DataDefinitions.h"
#include "Face.h"
#include "Sphere.h"
#include <iostream>
#include "BMPWriter.h"
#include "RenderViewer.h"
#include "renderingFunctions.h"

using namespace std;




int main(int argc, char* args[])
{
	
	cout << "Welcome to Renderjan!\n\n";
	
	RenderTestScene();
	//system("pause");

	return 0;
}


//TODO continue here
// http://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/shading-spherical-light


