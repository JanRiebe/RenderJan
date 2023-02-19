#include "SampleViewer.h"
#include <string>
#include "Image.h"
#include <fstream>
#include <iostream>
#include <map>
#include "sampler.h"


// The window we'll be rendering to
SDL_Window* gWindowSample = nullptr;
//TODO can this be removed, since it is already mentioned in the header?



SampleViewer::SampleViewer()
{
	//Start up SDL and create window
	if (!Init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		Render();
		EventLoop();
	}
}


/*
Destrcutor
Cleans up SDL.
*/
SampleViewer::~SampleViewer()
{
	Close();
}


void PutPixel32_nolock_sample(SDL_Surface * surface, int x, int y, Uint32 color)
{
	Uint8 * pixel = (Uint8*)surface->pixels;
	pixel += (y * surface->pitch) + (x * sizeof(Uint32));
	*((Uint32*)pixel) = color;
}



/*
Displays the image by accessing the pixels stored in it.
*/
void SampleViewer::Show(Image* img)
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
			#define CLAMP(x) if(x>1)x=1;else if(x<0) x=0;
			Pixel p = img->getPixelAt(x, y);
			CLAMP(p.r);
			CLAMP(p.g);
			CLAMP(p.b);
			CLAMP(p.a);
			Uint32 color = SDL_MapRGBA(displaySurface->format, p.r * 255, p.g * 255, p.b * 255, p.a * 255);

			// Writing the float data into the surface.
			//Uint32 *target_pixel = (Uint32 *)displaySurface->pixels + y * displaySurface->pitch + x * sizeof(Uint32);
			//*target_pixel = pixel;

			PutPixel32_nolock_sample(displaySurface, x, y, color);
		}
	}

	SDL_BlitSurface(displaySurface, NULL, gScreenSurface, NULL);
	SDL_UpdateWindowSurface(gWindowSample);

}



void SampleViewer::EventLoop()
{
	//Main loop flag
	bool quit = false;
	//Event handler
	SDL_Event e;

	//While application is running
	while (!quit) {
		std::cout<<"not quit\n";
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0) // polls an event from the event queue and puts it into e (the union we created before)
		{
			std::cout<<"\tpoll\n";
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			else if (e.type == SDL_KEYUP)
			{
				switch (e.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						quit = true;
						break;
				}
			}
		}
		Render();
		SDL_UpdateWindowSurface(gWindowSample);
	}
}




bool SampleViewer::Init()
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
		gWindowSample = SDL_CreateWindow("Renderjan", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		int x,y;
		SDL_GetWindowSize(gWindowSample,&x,&y);
		
		if (gWindowSample == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Get window surface
			gScreenSurface = SDL_GetWindowSurface(gWindowSample);

		}
	}


	printf("Successfully initialised RenderViewer.");
	return success;
}




void SampleViewer::Render()
{

	std::cout<<"render:\n";


	Image* img = new Image();

	int imgSize = SCREEN_WIDTH;

	Pixel* pix = new Pixel[imgSize*imgSize];

	for(int i=0;i<4096*1;i++)
	{
		int x, y;
		x = imgSize * sampleDimension(i, 1);
		y = imgSize * sampleDimension(i, 2);
		Pixel* p = &pix[y*imgSize + x];
		p->r = 1.0f;
		p->g = 1.0f;
		p->b = 1.0f;
		p->a = 1.0f;
		std::cout <<i<<" "<< x <<" "<< y<<"\n";
	}
	img->setPixel(pix, imgSize, imgSize);

	
	// Displaying the image on the window.
	Show(img);

	// The image can now be deleted, since its values have been copied to the displaySurface in Show.
	delete img;
}



void SampleViewer::Close()
{
	// Deallocate surface
	SDL_FreeSurface(gScreenSurface);
	gScreenSurface = NULL;
	SDL_FreeSurface(displaySurface);
	displaySurface = NULL;

	// Destroy window
	SDL_DestroyWindow(gWindowSample);
	gWindowSample = NULL;

	// Quit SDL subsystems
	SDL_Quit();

	printf("RenderViewer closed.\n");
}
