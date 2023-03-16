#pragma once
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace std;

namespace engine
{
	//Struct representing a pixel's r, g, b, and a values
	struct Pixel
	{
		unsigned char r, g, b, a;
	};

	//Image class basically just a 2d vector of rgba values
	class Image
	{
	public:
		Image(const char* path)
		{
			//Load image
			stbi_set_flip_vertically_on_load(true);
			unsigned char* imageData = stbi_load(path, &width, &height, &channels, 4);

			//If the image is loaded successfully
			if (imageData)
			{
				//Resize the pixmap vector to be able to insert just by index
				pixmap.resize(width);
				for (size_t i = 0; i < width; i++)
					pixmap[i].resize(height);

				int i = 0;
				//For each row and column
				for (size_t x = 0; x < width; x++)
				{
					for (size_t y = 0; y < height; y++)
					{
						//Get the rgba values and put them in a nice to use 2D vector of Pixels
						pixmap[x][y] = Pixel{imageData[i], imageData[i + 1], imageData[i + 2], imageData[i + 3]};
						i += 4;
					}
				}

				//Image data is no longer needed
				stbi_image_free(imageData);
			}
			else
			{
				std::cout << "Error loading texture from " << path << std::endl;
			}
		}

		vector<Pixel> operator[](int i)
		{
			return pixmap[i];
		}

		int width;
		int height;
		int channels;
	private:
		vector<vector<Pixel>> pixmap;
	};
	/*
	//Slice spritesheet image to multiple textures
	vector<Texture*> SliceSpritesheet(const char* path)
	{
		return vector<Texture*>();
	}*/
}