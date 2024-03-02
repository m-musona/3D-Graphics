#include "MeshComponent.h"

#include "../Actor.h"
#include "../Game.h"

#include "../Renderer/Shader.h"
#include "../Renderer/Mesh.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/Texture.h"
#include "../Renderer/VertexArray.h"

MeshComponent::MeshComponent(Actor* owner)
	:Component(owner),
	mMesh(nullptr),
	mTextureIndex(0),
	mVisible(true)
{
	mOwner->GetGame()->GetRenderer()->AddMeshComp(this);
}

MeshComponent::~MeshComponent()
{
	mOwner->GetGame()->GetRenderer()->RemoveMeshComp(this);
}

void MeshComponent::Draw(Shader* shader)
{
	if (mMesh)
	{
		// Set the world transform
		shader->SetMatrixUniform("uWorldTransform", mOwner->GetWorldTransform());

		// Set specular power
		shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());

		// Set the active texture
		Texture* texture = mMesh->GetTexture(mTextureIndex);
		if (texture) 
		{ 
			texture->SetActive(); 
		}

		// Set the mesh's vertex array as active
		VertexArray* vertexArray = mMesh->GetVertexArray();
		vertexArray->SetActive();

		// Draw
		glDrawElements(GL_TRIANGLES, vertexArray->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}
