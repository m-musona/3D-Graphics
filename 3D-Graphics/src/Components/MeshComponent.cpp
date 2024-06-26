#include "MeshComponent.h"

#include "../Actor.h"
#include "../Game.h"
#include "../LevelLoader.h"

#include "../Renderer/Shader.h"
#include "../Renderer/Mesh.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/Texture.h"
#include "../Renderer/VertexArray.h"

MeshComponent::MeshComponent(Actor* owner, bool isSkeletal)
	:Component(owner),
	mMesh(nullptr),
	mTextureIndex(0),
	mVisible(true),
	mIsSkeletal(isSkeletal)
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

void MeshComponent::LoadProperties(const rapidjson::Value& inObj)
{
	Component::LoadProperties(inObj);

	std::string meshFile;
	if (JsonHelper::GetString(inObj, "meshFile", meshFile))
	{
		SetMesh(mOwner->GetGame()->GetRenderer()->GetMesh(meshFile));
	}

	int idx;
	if (JsonHelper::GetInt(inObj, "textureIndex", idx))
	{
		mTextureIndex = static_cast<size_t>(idx);
	}

	JsonHelper::GetBool(inObj, "visible", mVisible);
	JsonHelper::GetBool(inObj, "isSkeletal", mIsSkeletal);
}

void MeshComponent::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	Component::SaveProperties(alloc, inObj);

	if (mMesh)
	{
		JsonHelper::AddString(alloc, inObj, "meshFile", mMesh->GetFileName());
	}
	JsonHelper::AddInt(alloc, inObj, "textureIndex", static_cast<int>(mTextureIndex));
	JsonHelper::AddBool(alloc, inObj, "visible", mVisible);
	JsonHelper::AddBool(alloc, inObj, "isSkeletal", mIsSkeletal);
}
