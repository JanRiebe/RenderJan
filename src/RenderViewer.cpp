#include "RenderViewer.h"
#include <string>
#include "Scene.h"



// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// The window we'll be rendering to
SDL_Window* gWindow = nullptr;
//TODO can this be removed, since it is already mentioned in the header?



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
		scene = new Scene();
		SetupTestScene();
		Render();
		EventLoop();
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
				switch (e.key.keysym.sym)
				{
					case SDLK_RETURN:
						Render();
						break;
					case SDLK_s:
						; // TODO add sphere
						break;
					case SDLK_l:
						; // TODO add light
						break;
					case SDLK_DELETE:
						; // TODO delete selected light or sphere
						break;
					case SDLK_PLUS:
						; // TODO make selected light or sphere bigger
						break;
					case SDLK_MINUS:
						; // TODO make selected light or sphere smaller
						break;
					case SDLK_UP:
						; // TODO move selected light or sphere in y axis
						break;
					case SDLK_DOWN:
						; // TODO move selected light or sphere in y axis
						break;
					case SDLK_LEFT:
						; // TODO move selected light or sphere in x axis
						break;
					case SDLK_RIGHT:
						; // TODO move selected light or sphere in x axis
						break;
					case SDLK_z:
						; // TODO move selected light or sphere in z axis
						break;
					case SDLK_h:
						; // TODO move selected light or sphere in z axis
						break;
					default:
						break;
					}
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



void RenderViewer::SetupTestScene()
{
	//lights.push_back(LightSource(Point(0.0f, 0.0f, -100.0f), Point(1.0f, 0.0f, 0.0f), 1.0f));
	//lights.push_back(LightSource(Point(-100.0f, 100.0f, -100.0f), Point(0.0f, 1.0f, 0.0f), 1.0f));
	//lights.push_back(LightSource(Point(-50.0f, -100.0f, -100.0f), Point(0.0f, 0.0f, 1.0f), 1.0f));
	scene->lights.push_back(LightSource(Point(-65.0f, -45.0f, -35.0f), Point(1.0f, 1.0f, 1.0f), 30000.0f, false));

	scene->spheres.push_back(Sphere{ Point(0.0f,0.0f,0.0f), 40.0f, Point(1.0f, 1.0f, 1.0f) });
	scene->spheres.push_back(Sphere{ Point(0.0f,10.0f,20.0f), 30.0f, Point(1.0f, 1.0f, 0.0f) });
	scene->spheres.push_back(Sphere{ Point(0.0f,-10.0f,-10.0f), 30.0f, Point(0.0f, 1.0f, 1.0f) });
	scene->spheres.push_back(Sphere{ Point(-50.0f,-30.0f,20.0f), 10.0f, Point(1.0f, 0.0f, 0.0f) });
	//spheres.push_back(Sphere{ Point(.0f,.0f,.0f), 500.0f, Point(0.18f, 0.18f, 0.18f) });

}


void RenderViewer::Render()
{
	// Setting the start time to calculate how long the render took.
	unsigned int startTime = SDL_GetTicks();


	// Rendering the scene into an image.
	Image* img = RenderScene(scene,SCREEN_WIDTH, SCREEN_HEIGHT);

	// Calsulating the time it took to render.
	int timeTook = SDL_GetTicks() - startTime;

	// Displaying the image on the window.
	Show(img);

	// The image can now be deleted, since its values have been copied to the displaySurface in Show.
	delete img;

	printf("\nRender complete in %d ticks.\nClose window to continue program.\n", timeTook);
}



void RenderViewer::Close()
{
	// Deallocate scene
	delete scene;

	// Deallocate surface
	SDL_FreeSurface(gScreenSurface);
	gScreenSurface = NULL;
	SDL_FreeSurface(displaySurface);
	displaySurface = NULL;

	// Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	// Quit SDL subsystems
	SDL_Quit();

	printf("RenderViewer closed.\n");
}
