#pragma once
// Freetype headers
#include <ft2build.h>
#include FT_FREETYPE_H

#include <engine/GL/Camera.h>
#include <string>
#include <iostream>
#include <engine/GL/Shader.h>
#include <engine/Application.h>
#include <map>


using namespace std;

class TextRender
{
public:
	
	// Constructor
	// face_index points to the font in the file if file has
	// multiple fonts. In this case index is 0
	TextRender(engine::Camera* cam);
	
	//Destructor
	~TextRender();

	void LoadText(const char* filepathname, FT_Long face_index);

	// Text render function that renders a string of characters
	void RenderText(string text, float x, float y, float scale, glm::vec3 color);

private:
	
	void initGLStuff();

	FT_Library ft;
	FT_Face face;

	unsigned int VAO, VBO;

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
	struct Character
	{
		unsigned int TextureID; // ID handle of t he glyph texture
		glm::ivec2 Size;		// Size of glyph
		glm::ivec2 Bearing;		// Offset from baseline to left/top of glyph
		unsigned int Advance;	// Offset to advance to next glyph
	};

	std::map<char, Character> characters;

	unsigned int texture;

	engine::Shader* m_shader;
	engine::Camera* camera;
};