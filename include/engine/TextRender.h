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
#include <vector>

using namespace std;

struct Character
{
	unsigned int TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	unsigned int Advance;
};

struct  TrueFont
{
	const char* filepathname;
	FT_Long face_index;
	FT_UInt pixel_width;
	FT_UInt pixel_height;
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

		//

		// Multiple TrueFonts function
		map<FT_Face, TrueFont> SetUpTTF(FT_Face face, const char* filepathname, FT_Long face_index, FT_UInt pixel_width, FT_UInt pixel_height);


		// VAO & VBO function
		void TexConfig();

		// Text loading function
		void LoadText(map<FT_Face, TrueFont> Faces);
		
		// Text Rendering function
		void RenderText(Camera* cam, string text, float x, float y, float scale, glm::vec3 colour);

	private:
		map<GLchar, Character> Characters;

		map<FT_Face, TrueFont> Faces;

		unsigned int VAO, VBO;

		unsigned int texture;

		FT_Library ft;

		glm::mat4 projection;
		glm::mat4 viewMatrix;

		engine::Shader* m_shader;
	};
}