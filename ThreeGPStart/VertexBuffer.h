#pragma once
#include "ExternalLibraryHeaders.h"
#include "Helper.h"
#include "Mesh.h"

class VertexBuffer
{
private:

	GLuint m_buffer;

public:
	VertexBuffer(const void* data, GLuint size);
	~VertexBuffer();

	void Bind();
	void UnBind();
};

