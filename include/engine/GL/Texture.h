#pragma once
#include <string>
#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <engine/Image.h>

namespace engine
{
	//Abstraction class for OpenGL textures
	class Texture
	{
	public:
		//Load a texture from path
		Texture(const char* path, unsigned int filteringType = GL_NEAREST)
		{
			//Load image
			int width, height, nrChannels;
			stbi_set_flip_vertically_on_load(true);
			unsigned char* imageData = stbi_load(path, &width, &height, &nrChannels, 0);

			if (imageData)
			{
				//Set the OpenGL texture format to include alpha if appropriate
				GLint colorFormat;
				if (nrChannels == 4)
				{
					colorFormat = GL_RGBA;
				}
				else
				{
					glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
					colorFormat = GL_RGB;
				}

				//Generate and bind texture
				glGenTextures(1, &id);
				glBindTexture(GL_TEXTURE_2D, id);

				//Set texture filtering parameters
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filteringType);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filteringType);

				//Generate the texture using the image data
				glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, width, height, 0, colorFormat, GL_UNSIGNED_BYTE, imageData);
				glGenerateMipmap(GL_TEXTURE_2D);

				//Unbind texture
				glBindTexture(GL_TEXTURE_2D, 0);

				//Image data is no longer needed
				stbi_image_free(imageData);
			}
			else
			{
				std::cout << "Error loading texture from " << path << std::endl;
			}
		}
		//Create a texture from an image
		Texture(Image image, unsigned int filteringType = GL_NEAREST)
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

		~Texture()
		{
			glDeleteTextures(1, &id);
		}

		//Sets the OpenGL sampling type when up and downscaling the texture. Ex. GL_NEAREST, GL_LINEAR, etc.
		void SetScalingFilter(unsigned int type)
		{
			glBindTexture(GL_TEXTURE_2D, id);

			//Set texture filtering parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, type);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, type);

			//Unbind texture
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		//Get this textures OpenGL ID
		unsigned int ID()
		{
			return id;
		}

		//Use this texture to draw the next sprite
		void Use()
		{
			glBindTexture(GL_TEXTURE_2D, id);
		}

	private:
		unsigned int id = 0;
	};
}