#include "Renderer.h"
#include "Camera.h"
#include "ImageLoader.h"


Renderer::Renderer() 
{

}
 char DebugTxt[1000];

 Model model_aquaPig_propeller;

 Model model_cube;

// On exit must clean up any OpenGL resources e.g. the program, the buffers
Renderer::~Renderer()
{
	// TODO: clean up any memory used including OpenGL objects via glDelete* calls
	glDeleteProgram(m_default_program);
	glDeleteBuffers(1, &m_VAO);
}

// Use IMGUI for a simple on screen GUI
void Renderer::DefineGUI()
{
	// Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	ImGui::Begin("3GP");						// Create a window called "3GP" and append into it.

	ImGui::Text("Visibility.");					// Display some text (you can use a format strings too)	

	ImGui::Checkbox("Wireframe", &m_wireframe);	// A checkbox linked to a member variable

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		
	ImGui::Text( DebugTxt);

	ImGui::End();
}

// Load, compile and link the shaders and create a program object to host them
GLuint Renderer::CreateProgram(std::string vertexPath, std::string fragmentPath)
{
	// Create a new program (returns a unqiue id)
	GLuint program = glCreateProgram();

	// Load and create vertex and fragment shaders
	GLuint vertex_shader{ Helpers::LoadAndCompileShader(GL_VERTEX_SHADER, vertexPath) };
	GLuint fragment_shader{ Helpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, fragmentPath) };
	if (vertex_shader == 0 || fragment_shader == 0)
		return false;

	// Attach the vertex shader to this program (copies it)
	glAttachShader(program, vertex_shader);

	// The attibute location 0 maps to the input stream "vertex_position" in the vertex shader
	// Not needed if you use (location=0) in the vertex shader itself
	//glBindAttribLocation(m_program, 0, "vertex_position");

	// Attach the fragment shader (copies it)
	glAttachShader(program, fragment_shader);

	// Done with the originals of these as we have made copies
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	// Link the shaders, checking for errors
	if (!Helpers::LinkProgramShaders(program))
		return NULL;

	return program;
}

// Load / create geometry into OpenGL buffers	
bool Renderer::InitialiseGeometry()
{
	m_default_program = CreateProgram("Data/Shaders/vertex_shader.vert", "Data/Shaders/fragment_shader.frag");
	m_unlit_program = CreateProgram("Data/Shaders/vertex_shader_unlit.vert", "Data/Shaders/fragment_shader_unlit.frag");
	m_colour_unlit_program = CreateProgram("Data/Shaders/vertex_shader_colour_unlit.vert", "Data/Shaders/fragment_shader_colour_unlit.frag");

	// Load and compile shaders into m_program
	//if (!CreateProgram("Data\\shaders\\vertex_shader.vert", "Data\\Shaders\\fragment_shader.frag"))
	//{
	//	std::cout << "HELLO" << std::endl;

	//	return false;
	//}
		
	//--------------------------------------------SKYBOX---------------------------------------------------------------
	Model skyBox;
	std::vector<std::string> skyTextures;
	skyTextures.push_back("Data\\Models\\Sky\\Clouds\\SkyBox_Top.tga");
	skyTextures.push_back("Data\\Models\\Sky\\Clouds\\SkyBox_Right.tga");
	skyTextures.push_back("Data\\Models\\Sky\\Clouds\\SkyBox_Left.tga");
	skyTextures.push_back("Data\\Models\\Sky\\Clouds\\SkyBox_Front.tga");
	skyTextures.push_back("Data\\Models\\Sky\\Clouds\\SkyBox_Back.tga");
	skyTextures.push_back("Data\\Models\\Sky\\Clouds\\SkyBox_Bottom.tga");

	std::vector<std::string> models_Sky;
	models_Sky.push_back("Data\\Models\\Sky\\Clouds\\skybox.x");

	std::vector<glm::vec3> translations;
	std::vector<glm::vec3> rotations;

	skyBox.LoadMeshes("Data\\Models\\Sky\\Clouds\\skybox.x", skyTextures, translations, rotations);
	models.push_back(skyBox);

	//--------------------------------------------CUBE---------------------------------------------------------------

	std::vector<glm::vec3> cubeVerts
	{
		//FaceA
		glm::vec3(-10, -10, 10),//0
		glm::vec3(10, -10, 10),//1
		glm::vec3(10, 10, 10),//2
		glm::vec3(-10, 10, 10),//3

		//FaceB
		glm::vec3(-10, -10, 10),//4
		glm::vec3(-10, 10, 10),//5
		glm::vec3(-10, 10, -10),//6
		glm::vec3(-10, -10, -10),//7

		//FaceC
		glm::vec3(-10, -10, -10),//8
		glm::vec3(-10, 10,-10),//9
		glm::vec3(10, 10, -10),//10
		glm::vec3(10, -10, -10),//11

		//FaceD
		glm::vec3(10, -10, -10),//12
		glm::vec3(10, 10, -10),//13
		glm::vec3(10, 10, 10),//14
		glm::vec3(10, -10, 10),//15

		//FaceE
		glm::vec3(-10, 10, 10),//16
		glm::vec3(10, 10, 10),//17
		glm::vec3(10, 10, -10),//18
		glm::vec3(-10, 10, -10),//19

		//FaceF
		glm::vec3(-10, -10, 10),//20
		glm::vec3(10, -10, 10),//21
		glm::vec3(10, -10, -10),//22
		glm::vec3(-10, -10, -10),//23

	};

	std::vector<GLuint> cubeElements
	{
		//FaceA
		0,1,3,
		1,2,3,

		//FaceB
		4,5,7,
		5,6,7,

		//FaceC
		8,9,10,
		8,10,11,

		//FaceD
		12,14,15,
		12,13,14,

		//FaceE
		16,17,18,
		16,18,19,

		//FaceF
		20,22,21,
		20,23,22,
	};


	std::vector<glm::vec3> cubeColours
	{
		//FaceA
		glm::vec3(1.0, 0, 0),
		glm::vec3(1.0, 0, 0),
		glm::vec3(1.0, 0, 0),
		glm::vec3(1.0, 0, 0),

		//FaceB
		glm::vec3(0, 1.0, 0),
		glm::vec3(0, 1.0, 0),
		glm::vec3(0, 1.0, 0),
		glm::vec3(0, 1.0, 0),

		//FaceC
		glm::vec3(0, 0, 1.0),
		glm::vec3(0, 0, 1.0),
		glm::vec3(0, 0, 1.0),
		glm::vec3(0, 0, 1.0),

		//FaceD
		glm::vec3(0, 1.0, 1.0),
		glm::vec3(0, 1.0, 1.0),
		glm::vec3(0, 1.0, 1.0),
		glm::vec3(0, 1.0, 1.0),

		//FaceE
		glm::vec3(1.0, 1.0, 0),
		glm::vec3(1.0, 1.0, 0),
		glm::vec3(1.0, 1.0, 0),
		glm::vec3(1.0, 1.0, 0),

		//FaceF
		glm::vec3(1.0, 1.0, 1.0),
		glm::vec3(1.0, 1.0, 1.0),
		glm::vec3(1.0, 1.0, 1.0),
		glm::vec3(1.0, 1.0, 1.0),
	};

	model_cube.LoadMeshColour(cubeVerts, cubeColours, cubeElements, glm::vec3{50,50,50});
	models.push_back(model_cube);

	//--------------------------------------------AQUA_PIG---------------------------------------------------------------
	
	std::vector<std::string> textures_aquaPig;
	textures_aquaPig.push_back("Data\\Models\\AquaPig\\aqua_pig_1K.png");

	translations.push_back(glm::vec3{ 0, 0, 0 });
	rotations.push_back(glm::vec3{ 0, 0, 0 });

	Model model_aquaPig_gun;
	translations[0] = glm::vec3{ 0, 1.506 + 0.569, 0.644 - 1.866 };
	rotations[0] = glm::vec3{ 0, 0, 0 };
	model_aquaPig_gun.LoadMeshes("Data\\Models\\AquaPig\\gun.obj", textures_aquaPig, translations, rotations);
	models.push_back(model_aquaPig_gun);

	Model model_aquaPig_gun_base;
	translations[0] = glm::vec3{ 0, 0.569, -1.866 };
	rotations[0] = glm::vec3{ 0, 0, 0 };
	model_aquaPig_gun_base.LoadMeshes("Data\\Models\\AquaPig\\gun_base.obj", textures_aquaPig, translations, rotations);
	models.push_back(model_aquaPig_gun_base);

	Model model_aquaPig_hull;
	translations[0] = glm::vec3{ 0, 0, 0 };
	rotations[0] = glm::vec3{ 0, 0, 0 };
	model_aquaPig_hull.LoadMeshes("Data\\Models\\AquaPig\\hull.obj", textures_aquaPig, translations, rotations);
	models.push_back(model_aquaPig_hull);

	Model model_aquaPig_wing_left;
	translations[0] = glm::vec3{ 2.231, 0.272, -2.663 };
	rotations[0] = glm::vec3{ 0, 0, 0 };
	model_aquaPig_wing_left.LoadMeshes("Data\\Models\\AquaPig\\wing_left.obj", textures_aquaPig, translations, rotations);
	models.push_back(model_aquaPig_wing_left);
	

	Model model_aquaPig_wing_right;
	translations[0] = glm::vec3{ -2.231, 0.272, -2.663 };
	rotations[0] = glm::vec3{ 0, 0, 0 };
	model_aquaPig_wing_right.LoadMeshes("Data\\Models\\AquaPig\\wing_right.obj", textures_aquaPig, translations, rotations);
	models.push_back(model_aquaPig_wing_right);

	
	translations[0] = glm::vec3{ 0, 1.395, -3.616 };
	rotations[0] = glm::vec3{ glm::radians(90.0f), 0, 0 };
	model_aquaPig_propeller.LoadMeshes("Data\\Models\\AquaPig\\propeller.obj", textures_aquaPig, translations, rotations);
	models.push_back(model_aquaPig_propeller);

	//--------------------------------------------JEEP---------------------------------------------------------------
	Model jeepModel;
	std::vector<std::string> textures_jeep;
	textures_jeep.push_back("Data\\Models\\Jeep\\jeep_army.jpg");
	std::vector<glm::vec3> translations_jeep;

	translations_jeep.push_back(glm::vec3(10, 10, 10));
	std::vector<glm::vec3> rotations_jeep;
	jeepModel.LoadMeshes("Data\\Models\\Jeep\\jeep2.obj", textures_jeep, translations_jeep, rotations_jeep);
	models.push_back(jeepModel);

	//--------------------------------------------TERRAIN---------------------------------------------------------------
	std::vector<glm::vec3> terrainPositions;
	std::vector<glm::vec2> uvCoords;
	std::vector<GLuint> terrainElements;
	std::vector<std::string> terrainTexture;
	std::vector<glm::vec3> normals;

	terrainTexture.push_back("Data\\Textures\\grass_green-01_df_.dds");

	//------------------------POSITIONS-----------------------------------------------------------
	glm::vec3 pos;

	int xLength = 50;
	int zLength = 50;

	for (size_t z = 0; z  < zLength+1; z++)
	{
		for (size_t x = 0; x < xLength+1; x++)
		{
			pos = glm::vec3(x * 2, 0, z * 2);
			terrainPositions.push_back(pos);
			glm::vec3 norm = glm::vec3(0, 0, 0);
			normals.push_back(norm);
		}
	}
	
	int numTiles = xLength * zLength;
	bool changePattern = false;
	int offset = 0;
	//-----------------------ELEMENTS--------------------------------------------------------------
	for (size_t i = 0; i < numTiles; i++)
	{
		offset = i / xLength;

		if(((i % xLength) == 0) && ((xLength % 2) == 0))
		{
			changePattern = !changePattern;
		}

		if(!changePattern)
		{
			terrainElements.push_back(xLength + 1 + i + offset);
			terrainElements.push_back(xLength + 1 + i + 1 + offset);
			terrainElements.push_back(i + 1 + offset);

			terrainElements.push_back(xLength + 1 + i + offset);
			terrainElements.push_back(i + 1 + offset);
			terrainElements.push_back(i + offset);
		}
		else
		{
			terrainElements.push_back(xLength + 1 + i + 1 + offset);
			terrainElements.push_back(i + 1 + offset);
			terrainElements.push_back(i + offset);

			terrainElements.push_back(xLength + 1 + i + 1 + offset);
			terrainElements.push_back(i + offset);
			terrainElements.push_back(xLength + 1 + i + offset);
		}

		changePattern = !changePattern;
	}
	std::cout << "TE:"  << terrainElements.size()  << std::endl;

	//---------------------DISPLACEMENT---------------------------------------------------------------
	 
	std::vector<float> noiseValues;

	for (size_t i = 0; i < terrainPositions.size(); i++)//NOISE
	{
		noiseValues.push_back(Noise(terrainPositions[i].x, terrainPositions[i].z));

		terrainPositions[i].y += Noise(terrainPositions[i].x, terrainPositions[i].z);
	}

	std::vector<glm::vec3> vertsOrig = terrainPositions;

	PerlinNoise(terrainPositions, terrainPositions, xLength);


	Helpers::ImageLoader displaceLoader; //HEIGHT MAP

	if (!displaceLoader.Load("Data\\Heightmaps\\curvy.gif"))
	{
		std::cout << "exit!";
		return false;
	}

	float vertexXtoImage = (float)displaceLoader.Width() / xLength;
	float vertexZtoImage = (float)displaceLoader.Height() / zLength;

	BYTE* imageData = displaceLoader.GetData();

	float scaling = 0.1f;
	int i = 0;
	for (size_t z = 0; z < zLength + 1; z++)
	{
		for (size_t x = 0; x < xLength + 1; x++)
		{
			int imageX = (int)(vertexXtoImage * x);
			int imageZ = (int)(vertexZtoImage * z);

			size_t offset = ((size_t)imageX + (size_t)imageZ * displaceLoader.Width()) * 4;
			BYTE height = imageData[offset];

			terrainPositions[i].y += (float)height * scaling;

			i++;
		}
	}

	//-----------------------NORMALS-----------------------------------------------------------------

	for (size_t i = 0; i < terrainElements.size(); i+=3)
	{
		glm::vec3 cross = glm::cross((terrainPositions[terrainElements[i + 1]] - terrainPositions[terrainElements[i]]), (terrainPositions[terrainElements[i + 2]] - terrainPositions[terrainElements[i]]));

		normals[terrainElements[i]] += cross;
		normals[terrainElements[i + 1]] += cross;
		normals[terrainElements[i + 2]] += cross;
	}

	for(glm::vec3 &norm : normals)
	{
		norm = glm::normalize(norm);
	}

	//----------------------TEXTURE-------------------------------------------------------------------
	float uvMultiplier = 1.0f / xLength;

	for (size_t v = 0; v < zLength+1; v++)
	{
		for (size_t u = 0; u < xLength+1; u++)
		{
			glm::vec2 uv = glm::vec2((float)(u), (float)(v));
			uvCoords.push_back(uv);
		}
	}

	std::cout << "TE:" << uvCoords.size() << std::endl;

	//----------------------CREATE_MODEL---------------------------------------------------------------
	std::vector<glm::vec3> terrain_translations;
	std::vector<glm::vec3> terrain_rotations;

	Model terrain;
	terrain.LoadMesh(terrainPositions, terrainElements, uvCoords, normals, terrainTexture, terrain_translations, terrain_rotations);

	models.push_back(terrain);
	return true;
}

float Renderer::Noise(int x, int z)
{
	int n = x + z * 23;  // 15 is the seed
	n = (n >> 13) ^ n;
	int nn = (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;
	return 1.0f - ((float)nn / 1073741824.0f);
}

float Renderer::CosineLerp(float a, float b, float x)
{
	float ft = x * 3.1415927f;
	float f = (1.0f - cos(ft)) * 0.5f;
	return a * (1.0f - f) + b * f;
}

void Renderer::PerlinNoise(std::vector<glm::vec3>& vertices, std::vector<glm::vec3> vertsOrig, int xLength)
{
	for (size_t i = 0; i < vertices.size(); i++)
	{
		if (i < vertices.size() - (xLength + 1 + 1))
		{
			float s = vertsOrig[i].y;
			float t = vertsOrig[i + 1].y;
			float u = vertsOrig[i + xLength + 1].y;
			float v = vertsOrig[i + xLength + 1 + 1].y;

			float c1 = CosineLerp(s, t, vertsOrig[i].x);
			float c2 = CosineLerp(u, v, vertsOrig[i].x);

			vertices[i].y = CosineLerp(c1, c2, vertsOrig[i].z) * 0.5;
		}
	}
}

// Render the scene. Passed the delta time since last called.
void Renderer::Render(const Helpers::Camera& camera, float deltaTime)
{			
	// Configure pipeline settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Wireframe mode controlled by ImGui
	if (m_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Clear buffers from previous frame
	glClearColor(0.0f, 0.0f, 0.3f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Compute viewport and projection matrix
	GLint viewportSize[4];
	glGetIntegerv(GL_VIEWPORT, viewportSize);
	const float aspect_ratio = viewportSize[2] / (float)viewportSize[3];
	glm::mat4 projection_xform = glm::perspective(glm::radians(45.0f), aspect_ratio, 0.1f, 1500.0f);

	// Use our program. Doing this enables the shaders we attached previously.
	glUseProgram(m_default_program);

	// Compute camera view matrix and combine with projection matrix for passing to shader
	glm::mat4 view_xform = glm::lookAt(camera.GetPosition(), camera.GetPosition() + camera.GetLookVector(), camera.GetUpVector());
	
	GLuint combined_xform_id;
	glm::mat4 combined_xform;
	glm::mat4 model_xform = glm::mat4(1);

	// Uncomment all the lines below to rotate cube first round y then round x
	static float angle = 0;
	static bool rotateY = true;

	if (rotateY) // Rotate around y axis	
	{
		model_cube.meshes[0]->rotation.y += -0.005;
	}
	else // Rotate around x axis	
	{
		model_cube.meshes[0]->rotation.x += -0.005;
	}


	angle+=0.005f;
	if (angle > glm::two_pi<float>())
	{
		angle = 0;
		rotateY = !rotateY;
	}

	// Send the model matrix to the shader in a uniform

	GLuint model_xform_id;
	GLuint camera_id;

	//-------------------------------LIGHTS-----------------------------------------------------------------------------

	//-------------------------------Directional------------------------------------------------
	GLuint intensity_id = glGetUniformLocation(m_default_program, "lights[0].intensity");
	glUniform3f(intensity_id, 1, 1, 1);

	GLuint posdir_id = glGetUniformLocation(m_default_program, "lights[0].posdir");
	glUniform3f(posdir_id, -0.5f, -0.5f, 0.0f);

	GLuint type_id = glGetUniformLocation(m_default_program, "lights[0].type");
	glUniform1i(type_id, 0);

	//-------------------------------Point------------------------------------------------

	GLuint intensity_id2 = glGetUniformLocation(m_default_program, "lights[1].intensity");
	glUniform3f(intensity_id2, 1, 1, 1);

	GLuint posdir_id2 = glGetUniformLocation(m_default_program, "lights[1].posdir");
	glUniform3f(posdir_id2,20, 10, 20);

	GLuint type_id2 = glGetUniformLocation(m_default_program, "lights[1].type");
	glUniform1i(type_id2, 1);


	//-------------------------------Spot------------------------------------------------

	GLuint intensity_id3 = glGetUniformLocation(m_default_program, "lights[2].intensity");
	glUniform3f(intensity_id3, 1, 1, 1);

	GLuint posdir_id3 = glGetUniformLocation(m_default_program, "lights[2].posdir");
	glUniform3f(posdir_id3, 50, 5, 50);

	GLuint type_id3 = glGetUniformLocation(m_default_program, "lights[2].type");
	glUniform1i(type_id3, 2);


	//-----------------------------Propeller---------------------------------------------------
	model_aquaPig_propeller.meshes[0]->rotation.y += -0.01;

	for (size_t i = 0; i < models.size(); i++)
	{
		if(i == 0)
		{
			glUseProgram(m_unlit_program);

			model_xform_id = glGetUniformLocation(m_unlit_program, "model_xform");
			glUniformMatrix4fv(model_xform_id, 1, GL_FALSE, glm::value_ptr(model_xform));

			view_xform = glm::mat4(glm::mat3(view_xform));
			glDepthMask(GL_FALSE);
			glDisable(GL_DEPTH_TEST);

			combined_xform = projection_xform * view_xform;

			combined_xform_id = glGetUniformLocation(m_unlit_program, "combined_xform");
			glUniformMatrix4fv(combined_xform_id, 1, GL_FALSE, glm::value_ptr(combined_xform));
		}
		else if(i == 1)
		{
			glUseProgram(m_colour_unlit_program);

			model_xform_id = glGetUniformLocation(m_colour_unlit_program, "model_xform");
			glUniformMatrix4fv(model_xform_id, 1, GL_FALSE, glm::value_ptr(model_xform));

			

			glDepthMask(GL_TRUE);
			glEnable(GL_DEPTH_TEST);
			view_xform = glm::lookAt(camera.GetPosition(), camera.GetPosition() + camera.GetLookVector(), camera.GetUpVector());

			combined_xform = projection_xform * view_xform;

			combined_xform_id = glGetUniformLocation(m_colour_unlit_program, "combined_xform");
			glUniformMatrix4fv(combined_xform_id, 1, GL_FALSE, glm::value_ptr(combined_xform));
		}
		else
		{
			glUseProgram(m_default_program);

			model_xform_id = glGetUniformLocation(m_default_program, "model_xform");
			glUniformMatrix4fv(model_xform_id, 1, GL_FALSE, glm::value_ptr(model_xform));



			glDepthMask(GL_TRUE);
			glEnable(GL_DEPTH_TEST);
			view_xform = glm::lookAt(camera.GetPosition(), camera.GetPosition() + camera.GetLookVector(), camera.GetUpVector());

			combined_xform = projection_xform * view_xform;

			combined_xform_id = glGetUniformLocation(m_default_program, "combined_xform");
			glUniformMatrix4fv(combined_xform_id, 1, GL_FALSE, glm::value_ptr(combined_xform));

			camera_id = glGetUniformLocation(m_default_program, "camera_pos");
			glUniform3f(camera_id, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		}

		for (size_t j = 0; j < models[i].meshes.size(); j++)
		{
			glm::mat4 model_xform = glm::mat4(1);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, models[i].meshes[j]->texture);

			if(i == 0)
			{
				glUniform1i(glGetUniformLocation(m_unlit_program, "sampler_tex"), 0);
			}
			else if(i > 1)
			{
				glUniform1i(glGetUniformLocation(m_default_program, "sampler_tex"), 0);
			}
			
			glBindVertexArray(models[i].meshes[j]->vao);

			//std::cout << "rotx: " << models[i].meshes[j]->rotation.x << std::endl;
			//std::cout << "roty: " << models[i].meshes[j]->rotation.y << std::endl;

			model_xform = glm::translate(model_xform, models[i].meshes[j]->translation);
			
			model_xform = glm::rotate(model_xform, models[i].meshes[j]->rotation.x, glm::vec3{ 1, 0, 0 });
			model_xform = glm::rotate(model_xform, models[i].meshes[j]->rotation.y, glm::vec3{ 0, 1, 0 });
			model_xform = glm::rotate(model_xform, models[i].meshes[j]->rotation.z, glm::vec3{ 0, 0, 1 });

			glUniformMatrix4fv(model_xform_id, 1, GL_FALSE, glm::value_ptr(model_xform));

			glDrawElements(GL_TRIANGLES, models[i].meshes[j]->numElements, GL_UNSIGNED_INT, (void*)0);
		}
	}
}

