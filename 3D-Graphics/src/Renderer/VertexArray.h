#pragma once


class VertexArray
{
public:
	// Different supported vertex layouts
	enum Layout
	{
		PosNormTex,
		PosNormSkinTex
	};

	VertexArray(const void* verts, unsigned int numVerts, Layout layout, const unsigned int* indices, unsigned int numIndices);
	~VertexArray();

	// Activate this vertex array (so we can draw it)
	void SetActive();

	unsigned int GetNumIndices() const { return mNumIndices; }
	unsigned int GetNumVerts() const { return mNumVerts; }

	static unsigned int GetVertexSize(VertexArray::Layout layout);

private:
	// How many vertices in the vertex buffer
	unsigned int mNumVerts;

	// How many indices in the index buffer
	unsigned int mNumIndices;

	// OpenGL ID of the Vertex buffer
	unsigned int mVertexBuffer;

	// OpenGL ID of the Index buffer
	unsigned int mIndexBuffer;

	// OpenGl Id of the Vertex Array Object
	unsigned int mVertexArray;
};
