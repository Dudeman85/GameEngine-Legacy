#pragma once

#include <iostream>

using namespace std;

class TextRender
{
public:

	// Constructor
	//TextRender(engine::Camera* cam);

	//Destructor
	~TextRender();

	// face_index points to the font in the file if file has
	// multiple fonts. In this case index is 0
	void FontLoad(const char* filepathname, FT_Long face_index);

	// Text render function that renders a string of characters
	//void RenderText(string text, float x, float y, float scale, glm::vec3 color);

private:

	void initGLStuff();

	//engine::Shader* m_shader;
	//engine::Camera* camera;
};