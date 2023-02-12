#include "Model.h"
#include "ImageLoader.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"

bool Model::LoadMeshes(std::string modelFilePath, std::vector<std::string> textureFilePath, std::vector<glm::vec3> translations, std::vector<glm::vec3> rotations)
{
	// Load in the jeep
	Helpers::ModelLoader loader;

	if (!loader.LoadFromFile(modelFilePath))
	{
		std::cout << "exit!";
		return false;
	}

	int i = 0;

	// Now we can loop through all the mesh in the loaded model:
	for (const Helpers::Mesh& mesh : loader.GetMeshVector())
	{
		//----------------------------------CREATE_MESH----------------------------------------------------------------------------
		Mesh* newMesh = new Mesh();
		newMesh->numElements = mesh.elements.size();

		if(translations.size() != loader.GetMeshVector().size()) // INIT TRANSLATIONS PER MESH
		{
			newMesh->translation = glm::vec3{ 0,0,0 };
		}
		else
		{
			newMesh->translation = translations[i];
		}
		
		if(rotations.size() != loader.GetMeshVector().size()) // INIT ROTATIONS PER MESH
		{
			newMesh->rotation = glm::vec3{ 0,0,0 };
		}
		else
		{
			newMesh->rotation = rotations[i];
		}

		//----------------------------------POSITIONS_BUFFER------------------------------------------------------------------------
		VertexBuffer posVb(mesh.vertices.data(), sizeof(GLfloat) * mesh.vertices.size() * 3);

		//----------------------------------TEXTURE_COORDINATES_BUFFER---------------------------------------------------------------
		VertexBuffer texcoordVb(mesh.uvCoords.data(), sizeof(GLfloat) * mesh.uvCoords.size() * 2);

		//----------------------------------NORMALS_BUFFER------------------------------------------------------------------------
		VertexBuffer normals (mesh.normals.data(), sizeof(GLfloat) * mesh.normals.size() * 3);

		//----------------------------------ELEMENTS_BUFFER--------------------------------------------------------------------------
		ElementBuffer elementBuffer(mesh.elements.data(), sizeof(GLuint) * mesh.elements.size());

		//----------------------------------GEN_MESH_VAO----------------------------------------------------------------------------
		glGenVertexArrays(1, &newMesh->vao);
		glBindVertexArray(newMesh->vao);

		glEnableVertexAttribArray(0);
		posVb.Bind();
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(1);
		texcoordVb.Bind();
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(2);
		normals.Bind();
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		elementBuffer.Bind();
		glBindVertexArray(0);

	//----------------------------------LOAD_TEXTURE_INTO_TEXURE_BUFFER----------------------------------------------------------
		Helpers::ImageLoader imgLoader;
		imgLoader.Load(textureFilePath[i]);

		GLuint height = imgLoader.Height();
		GLuint width = imgLoader.Width();

		glGenTextures(1, &newMesh->texture);
		glBindTexture(GL_TEXTURE_2D, newMesh->texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgLoader.GetData());
		glGenerateMipmap(GL_TEXTURE_2D);

		//----------------------------------PUSHBACK_NEWMESH--------------------------------------------------------------------------
		meshes.push_back(newMesh);
		i++;
	}
	return true;
}

bool Model::LoadMesh(std::vector<glm::vec3> vertices, std::vector<GLuint> elements, std::vector<glm::vec2> uvCoords, std::vector<glm::vec3> normals, std::vector<std::string> textureFilePath, std::vector<glm::vec3> translations, std::vector<glm::vec3> rotations)
{
	int i = 0;

	//----------------------------------CREATE_MESH----------------------------------------------------------------------------
	Mesh* newMesh = new Mesh();
	newMesh->numElements = elements.size();

	if (translations.size() != 1) // INIT TRANSLATIONS
	{
		newMesh->translation = glm::vec3{ 0,0,0 };
	}
	else
	{
		newMesh->translation = translations[i];
	}

	if (rotations.size() != 1) // INIT ROTATIONS
	{
		newMesh->rotation = glm::vec3{ 0,0,0 };
	}
	else
	{
		newMesh->rotation = rotations[i];
	}

	//----------------------------------POSITIONS_BUFFER------------------------------------------------------------------------
	VertexBuffer posVb(vertices.data(), sizeof(GLfloat) * vertices.size() * 3);

	//----------------------------------TEXTURE_COORDINATES_BUFFER---------------------------------------------------------------
	VertexBuffer texcoordVb(uvCoords.data(), sizeof(GLfloat) * uvCoords.size() * 2);

	//----------------------------------NORMALS_BUFFER------------------------------------------------------------------------
	VertexBuffer norms(normals.data(), sizeof(GLfloat) * normals.size() * 3);

	//----------------------------------ELEMENTS_BUFFER--------------------------------------------------------------------------
	ElementBuffer elementBuffer(elements.data(), sizeof(GLuint) * elements.size());

	//----------------------------------GEN_MESH_VAO----------------------------------------------------------------------------
	glGenVertexArrays(1, &newMesh->vao);
	glBindVertexArray(newMesh->vao);

	glEnableVertexAttribArray(0);
	posVb.Bind();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	texcoordVb.Bind();
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(2);
	norms.Bind();
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	elementBuffer.Bind();
	glBindVertexArray(0);

	//----------------------------------LOAD_TEXTURE_INTO_TEXURE_BUFFER----------------------------------------------------------
	Helpers::ImageLoader imgLoader;
	imgLoader.Load(textureFilePath[i]);

	GLuint height = imgLoader.Height();
	GLuint width = imgLoader.Width();

	glGenTextures(1, &newMesh->texture);
	glBindTexture(GL_TEXTURE_2D, newMesh->texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgLoader.GetData());
	glGenerateMipmap(GL_TEXTURE_2D);

	//----------------------------------PUSHBACK_NEWMESH--------------------------------------------------------------------------
	meshes.push_back(newMesh);
	i++;

	return true;
}

bool Model::LoadMeshColour(std::vector<glm::vec3> vertices, std::vector<glm::vec3> colours, std::vector<GLuint> elements, glm::vec3 translation, glm::vec3 rotation)
{
	//----------------------------------CREATE_MESH----------------------------------------------------------------------------
	Mesh* newMesh = new Mesh();
	newMesh->numElements = elements.size();

	newMesh->translation = translation;
	newMesh->rotation = rotation;
	
	//----------------------------------POSITIONS_BUFFER------------------------------------------------------------------------
	VertexBuffer posVb(vertices.data(), sizeof(GLfloat) * vertices.size() * 3);

	//----------------------------------COLOUR_BUFFER------------------------------------------------------------------------
	VertexBuffer colourVb(colours.data(), sizeof(GLfloat) * vertices.size() * 3);

	//----------------------------------ELEMENTS_BUFFER--------------------------------------------------------------------------
	ElementBuffer elementBuffer(elements.data(), sizeof(GLuint) * elements.size());

	//----------------------------------GEN_MESH_VAO----------------------------------------------------------------------------
	glGenVertexArrays(1, &newMesh->vao);
	glBindVertexArray(newMesh->vao);

	glEnableVertexAttribArray(0);
	posVb.Bind();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	colourVb.Bind();
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	elementBuffer.Bind();
	glBindVertexArray(0);

	//----------------------------------PUSHBACK_NEWMESH--------------------------------------------------------------------------
	meshes.push_back(newMesh);

	return true;
}