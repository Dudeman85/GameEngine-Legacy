#pragma once

// TODO: Need to see how to use multiple Text TTF files.

// FreeType includes
#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <engine/GL/Shader.h>
#include <engine/GL/Camera.h>
#include <map>

using namespace std;

struct Character
{
	unsigned int TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	unsigned int Advance;
};

namespace engine
{
	class TextRender
	{
	public:
		// Constructor
		TextRender();
		
		// Destructor
		~TextRender();

		// VAO & VBO function
		void TexConfig();

		// Text loading function
		void LoadText();
		
		// Text Rendering function
		void RenderText(Camera* cam, string text, float x, float y, float scale, glm::vec3 colour);

	private:
		map<GLchar, Character> Characters;

		unsigned int VAO, VBO;

		unsigned int texture;

		FT_Library ft;
		FT_Face face;

		glm::mat4 projection;

		engine::Shader* m_shader;
	};
}