#include "Skybox.h"
#include "ImageLoader.h"

void Skybox::LoadSkybox()
{

	Helpers::ImageLoader imgLoader;
	imgLoader.Load(textureFilePath);

	GLuint height = imgLoader.Height();
	GLuint width = imgLoader.Width();

	//-------------------------------------------------------------------------------------------------------------------------------------------
	GLuint positionsVBO;
	glGenBuffers(1, &positionsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size() * 3, vertices.data(), GL_STATIC_DRAW);

	//----------------------------------TEXTURE_COORDINATES_BUFFER---------------------------------------------------------------
	GLuint texcoordVBO;
	glGenBuffers(1, &texcoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, texcoordVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * uvCoords.size() * 2, uvCoords.data(), GL_STATIC_DRAW);

	//----------------------------------ELEMENTS_BUFFER--------------------------------------------------------------------------
	GLuint elementBuffer;
	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * elements.size(), elements.data(), GL_STATIC_DRAW);

	//----------------------------------GEN_MESH_VAO----------------------------------------------------------------------------
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
	glVertexAttribPointer // Define Layout
	(
		0,// attrib num
		3,// size in components
		GL_FLOAT,//type
		GL_FALSE,//normalised or not
		0, // stride
		(void*)0 // offset
	);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, texcoordVBO);
	glVertexAttribPointer// Define Layout
	(
		1,// attrib num
		2,// size in components
		GL_FLOAT,//type
		GL_FALSE,//normalised or not
		0, // stride
		(void*)0 // offset
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBindVertexArray(0);

	//----------------------------------LOAD_TEXTURE_INTO_TEXURE_BUFFER----------------------------------------------------------
	//glGenTextures(1, &texture);
	//glBindTexture(GL_TEXTURE_2D, texture);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgLoader.GetData());
	//glGenerateMipmap(GL_TEXTURE_2D);

	//glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	//glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, );

	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}
