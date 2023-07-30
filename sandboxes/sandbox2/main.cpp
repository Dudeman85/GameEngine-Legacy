//#include <iostream>
//#include <engine/Application.h>
//#include <engine/Tilemap.h>
//#include <vector>
//#include <chrono>
//#include <thread>
//
//// Freetype headers
//#include <ft2build.h>
//#include FT_FREETYPE_H
//
//using namespace std;
//using namespace engine;
//
//ECS ecs;
//
//
//int main()
//{
//	// Create the window and OpenGL context before creating EngineLib
//	GLFWwindow* window = CreateWindow(800, 600, "Sandbox2_Main");
//
//	// Initialize the default engine library
//	EngineLib engine;
//
//	// Create the Camera
//	Camera cam = Camera(800, 600);
//
//	// Window background colour
//	RenderSystem::SetBackgroundColor(0, .5, .1);
//
//	// Tilemap
//
//	// Game loop
//	while (!glfwWindowShouldClose(window))
//	{
//		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//		{
//			glfwSetWindowShouldClose(window, true);
//		}
//
//		// Update all engine systems, this usually should go last in the game loop
//		// For greater control of system execution, you can update each one manually
//		engine.Update(&cam);
//
//		// OpenGL stuff, goes very last
//		glfwSwapBuffers(window);
//		glfwPollEvents();
//	}
//
//	// Initialize FreeType library
//	FT_Library ft;
//	if (FT_Init_FreeType(&ft))
//	{
//		cout << "ERROR::FREETYPE: Could not initi FreeType Library" << endl;
//		return -1;
//	}
//
//	// Font loading
//	FT_Face face;
//	if (FT_New_Face(ft, "assets/fonts/Arial.ttf", 0, &face))
//	{
//		cout << "ERROR::FREETYPE: Failed to load font" << endl;
//		return -1;
//	}
//
//	// Pixel font size
//	FT_Set_Pixel_Sizes(face, 0, 48);
//
//	// Active glyph that creates an 8-bit grayscale bitmap
//	// We can access this by face->glyph->bitmap
//	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
//	{
//		cout << "ERROR::FREETYPE: Failed to load Glyph" << endl;
//		return -1;
//	}
//
//	/*
//	* bitmap properties that are needed to store characters into
//	* struct map:
//	* 
//	*  width: The width (in pixels) of the bitmap accessed via face->glyph->bitmap.width.
//	* 
//	*  height: The height (in pixels) of the bitmap accessed via face->glyph->bitmap.rows.
//	* 
//	*  bearingX: The horizontal bearing e.g. the horizontal position (in pixels) of the bitmap relative 
//	*			 to the origin accessed via face->glyph->bitmap.left.
//	* 
//	* bearingY: The vertical bearing e.g. the vertical position (in pixels) of the bitmap relative
//	*			to the baseline accessed via face->glyph->bitmap_top.
//	* 
//	* advance: The horizontal advance e.g. the horizontal distance (in 1/64th pixels) from the origin
//	*			to the origin of the next glyph. Accessed via face->glyph-advance.x
//	*/
//
//	// Character struct where to store data for queries for character rendering
//	struct Character {
//		unsigned int TextureID; // ID handle of t he glyph texture
//		glm::ivec2 Size;		// Size of glyph
//		glm::ivec2 Bearing;		// Offset from baseline to left/top of glyph
//		unsigned int Advance;	// Offset to advance to next glyph
//	};
//
//	std::map<char, Character> Characters;
//
//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restrictions
//
//	// 128 characters of the ASCII character set
//	for (unsigned char c = 0; c < 128; c++)
//	{
//		// load character glyph
//		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
//		{
//			cout << "ERROR::FREETYPE: Failed to load Glyph" << endl;
//			continue;
//		}
//
//		// generate texture
//		unsigned int texture;
//		glGenTextures(1, &texture);
//		glBindTexture(GL_TEXTURE_2D, texture);
//		glTexImage2D(
//			GL_TEXTURE_2D,
//			0,
//			GL_RED,
//			face->glyph->bitmap.width,
//			face->glyph->bitmap.rows,
//			0,
//			GL_RED,
//			GL_UNSIGNED_BYTE,
//			face->glyph->bitmap.buffer
//		);
//
//		// set texture options
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//		// now store character for later use
//		Character character = {
//			texture,
//			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
//			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
//			face->glyph->advance.x
//		};
//
//		Characters.insert(pair<char, Character>(c, character));
//
//	}
//
//	glfwTerminate();
//
//	return 0;
//}









//	//Initialize the default engine library
//	EngineLib engine;
//
//	engine.physicsSystem->gravity = Vector2(0, -400);
//	engine.physicsSystem->step = 4;
//
//	//Create the camera
//	Camera cam = Camera(800, 600);
//
//	//Load a new texture
//	Texture texture = Texture("strawberry.png");
//
//	//Create a new entity
//	Entity player = ecs.newEntity();
//	ecs.addComponent(player, Transform{ .x = 0, .y = 0 ,.z = 1, .xScale = 20, .yScale = 20 });
//	ecs.addComponent(player, Sprite{});
//	ecs.addComponent(player, Animator{});
//	ecs.addComponent(player, Rigidbody{ .gravityScale = 0, .drag = 0, .friction = 0, .elasticity = 0 });
//	ecs.addComponent(player, BoxCollider{});
//	BoxCollider& collider = ecs.getComponent<BoxCollider>(player);
//
//	//Define the test animation
//	Animator& animator = ecs.getComponent<Animator>(player);
//	auto testAnims = AnimationsFromSpritesheet("gradient.png", 2, 2, vector<int>(4, 200));
//	AnimationSystem::AddAnimation(player, testAnims[0], "1");
//	AnimationSystem::AddAnimation(player, testAnims[1], "2");
//	AnimationSystem::PlayAnimation(player, "2", true);
//
//	//Top-Right
//	Entity sprite2 = ecs.newEntity();
//	ecs.addComponent(sprite2, Transform{ .x = 300, .y = 200, .xScale = 20, .yScale = 20 });
//	ecs.addComponent(sprite2, Sprite{ &texture });
//	ecs.addComponent(sprite2, Rigidbody{ .kinematic = true });
//	ecs.addComponent(sprite2, BoxCollider{ .scale = Vector2(10, 1) });
//	//Bottom-Left
//	Entity sprite3 = ecs.newEntity();
//	ecs.addComponent(sprite3, Transform{ .x = -300, .y = -200, .xScale = 20, .yScale = 20 });
//	ecs.addComponent(sprite3, Sprite{ &texture });
//	ecs.addComponent(sprite3, Rigidbody{ .kinematic = true });
//	ecs.addComponent(sprite3, BoxCollider{});
//	//Top-Left
//	Entity sprite4 = ecs.newEntity();
//	ecs.addComponent(sprite4, Transform{ .x = -310, .y = 200, .xScale = 20, .yScale = 20 });
//	ecs.addComponent(sprite4, Sprite{ &texture });
//	ecs.addComponent(sprite4, Rigidbody{ .drag = 0.1, .friction = 0.2, .elasticity = 0.125, .kinematic = false });
//	ecs.addComponent(sprite4, BoxCollider{});
//	//Bottom-Right
//	Entity sprite5 = ecs.newEntity();
//	ecs.addComponent(sprite5, Transform{ .x = 300, .y = -200, .xScale = 20, .yScale = 20 });
//	ecs.addComponent(sprite5, Sprite{ &texture });
//	ecs.addComponent(sprite5, Rigidbody{ .velocity = Vector2(-985, 1000), .drag = 0.25, .elasticity = 0.125, .kinematic = false });
//	ecs.addComponent(sprite5, BoxCollider{});
//
//	RenderSystem::SetBackgroundColor(0, .5, .1);
//	Tilemap map(&cam);
//	map.loadMap("assets/demo.tmx");
//	engine.renderSystem->SetTilemap(&map);
//	map.position = glm::vec3(0.0f, 0.0f, 0.0f);
//
//
//	//engine.physicsSystem->SetTilemap(&map);
//	//Game Loop
//	while (!glfwWindowShouldClose(window))
//	{
//		//std::this_thread::sleep_for(std::chrono::milliseconds{100});
//
//		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//			glfwSetWindowShouldClose(window, true);
//
//		//test movement
//		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
//		{
//			engine.physicsSystem->Move(player, Vector2(500, 0) * engine.deltaTime);
//		}
//		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
//		{
//			engine.physicsSystem->Move(player, Vector2(-500, 0) * engine.deltaTime);
//		}
//		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
//		{
//			engine.physicsSystem->Move(player, Vector2(0, 500) * engine.deltaTime);
//		}
//		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
//		{
//			engine.physicsSystem->Move(player, Vector2(0, -500) * engine.deltaTime);
//		}
//
//		Transform playerTransform = ecs.getComponent<Transform>(player);
//		cam.SetPosition(playerTransform.x, playerTransform.y, playerTransform.z + 100);
//
//		if (collider.collisions.size() > 0)
//		{
//			for (const Collision& c : collider.collisions)
//			{
//				cout << c.a << " " << c.b << endl;
//			}
//		}
//
//
//		//Update all engine systems, this usually should go last in the game loop
//		//For greater control of system execution, you can update each one manually
//		engine.Update(&cam);
//
//
//
//		//TODO add view matrix and get projection matrix from camera so that tilemap is rendered in the correct place
//		//map.draw(1.0f);
//
//
//		//OpenGL stuff, goes very last
//		glfwSwapBuffers(window);
//		glfwPollEvents();
//	}
//
//	glfwTerminate();
//
//	return 0;
//}
////#endif
//
//
