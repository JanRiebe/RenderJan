#include "BMPWriter.h"
#include <iostream>

/*
adapting this code https://en.wikipedia.org/wiki/User:Evercat/Buddhabrot.c
*/


void BMPWriter::WriteBoolMap(const std::vector<bool> * const values, const int WIDTH, const int HEIGHT, const char* const filename)
{


	uint32_t headers[13];
	FILE   * pFileSave;
	int x, y, i;

	int      nExtraBytes = (WIDTH * 3) % 4;
	int      nPaddedSize = (WIDTH * 3 + nExtraBytes) * HEIGHT;
	uint32_t nPlanes = 1;
	uint32_t nBitcount = 24 << 16;

	headers[0] = nPaddedSize + 54;    // bfSize (total file size)
	headers[1] = 0;                   // bfReserved1 bfReserved2
	headers[2] = 54;                  // bfOffbits
	headers[3] = 40;                  // biSize BITMAPHEADER
	headers[4] = WIDTH;               // biWidth
	headers[5] = HEIGHT;              // biHeight
	headers[6] = nBitcount | nPlanes; // biPlanes, biBitcount
	headers[7] = 0;                   // biCompression
	headers[8] = nPaddedSize;         // biSizeImage
	headers[9] = 0;                   // biXPelsPerMeter
	headers[10] = 0;                   // biYPelsPerMeter
	headers[11] = 0;                   // biClrUsed
	headers[12] = 0;                   // biClrImportant

	errno_t err = fopen_s(&pFileSave, filename, "wb");
	if (err != 0)
	{
		std::cout << "The file " << filename << " could not be opened.\n";
		std::cout << "Error number: " << err << std::endl;
	}

	if (pFileSave)
	{
		// Output Headers
		fprintf(pFileSave, "BM");
		for (i = 0; i < 13; i++)
		{
			fprintf(pFileSave, "%c", ((headers[i]) >> 0) & 0xFF);
			fprintf(pFileSave, "%c", ((headers[i]) >> 8) & 0xFF);
			fprintf(pFileSave, "%c", ((headers[i]) >> 16) & 0xFF);
			fprintf(pFileSave, "%c", ((headers[i]) >> 24) & 0xFF);
		}
		uint8_t r;
		uint8_t g;
		uint8_t b;
		// written upside down
		for (y = HEIGHT - 1; y >= 0; y--)
		{
			for (x = 0; x < WIDTH; x++)
			{
				if (values->at(x + WIDTH*y))
				{
					r = 'a';
					g = 'a';
					b = 'a';
				}
				else
				{
					r = 0;
					g = 0;
					b = 255;
				}


				// written BGR
				fprintf(pFileSave, "%c", b);
				fprintf(pFileSave, "%c", g);
				fprintf(pFileSave, "%c", r);
			}

			if (nExtraBytes) // lines must be of lengths divisible by 4 bytes.
				for (i = 0; i < nExtraBytes; i++)
					fprintf(pFileSave, "%c", 0);
		}

		fclose(pFileSave);

	}

}


void BMPWriter::WriteFloatMap(const std::vector<float> * const values, const int WIDTH, const int HEIGHT, const char* const filename)
{


	uint32_t headers[13];
	FILE   * pFileSave;
	int x, y, i;

	int      nExtraBytes = (WIDTH * 3) % 4;
	int      nPaddedSize = (WIDTH * 3 + nExtraBytes) * HEIGHT;
	uint32_t nPlanes = 1;
	uint32_t nBitcount = 24 << 16;

	headers[0] = nPaddedSize + 54;    // bfSize (total file size)
	headers[1] = 0;                   // bfReserved1 bfReserved2
	headers[2] = 54;                  // bfOffbits
	headers[3] = 40;                  // biSize BITMAPHEADER
	headers[4] = WIDTH;               // biWidth
	headers[5] = HEIGHT;              // biHeight
	headers[6] = nBitcount | nPlanes; // biPlanes, biBitcount
	headers[7] = 0;                   // biCompression
	headers[8] = nPaddedSize;         // biSizeImage
	headers[9] = 0;                   // biXPelsPerMeter
	headers[10] = 0;                   // biYPelsPerMeter
	headers[11] = 0;                   // biClrUsed
	headers[12] = 0;                   // biClrImportant

	errno_t err = fopen_s(&pFileSave, filename, "wb");
	if (err != 0)
	{
		std::cout << "The file " << filename << " could not be opened.\n";
		std::cout << "Error number: " << err << std::endl;
	}

	if (pFileSave)
	{
		// Output Headers
		fprintf(pFileSave, "BM");
		for (i = 0; i < 13; i++)
		{
			fprintf(pFileSave, "%c", ((headers[i]) >> 0) & 0xFF);
			fprintf(pFileSave, "%c", ((headers[i]) >> 8) & 0xFF);
			fprintf(pFileSave, "%c", ((headers[i]) >> 16) & 0xFF);
			fprintf(pFileSave, "%c", ((headers[i]) >> 24) & 0xFF);
		}
		uint8_t r;
		uint8_t g;
		uint8_t b;
		// written upside down
		for (y = HEIGHT - 1; y >= 0; y--)
		{
			for (x = 0; x < WIDTH; x++)
			{
				if (values->at(x + WIDTH*y) <= 1)
				{
					r = (uint8_t)((values->at(x + WIDTH*y))*255);
					g = r;
					b = r;
				}
				else
				{
					r = 255;
					g = 255;
					b = 255;
				}


				// written BGR
				fprintf(pFileSave, "%c", b);
				fprintf(pFileSave, "%c", g);
				fprintf(pFileSave, "%c", r);
			}

			if (nExtraBytes) // lines must be of lengths divisible by 4 bytes.
				for (i = 0; i < nExtraBytes; i++)
					fprintf(pFileSave, "%c", 0);
		}

		fclose(pFileSave);

	}

}







