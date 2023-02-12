#pragma once
#include "ExternalLibraryHeaders.h"
#include "Helper.h"
#include "Mesh.h"

class ElementBuffer
{
private:

	GLuint m_buffer;

public:
	ElementBuffer(const void* data, GLuint size);
	~ElementBuffer();

	void Bind();
	void UnBind();
};

