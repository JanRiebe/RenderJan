#include "RenderViewer.h"
#include <string>
#include "Scene.h"
//#include "Material.h"
#include "json/json.h"
#include <fstream>
#include <iostream>
#include <map>


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
		SetupTestScene();
		ReadSceneFile("../test_scene_file.json");
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

	SceneElement* selected = nullptr;

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
					case SDLK_ESCAPE:
						quit = true;
						break;
					case SDLK_RETURN:
						Render();
						break;
						/*
					case SDLK_s:
						//add sphere
						scene->spheres.push_back(
							Sphere(
								Point(0.0f,0.0f,0.0f),			// position
								40.0f,											// radius
								material)										// material
							);
						// Setting the just created SceneElement as the currently selected one.
						selected = &(scene->spheres.back());
						break;
					case SDLK_l:
						// add light
						scene->lights.push_back(
							LightSource(
								Point(0.0f, 0.0f, 0.0f),	// position
								Point(1.0f, 1.0f, 1.0f),				// color
								30000.0f,												// intensity
								false														// is point light
							));
						// Setting the just created SceneElement as the currently selected one.
						selected = &(scene->lights.back());
						break;
						*/
					case SDLK_c:
						// Setting the camera as the currently selected SceneElement.
						selected = &(scene->camera);
						break;
					case SDLK_DELETE:
						// TODO delete selected light or sphere
						break;
					case SDLK_PLUS:
						// make selected light or sphere bigger
						if(selected)
						{
							selected->scale *= 2.0f;
						}
						break;
					case SDLK_MINUS:
						// make selected light or sphere smaller
						if(selected)
						{
							selected->scale *= 0.5f;
						}
						break;
					case SDLK_UP:
						// move selected light or sphere in y axis
						if(selected)
						{
							selected->position.y+=10;
						}
						break;
					case SDLK_DOWN:
						//move selected light or sphere in y axis
						if(selected)
						{
							selected->position.y-=10;
						}
						break;
					case SDLK_LEFT:
						//move selected light or sphere in x axis
						if(selected)
						{
							selected->position.x+=10;
						}
						break;
					case SDLK_RIGHT:
						// move selected light or sphere in x axis
						if(selected)
						{
							selected->position.x-=10;
						}
						break;
					case SDLK_z:
						// move selected light or sphere in z axis
						if(selected)
						{
							selected->position.z+=10;
						}
						break;
					case SDLK_h:
						// move selected light or sphere in z axis
						if(selected)
						{
							selected->position.z-=10;
						}
						break;
					default:
						break;
					}
					if(!quit)
					Render();
			}
		}
		SDL_UpdateWindowSurface(gWindow);
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
	scene = new Scene();
	scene->camera.imageWidth = SCREEN_WIDTH;
	scene->camera.imageHeight = SCREEN_HEIGHT;
/*
	if(material == nullptr)
	{
		material = new Material();
		material->baseColor = {1,1,1};
		material->ior = 1.4;
		material->reflectivity = 0;
	}
	/*
	//lights.push_back(LightSource(Point(0.0f, 0.0f, -100.0f), Point(1.0f, 0.0f, 0.0f), 1.0f));
	//lights.push_back(LightSource(Point(-100.0f, 100.0f, -100.0f), Point(0.0f, 1.0f, 0.0f), 1.0f));
	//lights.push_back(LightSource(Point(-50.0f, -100.0f, -100.0f), Point(0.0f, 0.0f, 1.0f), 1.0f));
	scene->lights.push_back(LightSource(Point(-65.0f, -45.0f, -35.0f), Point(1.0f, 1.0f, 1.0f), 30000.0f, false));

	scene->spheres.push_back(Sphere{ Point(0.0f,0.0f,0.0f), 40.0f, Point(1.0f, 1.0f, 1.0f) });
	scene->spheres.push_back(Sphere{ Point(0.0f,10.0f,20.0f), 30.0f, Point(1.0f, 1.0f, 0.0f) });
	scene->spheres.push_back(Sphere{ Point(0.0f,-10.0f,-10.0f), 30.0f, Point(0.0f, 1.0f, 1.0f) });
	scene->spheres.push_back(Sphere{ Point(-50.0f,-30.0f,20.0f), 10.0f, Point(1.0f, 0.0f, 0.0f) });
	//spheres.push_back(Sphere{ Point(.0f,.0f,.0f), 500.0f, Point(0.18f, 0.18f, 0.18f) });
*/
}


void RenderViewer::ReadSceneFile(string path)
{

	scene = new Scene();
	scene->camera.imageWidth = SCREEN_WIDTH;
	scene->camera.imageHeight = SCREEN_HEIGHT;

	std::ifstream scene_file(path, std::ifstream::binary);
	Json::Value root;
	scene_file >> root;

	// Reading in materials.
	for (int i=0;i<root["materials"].size(); i++)
	{
		string name = (root["materials"][i]["id"]).asString();
		Point baseColor = {root["materials"][i]["base_R"].asFloat(), root["materials"][i]["base_G"].asFloat(), root["materials"][i]["base_B"].asFloat()};
		float ior = root["materials"][i]["ior"].asFloat();
		float reflectivity = root["materials"][i]["reflectivity"].asFloat();
		float translucency = root["materials"][i]["translucency"].asFloat();
		float fresnel = root["materials"][i]["fresnel"].asFloat();
		scene->materials.insert ( std::pair<string, Material>(name,{baseColor, ior, reflectivity, translucency, fresnel}) );
	}

	// Reading in surfaces.
	for (int i=0;i<root["surfaces"].size(); i++)
	{
		// Reasing in data.
		float pos_X = root["surfaces"][i]["pos_X"].asFloat();
		float pos_Y = root["surfaces"][i]["pos_Y"].asFloat();
		float pos_Z = root["surfaces"][i]["pos_Z"].asFloat();
		float size = root["surfaces"][i]["size"].asFloat();
		string material = root["surfaces"][i]["material"].asString();

		// Ensuring the requested material is present.
		map<string,Material>::iterator it = scene->materials.find(material);
		if(it == scene->materials.end())
		{
			std::cout << "No material "<<material<< " found for surface "<<i<<" assigning standard material.\n";
			material = "test";
		}

		// Creating the sphere.
		scene->spheres.push_back(
			Sphere(
				Point(pos_X, pos_Y, pos_Z),
				size,
				&scene->materials[material]
			)
		);
	}

	// Reading in lights.
	for (int i=0;i<root["lights"].size(); i++)
	{
		float pos_X = root["lights"][i]["pos_X"].asFloat();
		float pos_Y = root["lights"][i]["pos_Y"].asFloat();
		float pos_Z = root["lights"][i]["pos_Z"].asFloat();
		float color_R = root["lights"][i]["color_R"].asFloat();
		float color_G = root["lights"][i]["color_G"].asFloat();
		float color_B = root["lights"][i]["color_B"].asFloat();
		float intensity = root["lights"][i]["intensity"].asFloat();


		scene->lights.push_back(
			LightSource(
				Point(pos_X, pos_Y, pos_Z),	// position
				Point(color_R, color_G, color_B),				// color
				intensity,												// intensity
				true														// is point light
			));
	}
}

void RenderViewer::Render()
{

	printf("\nRendering scene with %d spheres and %d lights.\n", (int)scene->spheres.size(), (int)scene->lights.size());

	// Setting the start time to calculate how long the render took.
	unsigned int startTime = SDL_GetTicks();

	// Rendering the scene into an image.
	Image* img = RenderScene(scene,SCREEN_WIDTH, SCREEN_HEIGHT, 8);

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
