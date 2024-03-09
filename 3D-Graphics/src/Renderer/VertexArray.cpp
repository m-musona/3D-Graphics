#include "VertexArray.h"
#include "glad/glad.h"

VertexArray::VertexArray(const void* verts, unsigned int numVerts, Layout layout, const unsigned int* indices, unsigned int numIndices)
	: mNumVerts(numVerts),
	mNumIndices(numIndices)
{
	// Create vertex array object and store its id in mVertexArray
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	// If layout is PosNormSkinTex then change vertexSize
	unsigned vertexSize = 8 * sizeof(float);
	if (layout == PosNormSkinTex)
	{
		vertexSize = 8 * sizeof(float) + 8 * sizeof(char);
	}

	// Create vertex buffer
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER /* Means Use As Vertex Buffer */, mVertexBuffer);
	// Copy verts into vertex buffer
	glBufferData(
		GL_ARRAY_BUFFER,				// the active buffer type to write to
		numVerts * vertexSize,	// number of bytes to copy
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
	// Specify the vertex attributes
	if (layout == PosNormTex)
	{
		// Position is 3 floats
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);

		// Normal is 3 floats
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 3));

		// Texture coordinates is 2 floats
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6));
	}
	else if (layout == PosNormSkinTex)
	{
		// POSITION IS 3 Floats
		// Enable first vertex attribute
		glEnableVertexAttribArray(0);
		// Specify size, type and format of the attribute
		glVertexAttribPointer(
			0,					// Attribute index (0 for the first one)
			3,					// Number of components (3 in this case)
			GL_FLOAT,			// Type of components
			GL_FALSE,			// (Only used for integral types)
			vertexSize,			// Stride (usually size of each vertex)
			0					// Offset from start of vertex to this attribute
		);

		// NORMAL IS 3 floats
		// Enable first vertex attribute
		glEnableVertexAttribArray(1);
		// Specify size, type and format of the attribute
		glVertexAttribPointer(
			1,											// Attribute index
			3,											// Number of components (2 because UV)
			GL_FLOAT,									// Type of components
			GL_FALSE,									// (Only used for integral types)
			vertexSize,									// Stride (usually size of each vertex)
			reinterpret_cast<void*>(sizeof(float) * 3)	// Offset pointer
		);

		// Skinning indices (keep as ints)
		glEnableVertexAttribArray(2);
		glVertexAttribIPointer(2, 4, GL_UNSIGNED_BYTE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6));

		// Skinning weights (convert to floats)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6 + sizeof(char) * 4));

		// TEXTURE COORDINATES IS 2 floats
		// Enable first vertex attribute
		glEnableVertexAttribArray(4);
		// Specify size, type and format of the attribute
		glVertexAttribPointer(
			4,																// Attribute index
			2,																// Number of components (2 because UV)
			GL_FLOAT,														// Type of components
			GL_FALSE,														// (Only used for integral types)
			vertexSize,														// Stride (usually size of each vertex)
			reinterpret_cast<void*>(sizeof(float) * 6 + sizeof(char) * 8)	// Offset pointer
		);
	}

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

