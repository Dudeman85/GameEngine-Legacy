#pragma once

class TextRender
{
public:
	// face_index points to the font in the file if file has
	// multiple fonts. In this case index is 0
	void FontLoad(const char* filepathname, FT_Long face_index);
};