// Freetype headers
#include <ft2build.h>
#include FT_FREETYPE_H
#include <engine/TextRender.h>

#include <iostream>
#include <engine/GL/Shader.h>
#include <engine/Application.h>
#include <engine/GL/Camera.h>
#include <map>

using namespace std;

//TextRender::TextRender(engine::Camera* cam)
//{
	//m_shader = new engine::Shader("vertexShader.glsl", "fragmentShader.glsl");
	//camera = cam;
//}

TextRender::~TextRender()
{
}

void TextRender::FontLoad(const char* filepathname, FT_Long face_index)
{	
	// Initialize FreeType library
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		cout << "ERROR::FREETYPE: Could not initi FreeType Library" << endl;
		//return -1;
	}

	// Font loading
	FT_Face face;
	if (FT_New_Face(ft, "assets/fonts/ARIAL.TTF", 0, &face))
	{
		cout << "ERROR::FREETYPE: Failed to load font" << endl;
		//return -1;
	}

	// Pixel font size
	FT_Set_Pixel_Sizes(face, 0, 48);

	// Active glyph that creates an 8-bit grayscale bitmap
	// We can access this by face->glyph->bitmap
	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
	{
		cout << "ERROR::FREETYPE: Failed to load Glyph" << endl;
		//return -1;
	}
	
	/*
	* bitmap properties that are needed to store characters into
	* struct map:
	*
	*  width: The width (in pixels) of the bitmap accessed via face->glyph->bitmap.width.
	*
	*  height: The height (in pixels) of the bitmap accessed via face->glyph->bitmap.rows.
	*
	*  bearingX: The horizontal bearing e.g. the horizontal position (in pixels) of the bitmap relative
	*			 to the origin accessed via face->glyph->bitmap.left.
	*
	* bearingY: The vertical bearing e.g. the vertical position (in pixels) of the bitmap relative
	*			to the baseline accessed via face->glyph->bitmap_top.
	*
	* advance: The horizontal advance e.g. the horizontal distance (in 1/64th pixels) from the origin
	*			to the origin of the next glyph. Accessed via face->glyph-advance.x
	*/

	// Character struct where to store data for queries for character rendering
	struct Character {
		unsigned int TextureID; // ID handle of t he glyph texture
		glm::ivec2 Size;		// Size of glyph
		glm::ivec2 Bearing;		// Offset from baseline to left/top of glyph
		unsigned int Advance;	// Offset to advance to next glyph
	};

	std::map<char, Character> Characters;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restrictions

	// 128 characters of the ASCII character set
	for (unsigned char c = 0; c < 128; c++)
	{
		// load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			cout << "ERROR::FREETYPE: Failed to load Glyph" << endl;
			continue;
		}

		// generate texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		// set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};

		Characters.insert(pair<char, Character>(c, character));

	}

	// Setting textures unpack size to 1 byte
	// to avoid alignment issues
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Clearing FreeTypes resources
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	// configure VAO/VBO for texture quads
   // -----------------------------------
	//glGenVertexArrays(1, &VAO);
	//glGenBuffers(1, &VBO);
	//glBindVertexArray(VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TextRender::initGLStuff()
{

}

//void TextRender::RenderText(string text, float x, float y, float scale, glm::vec3 color)
//{
	// active corresponding render state
	//m_shader->use();
	//glUniform3f(glGetUniformLocation(m_shader->ID, "textColor"), color.x, color.y, color.z);
	//glActiveTexture(GL_TEXTURE0);
	//glBindVertexArray(VAO);

	// iterate through all characters
	//string::const_iterator c;

	//for(c = text.begin(); c != text.end(); c++)
	//{
		//Character ch = Characters[*c];

		//float xpos = x + ch.Bearing.x * scale;
		//float yps = y - (ch.Size.y - ch.Bearing.y) * scale;

		//float w = ch.Size.x * scale;
		//float h = ch.Size.y * scale;
		// update VBO for each character
		//float vertices[6][4] = {
			//{ xpos,     ypos + h,   0.0f, 0.0f },
			//{ xpos,     ypos,       0.0f, 1.0f },
			//{ xpos + w, ypos,       1.0f, 1.0f },

			//{ xpos,     ypos + h,   0.0f, 0.0f },
			//{ xpos + w, ypos,       1.0f, 1.0f },
			//{ xpos + w, ypos + h,   1.0f, 0.0f }
		//};
		// render glyph texture over quad
		//glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of VBO memory
		//glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		//x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	//}
	//glBindVertexArray(0);
	//glBindTexture(GL_TEXTURE_2D, 0);
	//}
//}

