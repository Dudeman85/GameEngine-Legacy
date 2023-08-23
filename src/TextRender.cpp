#include <engine/TextRender.h>


	void TextRender::initGLStuff()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);

		// Configure VAO/VBO for texture quads
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	TextRender::TextRender(engine::Camera* cam)
	{
		m_shader = new engine::Shader("textVertexShader.glsl", "textFragmentShader.glsl");
		camera = cam;
	}

	TextRender::~TextRender()
	{
		// Clearing FreeTypes resources
		FT_Done_Face(face);
		FT_Done_FreeType(ft);
	}

	void TextRender::LoadText(const char* filepathname, FT_Long face_index)
	{
		try {
			// Initialize Freetype library
			if (FT_Init_FreeType(&ft))
			{
				throw runtime_error("ERROR::FREETYPE: Could not initi FreeType Library");
			}

			// Font loading
			if (FT_New_Face(ft, filepathname, face_index, &face))
			{
				throw runtime_error("ERROR::FREETYPE: Failed to load font");
			}

			// Pixel font size
			FT_Set_Pixel_Sizes(face, 0, 48);

			// Active glyph that creates an 8-bit grayscale bitmap
			// We can access this by face->gluph->bitmap
			if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
			{
				throw runtime_error("ERROR::FREETYPE: Failed to load Glyph");
			}

			// Disable byte-alignment restrictions
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			// 128 characters of the ASCII character set
			for (unsigned char c = 0; c < 128; c++)
			{
				// Load character glyph
				if (FT_Load_Char(face, c, FT_LOAD_RENDER))
				{
					cout << "ERROR::FREETYPE: Failed to load Glyph" << endl;
					continue;
				}

				// Generate texture
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

				// Set texture options
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				// Now store character for later use
				Character character = {
					texture,
					glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
					glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
					face->glyph->advance.x
				};

				characters.insert(pair<char, Character>(c, character));
			}

			// Setting textures unpack size to 1 byte
			// to avoid alignment issues
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			initGLStuff();
		}
		catch (const exception& e)
		{
			cerr << "Virhe ladattaessa fonttia: " << e.what() << endl;
		}
	}

	void TextRender::RenderText(string text, float x, float y, float scale, glm::vec3 color)
	{
		// Active corresponding render state
		m_shader->use();
		glUniform3f(glGetUniformLocation(m_shader->ID, "textColor"), color.x, color.y, color.z);
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(VAO);

		// iterate through all characters
		string::const_iterator c;

		for (c = text.begin(); c != text.end(); c++)
		{
			Character ch = characters[*c];

			float xpos = x + ch.Bearing.x * scale;
			float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

			float w = ch.Size.x * scale;
			float h = ch.Size.y * scale;

			// Update VBO for each character
			float vertices[6][4] = {
				{xpos, ypos + h, 0.0f, 0.0f},
				{xpos, ypos, 0.0f, 1.0f},
				{xpos + w, ypos, 1.0f, 1.0f},
				{xpos, ypos + h, 0.0f, 0.0f},
				{xpos + w, ypos, 1.0f, 1.0f},
				{xpos + w, ypos + h, 1.0f, 0.0f}
			};

			// Render glyph texture over quad
			glBindTexture(GL_TEXTURE_2D, ch.TextureID);

			// Update content of VBO memory
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			// Be sure to use glBufferSubData and not glBufferData
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			// Render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);

			// Now advance cursors for next glyph (note that advance is
			// number of 1/64 pixels)
			// Bitshift by 6 to get value in pixels ( 2^6 = 64 ( divide
			// amount of 1/64th pixels by 64 to get amount of pixels))
			x += (ch.Advance >> 6) * scale;
		}

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}