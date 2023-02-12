#pragma once
#include "ExternalLibraryHeaders.h"
#include "Helper.h"
#include "Mesh.h"

struct Mesh
{
	GLuint vao;
	GLuint texture;
	GLuint colour;
	GLuint numElements;

	glm::vec3 translation = glm::vec3(0 ,0 ,0);
	glm::vec3 rotation = glm::vec3(0, 0, 0);
};

class Model
{
public:
	std::vector<Mesh*> meshes;
	bool LoadMeshes(std::string modelFilePath, std::vector<std::string> textureFilePaths, std::vector<glm::vec3> translations, std::vector<glm::vec3> rotations);
	bool LoadMeshColour(std::vector<glm::vec3> vertices, std::vector<glm::vec3> colours, std::vector<GLuint> elements, glm::vec3 translation = glm::vec3{ 0,0,0 }, glm::vec3 rotation = glm::vec3{ 0,0,0 });
	bool LoadMesh(std::vector<glm::vec3> positions, std::vector<GLuint> elements, std::vector<glm::vec2> uvCoords, std::vector<glm::vec3> normals, std::vector<std::string> textureFilePath, std::vector<glm::vec3> translations, std::vector<glm::vec3> rotations);
private:

};

