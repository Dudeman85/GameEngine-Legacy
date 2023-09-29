#include <engine/Application.h>

#include <engine/Primitive.h>
#include <map>

using namespace engine;

//Creates a global instance of the ECS manager
ECS ecs;

map<Entity, vector<vector<Entity>>> projectionPoints;
map<Entity, vector<Entity>> normalVectors;

bool OverlapAtoB(vector<Vector2> aVerts, vector<Vector2> bVerts, Entity a, Entity b)
{
	Vector2 aPosition = ecs.getComponent<Transform>(a).position;
	Vector2 bPosition = ecs.getComponent<Transform>(b).position;

	//For each vertice in b move it to have a at origin
	for (int i = 0; i < bVerts.size(); i++)
	{
		bVerts[i] += bPosition;
		bVerts[i] -= aPosition;
	}

	//ONlY FOR VISUALISATION
	bool returnValue = true;

	//For each vertice in a
	for (int i = 0; i < aVerts.size(); i++)
	{
		//Overflow nextVertice to beginning
		int nextVertice = i < aVerts.size() - 1 ? i + 1 : 0;
		//Calculate the normal vector to project the other polygon to (left normal because clockwise)
		Vector2 normal = (aVerts[nextVertice] - aVerts[i]).LeftNormal();
		float boundry = normal.Dot(aVerts[i]);

		//Visualisation
		if (normalVectors[a].size() < aVerts.size())
		{
			normalVectors[a].push_back(ecs.newEntity());
			ecs.addComponent(normalVectors[a].back(), PrimitiveRenderer{ .primitive = Primitive::Line(aPosition, normal + aPosition), .color = Vector3(182, 66, 245), .wireframe = true, .uiElement = true });
			ecs.addComponent(normalVectors[a].back(), Transform{});
		}
		else
		{
			PrimitiveRenderer& pr = ecs.getComponent<PrimitiveRenderer>(normalVectors[a][i]);
			delete pr.primitive;
			pr.primitive = Primitive::Line(aPosition, normal + aPosition);
		}

		//For Visualisation
		bool miss = true;

		//For each vertice in b
		for (int j = 0; j < bVerts.size(); j++)
		{
			float projection = normal.Dot(bVerts[j]);

			//Visualisation
			if (projectionPoints[a].size() < aVerts.size())
			{
				projectionPoints[a].push_back(vector<Entity>());
			}
			if (projectionPoints[a][i].size() < bVerts.size())
			{
				projectionPoints[a][i].push_back(ecs.newEntity());
				ecs.addComponent(projectionPoints[a][i].back(), PrimitiveRenderer{ .primitive = Primitive::Rectangle(), .color = Vector3(255, 0, 0), .wireframe = false, .uiElement = true });
				ecs.addComponent(projectionPoints[a][i].back(), Transform{ .position = normal * projection, .scale = Vector3(0.01) });
			}
			else
			{
				Transform& projectionTransform = ecs.getComponent<Transform>(projectionPoints[a][i][j]);
				PrimitiveRenderer& primitiveRenderer = ecs.getComponent<PrimitiveRenderer>(projectionPoints[a][i][j]);
				projectionTransform.position = normal * projection + aPosition;
				if (abs(projection) < abs(boundry))
				{
					primitiveRenderer.color = Vector3(255, 0, 0);
					miss = false;
				}
				else
					primitiveRenderer.color = Vector3(0, 255, 0);
			}
		}
		if (miss)
			returnValue = false;
	}
	return returnValue;
}

bool CheckOverlap(Entity a, Entity b)
{
	PolygonCollider& aCollider = ecs.getComponent<PolygonCollider>(a);
	PolygonCollider& bCollider = ecs.getComponent<PolygonCollider>(b);
	PrimitiveRenderer& aRenderer = ecs.getComponent<PrimitiveRenderer>(a);
	PrimitiveRenderer& bRenderer = ecs.getComponent<PrimitiveRenderer>(b);
	Transform& aTransform = ecs.getComponent<Transform>(a);
	Transform& bTransform = ecs.getComponent<Transform>(b);

	vector<Vector2> aVerts;
	//Rotate and scale every point, movement is handled later
	for (int i = 0; i < aCollider.vertices.size(); i++)
	{
		Vector2 transformedVert = aCollider.vertices[i];
		//Rotate
		float angle = aTransform.rotation.z * PI / 180;
		transformedVert.x = aCollider.vertices[i].x * cos(angle) - aCollider.vertices[i].y * sin(angle);
		transformedVert.y = aCollider.vertices[i].x * sin(angle) + aCollider.vertices[i].y * cos(angle);
		//Scale
		transformedVert *= Vector2(aTransform.scale);
		aVerts.push_back(transformedVert);
	}
	vector<Vector2> bVerts;
	//Rotate and scale every point, movement is handled later
	for (int i = 0; i < bCollider.vertices.size(); i++)
	{
		Vector2 transformedVert = bCollider.vertices[i];
		//Rotate
		float angle = bTransform.rotation.z * PI / 180;
		transformedVert.x = bCollider.vertices[i].x * cos(angle) - bCollider.vertices[i].y * sin(angle);
		transformedVert.y = bCollider.vertices[i].x * sin(angle) + bCollider.vertices[i].y * cos(angle);
		//Scale
		transformedVert *= Vector2(bTransform.scale);
		bVerts.push_back(transformedVert);
	}

	bool abCollided = OverlapAtoB(aVerts, bVerts, a, b);
	bool baCollided = OverlapAtoB(bVerts, aVerts, b, a);

	if (abCollided && baCollided)
	{
		aRenderer.color = Vector3(255, 0, 0);
		bRenderer.color = Vector3(255, 0, 0);
	}
	else
	{
		aRenderer.color = Vector3(0, 255, 0);
		bRenderer.color = Vector3(0, 255, 0);
	}

	return abCollided && baCollided;
}

int main()
{
	//Create the window and OpenGL context before creating EngineLib
	//Paraeters define window size(x,y) and name
	GLFWwindow* window = CreateGLWindow(1000, 1000, "Window");
	//Initialize the default engine library
	EngineLib engine;
	//Create the camera
	Camera cam = Camera(1000, 1000);
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


	//POLYGON COLLIDER TESTING
	ecs.registerComponent<PolygonCollider>();


	Entity a = ecs.newEntity();
	vector<Vector2> aVerts{ Vector2(0.35, 0.35), Vector2(0.5, 0), Vector2(0.35, -0.35), Vector2(0, -0.5), Vector2(-0.35, -0.35), Vector2(-0.5, 0), Vector2(-0.35, 0.35), Vector2(0, 0.5) };
	ecs.addComponent(a, PolygonCollider{ .vertices = aVerts });
	Transform& aTransform = ecs.addComponent(a, Transform{ .scale = Vector3(0.5) });
	ecs.addComponent(a, PrimitiveRenderer{ .primitive = Primitive::Polygon(aVerts), .color = Vector3(0, 255, 0), .wireframe = true, .uiElement = true });

	Entity b = ecs.newEntity();
	vector<Vector2> bVerts{ Vector2(0.25, 0.25), Vector2(0.25, -0.25), Vector2(-0.25, -0.25), Vector2(-0.25, 0.25) };
	ecs.addComponent(b, PolygonCollider{ .vertices = bVerts });
	Transform& bTransform = ecs.addComponent(b, Transform{ .scale = Vector3(0.5) });
	ecs.addComponent(b, PrimitiveRenderer{ .primitive = Primitive::Polygon(bVerts), .color = Vector3(0, 255, 0), .wireframe = true, .uiElement = true });

	//Game Loop
	while (!glfwWindowShouldClose(window))
	{
		//Close window when Esc is pressed
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		//Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)
		{
			bTransform.position.x += 0.01;
		}
		if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
		{
			bTransform.position.x += -0.01;
		}
		if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)
		{
			bTransform.position.y += 0.01;
		}
		if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS)
		{
			bTransform.position.y += -0.01;
		}
		if (glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS)
		{
			bTransform.rotation.z += 1;
		}
		if (glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_PRESS)
		{
			bTransform.rotation.z += -1;
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			aTransform.position.x += 0.01;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			aTransform.position.x += -0.01;
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			aTransform.position.y += 0.01;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			aTransform.position.y += -0.01;
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			aTransform.rotation.z += 1;
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		{
			aTransform.rotation.z += -1;
		}


		CheckOverlap(a, b);

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