#include "pch.h"
#include "Invention.h"

#include "engine/world/GameObject.h"
#include "engine/renderer/Geometry.h"
#include "engine/events/EventManager.h"
#include "engine/concepts/AssetManager.h"
#include "engine/core/Time.h"
#include "game/inventions/parts/pEmpty.h"
#include "game/player/Player.h"

Invention::Invention()
	: m_Key(-1), m_Player(nullptr)
{
}

Invention::~Invention()
{
}

std::string Invention::Identify()
{
	return typeid(*this).name();
}

void Invention::Update()
{
	Run();
}

void Invention::SetKey(int key)
{
	m_Key = key;
}

void Invention::SetPlayer(Player* player)
{
	m_Player = player;
}

void Invention::AddPart(std::shared_ptr<Part> part)
{
	AddToPartList(part);
	part->SetInventionKey(m_Key);
	glm::vec3 position = part->GetPosition();
	int x = (int) round(position.x);
	int y = (int) round(position.y);
	int z = (int) round(position.z);
	if (m_PartSpace.InRange(x, y, z))
	{
		Part*& oldPart = m_PartSpace.Access(x, y, z);
		if (oldPart)
		{
			if (oldPart->GetPartType() == "empty")
			{
				delete oldPart;
				oldPart = nullptr;
			}
		}
	}
	m_PartSpace.Set(part.get(), x, y, z);
	part->OnPlace(this);
}

void Invention::AddToPartList(std::shared_ptr<Part> part)
{
	m_Parts.push_back(part);
}

void Invention::RemovePart(std::shared_ptr<Part> part_ptr)
{
	glm::vec3 position = part_ptr->GetPosition();
	m_PartSpace.Set(new pEmpty(), (int) round(position.x), (int) round(position.y), (int) round(position.z));
	auto end = std::remove_if(m_Parts.begin(), m_Parts.end(), [&](std::shared_ptr<Part> part) { return part == part_ptr; });
	m_Parts.resize(end - m_Parts.begin());
	part_ptr->OnRemove(this);
	part_ptr->SetInventionKey(-1);
}

std::shared_ptr<Part> Invention::GetPartWithCollider(Collider* collider)
{
	for (const std::shared_ptr<Part>& part : m_Parts)
	{
		if (part->GetHitBox().get() == collider)
			return part;
	}
	return nullptr;
}

Part* Invention::SamplePartSpace(Grid3Pos position)
{
	return m_PartSpace.Access(position);
}

void Invention::Run()
{
	for(auto&& part : m_Parts) 
	{
		part->Action(this);
	}
}

// Generate a unified mesh for the invention based on its parts
std::shared_ptr<Mesh> Invention::GenerateMesh()
{	
	std::shared_ptr<Shader> shader = AssetManager::GetShader("Standard");
	std::shared_ptr<Material> material = std::make_shared<Material>(shader);
	material->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	Texture texture("res/textures/Atlas.png", { GL_NEAREST, GL_CLAMP_TO_EDGE });
	material->SetTexture(&texture);

	if(!m_Mesh)
		m_Mesh = std::make_shared<Mesh>();
	//! Add support for different types of meshes
	int numVertices = 24;
	int numIndices = 12 * 3;
	int numUVs = 24;
	int numColors = 24;
	int numNormals = 24;
	int partCount = m_Parts.size();
	int totalVertexCount = partCount * numVertices;
	int totalIndexCount = partCount * numIndices;
	int totalUVCount = partCount * numUVs;
	int totalColorsCount = partCount * numColors;
	int totalNormalCount = partCount * numNormals;
	float* vertices = new float[3 * totalVertexCount];
	unsigned int* indices = new unsigned int[totalIndexCount];
	float* uvs = new float[2 * totalUVCount];
	float* colors = new float[3 * totalColorsCount];
	float* normals = new float[3 * totalNormalCount];
	int partIndex = 0;
	for (auto&& part : m_Parts)
	{
		float verticesTemp[3 * 24];
		std::copy(std::begin(Geometry::BoxVertices), std::end(Geometry::BoxVertices), std::begin(verticesTemp));
		unsigned int indicesTemp[3 * 12];
		std::copy(std::begin(Geometry::BoxIndices), std::end(Geometry::BoxIndices), std::begin(indicesTemp));
		float* UVsTemp = part->GetUVs();
		float colorsTemp[3 * 24] = {0.0};
		float normalsTemp[3 * 24];
		std::copy(std::begin(Geometry::BoxNormals), std::end(Geometry::BoxNormals), std::begin(normalsTemp));

		float sizeFactor = 1.0f / (pow(8.0f, part->GetSize() - 1.0f));
		glm::vec3 position = part->GetPosition();
		for (int i = 0; i < numVertices; i++) {			
			int beginIndex = i * 3;
			//! Remember, this only works because it is in local space
			verticesTemp[beginIndex] *= sizeFactor;
			verticesTemp[beginIndex + 1] *= sizeFactor;
			verticesTemp[beginIndex + 2] *= sizeFactor;
			verticesTemp[beginIndex] += position.x;
			verticesTemp[beginIndex+1] += position.y;
			verticesTemp[beginIndex+2] += position.z;
		}

		for (int i = 0; i < numIndices; i++) {
			indicesTemp[i] += partIndex * numVertices;
		}

		glm::vec3 color = part->GetColor();
		for (int i = 0; i < numColors; i++) {
			int beginIndex = i * 3;
			colorsTemp[beginIndex] = color.r;
			colorsTemp[beginIndex + 1] = color.g;
			colorsTemp[beginIndex + 2] = color.b;
		}

		std::copy(verticesTemp, verticesTemp + 3 * numVertices, vertices + partIndex * 3 * numVertices);
		std::copy(indicesTemp, indicesTemp + numIndices, indices + partIndex * numIndices);
		std::copy(UVsTemp, UVsTemp + 2 * numUVs, uvs + partIndex * 2 * numUVs);
		std::copy(colorsTemp, colorsTemp + 3 * numColors, colors + partIndex * 3 * numColors);
		std::copy(normalsTemp, normalsTemp + 3 * numNormals, normals + partIndex * 3 * numNormals); //!
		partIndex++;
		// Delete temps?
	}
	m_Mesh->SetVertices(vertices, totalVertexCount); //! Memory leak?
	m_Mesh->SetIndices(indices, totalIndexCount);
	m_Mesh->SetUVs(uvs, totalUVCount);
	m_Mesh->SetColors(colors, totalColorsCount);
	m_Mesh->SetNormals(normals, totalNormalCount);
	m_Mesh->SetMaterial(material);
	m_Mesh->UpdateMesh();

	return m_Mesh;
}

// Generate colliders for all parts of invention
std::vector<std::shared_ptr<MeshCollider>> Invention::GenerateColliders()
{
	if (auto gameObject = GetGameObject().lock())
	{
		for (std::shared_ptr<MeshCollider> collider : m_Colliders)
		{
			gameObject->RemoveAttribute(collider);
		}
	}
	m_Colliders.clear();
	for (const std::shared_ptr<Part>& part : m_Parts)
	{
		std::shared_ptr<MeshCollider> collider = std::make_shared<MeshCollider>();
		m_Colliders.push_back(collider);
		float* vertices = new float[3 * 24];
		unsigned int vertexCount = 24;
		std::copy(std::begin(Geometry::BoxVertices), std::end(Geometry::BoxVertices), vertices);
		unsigned int* indices = new unsigned int[3 * 12];
		unsigned int indexCount = 3 * 12;
		std::copy(std::begin(Geometry::BoxIndices), std::end(Geometry::BoxIndices), indices);

		collider->center = part->GetPosition();

		collider->SetVertices(vertices, vertexCount);
		collider->SetIndices(indices, indexCount);
		part->SetHitBox(collider);
	}
	// ApplyColliders() //! ?
	return m_Colliders;
}

// Apply locally cached mesh data to mesh attribute attatched to gameObject
void Invention::ApplyMesh()
{
	if (auto gameObject = GetGameObject().lock()) {
		std::shared_ptr<Mesh> mesh = gameObject->GetAttribute<Mesh>();
		if (mesh)
		{
			*mesh = *m_Mesh;
		}
		else
		{
			gameObject->AddAttribute(m_Mesh);
		}			
	}
}

// Apply locally cached collider data to collider attributes attatched to gameObject
void Invention::ApplyColliders()
{
	if (auto gameObject = GetGameObject().lock()) {
		for (std::shared_ptr<MeshCollider> collider : m_Colliders)
		{
			gameObject->AddAttribute(collider);
		}
	}
}
