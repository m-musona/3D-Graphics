#include "VertexArray.h"
#include "glad/glad.h"

VertexArray::VertexArray(const float* verts, unsigned int numVerts, const unsigned int* indices, unsigned int numIndices)
	: mNumVerts(numVerts),
	mNumIndices(numIndices)
{
	// Create vertex array object and store its id in mVertexArray
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	// Create vertex buffer
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER /* Means Use As Vertex Buffer */, mVertexBuffer);

	// Copy verts into vertex buffer
	glBufferData(
		GL_ARRAY_BUFFER,				// the active buffer type to write to
		numVerts * 8 * sizeof(float),	// number of bytes to copy
		verts,							// Source to copy from (pointer)
		GL_STATIC_DRAW					// How will we use this data
	);

	// Create index buffer
	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER /* Means Use As Index Buffer */, mIndexBuffer);

	// Copy indices into index buffer
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,			// the active buffer type to write to
		numIndices * sizeof(unsigned int),	// number of bytes to copy
		indices,							// Source to copy from (pointer)
		GL_STATIC_DRAW						// How will we use this data
	);

	// SPECIFY VERTEX LAYOUT

	// POSITION IS 3 FLoatas
	// Enable first vertex attribute
	glEnableVertexAttribArray(0);
	// Specify size, type and formaat of the attribute
	glVertexAttribPointer(
		0,					// Attribute index (0 for the first one)
		3,					// Number of components (3 in this case)
		GL_FLOAT,			// Type of components
		GL_FALSE,			// (Only used for integral types)
		sizeof(float) * 8,	// Stride (usually size of each vertex)
		0					// Offset from start of vertex to this attribute
	);

	// NORMAL IS 3 floats
	// Enable first vertex attribute
	glEnableVertexAttribArray(1);
	// Specify size, type and formaat of the attribute
	glVertexAttribPointer(
		1,											// Attribute index
		3,											// Number of components (2 because UV)
		GL_FLOAT,									// Type of components
		GL_FALSE,									// (Only used for integral types)
		sizeof(float) * 8,							// Stride (usually size of each vertex)
		reinterpret_cast<void*>(sizeof(float) * 3)	// Offset pointer
	);

	// TEXTURE COORDINATES IS 2 floats
	// Enable first vertex attribute
	glEnableVertexAttribArray(2);
	// Specify size, type and formaat of the attribute
	glVertexAttribPointer(
		2,											// Attribute index
		2,											// Number of components (2 because UV)
		GL_FLOAT,									// Type of components
		GL_FALSE,									// (Only used for integral types)
		sizeof(float) * 8,							// Stride (usually size of each vertex)
		reinterpret_cast<void*>(sizeof(float) * 6)	// Offset pointer
	);
}

VertexArray::~VertexArray()
{
	// Destroy vertex buffer, index buffer, vertex array object
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteBuffers(1, &mIndexBuffer);
	glDeleteVertexArrays(1, &mVertexArray);
}

void VertexArray::SetActive()
{
	//Set Vertex Array currently useing
	glBindVertexArray(mVertexArray);
}

