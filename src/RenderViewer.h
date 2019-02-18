#pragma once
#include "Image.h"

//Using SDL and standard IO
#include <stdio.h>
#include <SDL.h>

using namespace std;




class RenderViewer
{
private:
	bool Init();
	void Close();
	void Render();
	void EventLoop();

	//Screen dimension constants
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	
	//The window we'll be rendering to 
	SDL_Window* gWindow = nullptr;
	//The window renderer 
	//SDL_Renderer* gRenderer = nullptr;
	//The surface contained by the window
	SDL_Surface* gScreenSurface = nullptr;

	SDL_Surface* displaySurface = nullptr;


public:
	RenderViewer();
	~RenderViewer();
	// Presents the image in the render view window.
	void Show(Image* img);
};

/*
Idea:
Add mouse input for moving points of the triangle.
*/
