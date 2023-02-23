#pragma once
#include <string>
#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace engine
{
	class Texture
	{
	public:
		Texture(const char* path)
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
				glGenTextures(1, &ID);
				glBindTexture(GL_TEXTURE_2D, ID);

				//Set texture wrapping to repeat
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				//Set texture filtering parameters
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				//Generate the texture using the image data
				glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, width, height, 0, colorFormat, GL_UNSIGNED_BYTE, imageData);
				glGenerateMipmap(GL_TEXTURE_2D);

				//Image data is no longer needed
				stbi_image_free(imageData);
			}
			else
			{
				std::cout << "Error loading texture from " << path << std::endl;
			}
		}
		~Texture()
		{
			glDeleteTextures(1, &ID);
		}

		//Get this textures OpenGL ID
		unsigned int GetID()
		{
			return ID;
		}

	private:
		unsigned int ID = 0;
	};
}