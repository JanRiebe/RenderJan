#pragma once
/*
This class defines how an image is internally handled.
*/


struct Pixel
{
	float r, g, b, a;
};

class Image
{
private:
	Pixel* pixel = nullptr;
	unsigned int width = 0;
	unsigned int height = 0;
public:
	~Image()
	{
		delete pixel;
	}

	void setPixel(Pixel* pixel, unsigned int width, unsigned int height)
	{
		this->pixel = pixel;
		this->width = width;
		this->height = height;
	}

	const Pixel getPixelAt(int x, int y)
	{
		if (x >= width || y >= height)
			throw "The given picture coordinates are outside of the size of the picture.";
		return pixel[x + y*width];
	}

	const unsigned int getWidth()
	{
		return width;
	}

	const unsigned int getHeight()
	{
		return height;
	}
};