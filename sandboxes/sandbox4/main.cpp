#include <engine/Application.h>

#include <engine/Primitive.h>

using namespace engine;

//Creates a global instance of the ECS manager
ECS ecs;

struct PolygonCollider
{
	//The vertices of the polygon making up the collider, going clockwise
	//The vertices must form a convex polygon
	vector<Vector2> vertices;
};

bool CheckOverlap(Entity a, Entity b)
{
	PolygonCollider& aCollider = ecs.getComponent<PolygonCollider>(a);
	PolygonCollider& bCollider = ecs.getComponent<PolygonCollider>(b);
	Transform& aTransform = ecs.getComponent<Transform>(a);
	Transform& bTransform = ecs.getComponent<Transform>(b);

	vector<Vector2> aVerts;
	//Rotate, scale and move every point
	for (int i = 0; i < aVerts.size(); i++)
	{
		//Rotate
		aVerts[i] = Vector2((aVerts[i].x * cos(aTransform.rotation.z)), (aVerts[i].y * sin(aTransform.rotation.z)));
		//Scale
		aVerts[i] = Vector2(aVerts[i].x * aTransform.scale.x, aVerts[i].y * aTransform.scale.y);
	}


	//For each vertice in a
	for (int i = 0; i < aCollider.vertices.size(); i++)
	{
		//Overflow nextVertice to beginning
		int nextVertice = i < aCollider.vertices.size() - 1 ? i + 1 : 0;
		//Calculate the normal vector to project the other polygon to (left normal because clockwise)
		Vector2 normal = (aCollider.vertices[nextVertice] - aCollider.vertices[i]).LeftNormal();


		Entity primitive = ecs.newEntity();
		Primitive* line = Primitive::Line(Vector3(0, 0, 0), normal.Normalize());
		ecs.addComponent(primitive, Transform{ .scale = Vector3(1) });
		ecs.addComponent(primitive, PrimitiveRenderer{ .primitive = line, .color = Vector3(0, 255.f / (i / 4.f + 1.f), 0), .wireframe = true, .uiElement = true });
		Entity primitive2 = ecs.newEntity();
		Primitive* line2 = Primitive::Line(aCollider.vertices[i], aCollider.vertices[nextVertice]);
		ecs.addComponent(primitive2, Transform{ .scale = Vector3(1) });
		ecs.addComponent(primitive2, PrimitiveRenderer{ .primitive = line2, .color = Vector3(0, 255.f / (i / 4.f + 1.f), 0), .wireframe = true, .uiElement = true });

		//For each vertice in b
		for (int i = 0; i < bCollider.vertices.size(); i++)
		{
			float projection = normal.Dot(bCollider.vertices[i]);

			Entity box = ecs.newEntity();
			ecs.addComponent(box, Transform{ .position = normal * projection, .scale = Vector3(0.01) });
			ecs.addComponent(box, PrimitiveRenderer{ .primitive = Primitive::Rectangle(), .color = Vector3(255, 0, 0), .wireframe = false, .uiElement = true });
		}
	}
	return false;
}


int main()
{
	//Create the window and OpenGL context before creating EngineLib
	//Paraeters define window size(x,y) and name
	GLFWwindow* window = CreateGLWindow(800, 800, "Window");
	//Initialize the default engine library
	EngineLib engine;
	//Create the camera
	Camera cam = Camera(800, 800);
	cam.perspective = false;
	cam.SetPosition(0, 0, 50);

	//changes window backround color
	SpriteRenderSystem::SetBackgroundColor(29, 46, 119);

	//Model loading
	Model model("assets/achelous.obj");

	Entity ship = ecs.newEntity();
	Transform& modelTransform = ecs.addComponent(ship, Transform{ .rotation = Vector3(90, 0, 0), .scale = Vector3(1) });
	//ecs.addComponent(ship, ModelRenderer{ .model = &model });
	//Angle of the ship, 0 is up
	float angle = 0;
	float rotationSpeed = 3;

	//UI SYSTEM TESTING
	//If a sprite is designated as a ui element the transform position will be normalized screen space coordinates, scale will also be different, rotation is the same
	//The type of camera and its size will make no difference to the appearance of the sprite
	Texture tex("assets/strawberry.png");
	Entity strawberry = ecs.newEntity();
	ecs.addComponent(strawberry, Transform{ .position = Vector3(0, 0, 0), .scale = Vector3(.1, .1, 0) });
	//ecs.addComponent(strawberry, SpriteRenderer{ .texture = &tex, .uiElement = true});


	//PRIMITIVE RENDER TESTING
	ecs.registerComponent<PrimitiveRenderer>();
	shared_ptr<PrimitiveRenderSystem> primitiveRenderSystem = ecs.registerSystem<PrimitiveRenderSystem>();
	Signature primitiveRenderSystemSignature;
	primitiveRenderSystemSignature.set(ecs.getComponentId<PrimitiveRenderer>());
	primitiveRenderSystemSignature.set(ecs.getComponentId<Transform>());
	ecs.setSystemSignature<PrimitiveRenderSystem>(primitiveRenderSystemSignature);


	Entity primitive = ecs.newEntity();
	vector<Vector3> verts1{ Vector3(0.35, 0.35, 0), Vector3(0.5, 0, 0), Vector3(0.35, -0.35, 0), Vector3(0, -0.5, 0), Vector3(-0.35, -0.35, 0), Vector3(-0.5, 0, 0), Vector3(-0.35, 0.35, 0), Vector3(0, 0.5, 0) };
	Primitive* line = Primitive::Polygon(verts1);
	Transform& primitiveTransform = ecs.addComponent(primitive, Transform{ .scale = Vector3(0.9) });
	//ecs.addComponent(primitive, PrimitiveRenderer{ .primitive = line, .color = Vector3(1, 0, 0), .wireframe = true, .uiElement = true });



	//POLYGON COLLIDER TESTING
	ecs.registerComponent<PolygonCollider>();

	Entity a = ecs.newEntity();
	Entity b = ecs.newEntity();

	vector<Vector2> verts{ Vector2(0.35, 0.35), Vector2(0.5, 0), Vector2(0.35, -0.35), Vector2(0, -0.5), Vector2(-0.35, -0.35), Vector2(-0.5, 0), Vector2(-0.35, 0.35), Vector2(0, 0.5) };
	vector<Vector2> verts2{ Vector2(0.25, 0.25), Vector2(0.25, -0.25), Vector2(-0.25, -0.25), Vector2(-0.25, 0.25) };
	vector<Vector3> verts3{ Vector3(0.25, 0.25, 0), Vector3(0.25, -0.25, 0), Vector3(-0.25, -0.25, 0), Vector3(-0.25, 0.25, 0) };

	ecs.addComponent(a, PolygonCollider{ .vertices = verts });
	ecs.addComponent(b, PolygonCollider{ .vertices = verts2 });
	ecs.addComponent(b, Transform{});
	ecs.addComponent(b, PrimitiveRenderer{ .primitive = Primitive::Polygon(verts3), .color = Vector3(1, 0, 0), .wireframe = true, .uiElement = true });

	CheckOverlap(a, b);

	//Game Loop
	while (!glfwWindowShouldClose(window))
	{
		//Close window when Esc is pressed
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		//Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			angle += rotationSpeed;
			if (angle >= 360)
				angle -= 360;

			cout << angle << endl;

			modelTransform.rotation.y += -rotationSpeed;
			modelTransform.rotation.z += rotationSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			angle -= rotationSpeed;
			if (angle < 0)
				angle += 360;

			cout << angle << endl;

			modelTransform.rotation.y += rotationSpeed;
			modelTransform.rotation.z += -rotationSpeed;
		}

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			cam.Rotate(1, 0, 0);
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			cam.Rotate(-1, 0, 0);



		//Update all engine systems, this usually should go last in the game loop
		//For greater control of system execution, you can update each one manually
		engine.Update(&cam);

		primitiveRenderSystem->Update(&cam);

		//OpenGL stuff, goes very last
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}