#include "stdafx.h"
#include "RenderViewer.h"
#include <string>
#include "renderingFunctions.h"





/*
Constructor
Creates a render viewer window on which rendered images can be shown.
Starts a loop to listen to system input events for handling window closing.
*/
RenderViewer::RenderViewer()
{
	//Start up SDL and create window 
	if (!Init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//EventLoop();
	}
}


/*
Destrcutor
Cleans up SDL.
*/
RenderViewer::~RenderViewer()
{
	Close();
}


void PutPixel32_nolock(SDL_Surface * surface, int x, int y, Uint32 color)
{
	Uint8 * pixel = (Uint8*)surface->pixels;
	pixel += (y * surface->pitch) + (x * sizeof(Uint32));
	*((Uint32*)pixel) = color;
}



/*
Displays the image by accessing the pixels stored in it.
*/
void RenderViewer::Show(Image* img)
{

	// Setting up the surface.
	SDL_FreeSurface(displaySurface);
	displaySurface = SDL_CreateRGBSurface(0, img->getWidth(), img->getHeight(), 32, 0, 0, 0, 0);
	
	// Writing the image data into the surface.
	for (int x = 0; x < img->getWidth(); x++)
	{
		for (int y = 0; y < img->getHeight(); y++)
		{
			// Transforming the float pixel data to fit the data needed for the surface.
			/*
			Pixel p = img->getPixelAt(x, y);
			Uint32 r = (Uint32)((unsigned char)(p.r * 255)) << (3 * 8);
			Uint32 g = (Uint32)((unsigned char)(p.g * 255)) << (2 * 8);
			Uint32 b = (Uint32)((unsigned char)(p.b * 255)) << (1 * 8);
			Uint32 a = (Uint32)((unsigned char)(p.a * 255));

			Uint32 pixel = r + g + b + a;
			*/
			Pixel p = img->getPixelAt(x, y);
			Uint32 color = SDL_MapRGBA(displaySurface->format, p.r * 255, p.g * 255, p.b * 255, p.a * 255);

			// Writing the float data into the surface.
			//Uint32 *target_pixel = (Uint32 *)displaySurface->pixels + y * displaySurface->pitch + x * sizeof(Uint32);
			//*target_pixel = pixel;

			PutPixel32_nolock(displaySurface, x, y, color);
		}
	}

	Render();

	printf( "\nRender complete.\nClose window to continue program.\n");
	// Keep the window open until closed and thus the program running but paused.
	EventLoop();
}


void RenderViewer::Render()
{
	SDL_BlitSurface(displaySurface, NULL, gScreenSurface, NULL);
	SDL_UpdateWindowSurface(gWindow);
}

void RenderViewer::EventLoop()
{
	//Main loop flag 
	bool quit = false;
	//Event handler 
	SDL_Event e;

	//While application is running 
	while (!quit) {

		//Handle events on queue 
		while (SDL_PollEvent(&e) != 0) // polls an event from the event queue and puts it into e (the union we created before)
		{
			//User requests quit 
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			else if (e.type == SDL_KEYUP)
			{
				if (e.key.keysym.sym == SDLK_r)
					RenderTestScene();
				quit = true;
			}
		}
	}
}




bool RenderViewer::Init()
{
	//Initialization flag 
	bool success = true;
	//Initialize SDL 
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Create window 
		gWindow = SDL_CreateWindow("Renderjan", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Get window surface
			gScreenSurface = SDL_GetWindowSurface(gWindow);

		}
	}
	printf("Successfully initialised RenderViewer.");
	return success;
}





void RenderViewer::Close()
{
	//Deallocate surface
	SDL_FreeSurface(gScreenSurface);
	gScreenSurface = NULL;
	SDL_FreeSurface(displaySurface);
	displaySurface = NULL;

	//Destroy window 
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();

	printf("RenderViewer closed.\n");
}


