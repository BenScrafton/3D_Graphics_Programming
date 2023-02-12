#pragma once
#include "ExternalLibraryHeaders.h"
#include "Helper.h"

#include "Camera.h"
#include "Model.h"

class Renderer
{
private:
	// Program object - to host shaders
	GLuint m_default_program{ 0 };
	GLuint m_unlit_program{ 0 };
	GLuint m_colour_unlit_program{ 0 };

	// Vertex Array Object to wrap all render settings
	GLuint m_VAO{ 0 };

	// Number of elments to use when rendering
	GLuint m_numElements{ 0 };

	bool m_wireframe{ false };

	std::vector<Model> models; 

	GLuint CreateProgram(std::string vertexPath, std::string fragmentPath);
public:
	Renderer();
	~Renderer();

	// Draw GUI
	void DefineGUI();

	// Create and / or load geometry, this is like 'level load'
	bool InitialiseGeometry();

	float Noise(int x, int y);
	float CosineLerp(float a, float b, float x);
	void PerlinNoise(std::vector<glm::vec3>& vertices, std::vector<glm::vec3> vertsOrig, int xLength);

	// Render the scene
	void Render(const Helpers::Camera& camera, float deltaTime);
};

