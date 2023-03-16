#pragma once
#include <vector>
#include <engine/GL/Texture.h>

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
						pixmap[x][y] = Pixel{ imageData[i], imageData[i + 1], imageData[i + 2], imageData[i + 3] };
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
		Image(vector<vector<Pixel>> pixels)
		{
			width = pixels.size();
			height = pixels[0].size();
			channels = 4;
			pixmap = pixels;
		}

		vector<Pixel> operator[](int i)
		{
			return pixmap[i];
		}

		//Get a subsection of pixels from x1 y1 top-left, to x2, y2 bottom-right. Index starts at 0
		Image Slice(int x1, int y1, int x2, int y2)
		{
			assert(x1 < x2 && y1 < y2 && "x1 and y1 must be less than x2 and y2!");
			assert(x1 >= 0 && y1 >= 0 && x2 < width && y2 < height && "Slice must be in bounds of original image!");

			vector<vector<Pixel>> slice;
			slice.resize(x2 - x1);
			for (size_t i = 0; i < x2 - x1; i++)
			{
				slice[i].resize(y2 - y1);
			}

			//For the region defined by parameters
			int sliceX = 0;
			int sliceY = 0;
			for (size_t x = x1; x < x2; x++)
			{
				sliceX++;
				for (size_t y = y1; y < y2; y++)
				{
					//Move the pixels from this pixmap to the sliced one
					slice[sliceX][sliceY] = pixmap[x][y];
					sliceY++;
				}
			}

			return Image(slice);
		}

		int width;
		int height;
		int channels;
	private:
		vector<vector<Pixel>> pixmap;

	};

	//Create a texture from an image
	//Declared in Texture.h
	Texture::Texture(Image image, unsigned int filteringType)
	{
		//Convert the image to a 1D char array for OpenGL
		unsigned char* imageData = new unsigned char[image.width * image.height * 4];
		int i = 0;
		for (size_t x = 0; x < image.width; x++)
		{
			for (size_t y = 0; y < image.height; y++)
			{
				imageData[i] = image[x][y].r;
				imageData[i + 1] = image[x][y].g;
				imageData[i + 2] = image[x][y].b;
				imageData[i + 3] = image[x][y].a;
				i += 4;
			}
		}

		//Generate and bind texture
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		//Set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filteringType);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filteringType);

		//Generate the texture using the image data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(GL_TEXTURE_2D);

		//Unbind texture
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//Slice spritesheet image to multiple textures.
	//spritesWide is how many sprites wide the spritesheet is and spritesHigh is how many sprites tall the spritesheet is
	vector<Texture> SliceSpritesheet(const char* path, int spritesWide, int spritesHigh)
	{
		vector<Texture> slicedTextures;

		//Load the spritesheet form path
		Image spritesheet = Image(path);

		//Get the size of each sprite
		const int width = floor(spritesheet.width / spritesWide);
		const int height = floor(spritesheet.height / spritesHigh);

		//Warn if the spritesheet is weirdly sized
		if (width * spritesWide != spritesheet.width || height * spritesHigh != spritesheet.height)
			cout << "Spritesheet is not divisible by sprite count. Clipping may occur!";

		//For each sprite to slice out
		for (size_t col = 0; col < spritesWide; col++)
		{
			for (size_t row = 0; row < spritesHigh; row++)
			{
				//Get the slice from the spritesheet
				Image slice = spritesheet.Slice(col * width, row * height, col * width + width, row * height + height);
				slicedTextures.push_back(Texture(slice));
			}
		}

		return slicedTextures;
	}
}