#pragma once


//Using SDL and standard IO
#include <stdio.h>
#include <SDL.h>

#include "Image.h"

using namespace std;



class SampleViewer
{
private:
	bool Init();
	void Close();
	void EventLoop();

	//Screen dimension constants
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 640;

	//The window we'll be rendering to
	SDL_Window* gWindow = nullptr;
	//The window renderer
	//SDL_Renderer* gRenderer = nullptr;
	//The surface contained by the window
	SDL_Surface* gScreenSurface = nullptr;

	SDL_Surface* displaySurface = nullptr;

	void Render();

public:
	SampleViewer();
	~SampleViewer();
	// Presents the image in the render view window.
	void Show(Image* img);
};
