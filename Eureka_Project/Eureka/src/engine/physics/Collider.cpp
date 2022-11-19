#include "pch.h"
#include "Collider.h"

#include "engine/math/math.h"
#include "engine/world/GameObject.h"
#include "engine/core/Time.h"

Collider::Collider()
	: center(glm::vec3(0))
{
}

Collider::~Collider()
{
}

std::string Collider::Identify()
{
	return typeid(*this).name();
}

MeshCollider::MeshCollider() : Collider()
	, m_Vertices(nullptr), m_Indices(nullptr), m_VertexCount(0), m_IndexCount(0)
{
}

MeshCollider::~MeshCollider()
{
	delete[] m_Vertices;
	delete[] m_Indices;
}

std::string MeshCollider::Identify()
{
	return typeid(*this).name();
}

void MeshCollider::SetVertices(float* const vertices, const unsigned int vertexCount)
{
	m_Vertices = vertices;
	m_VertexCount = vertexCount;
}

void MeshCollider::SetIndices(unsigned int* const indices, const unsigned int indexCount)
{
	m_Indices = indices;
	m_IndexCount = indexCount;
}

glm::vec3 MeshCollider::FindFurthestPoint(glm::vec3 direction) const
{
	glm::vec3 maxPoint(0);
	float maxDistance = -std::numeric_limits<float>::max();

	for (unsigned int i = 0; i < m_VertexCount * 3; i+=3)
	{
		glm::vec3 vertex = glm::vec3(m_Vertices[i], m_Vertices[i+1], m_Vertices[i+2]) + center;
		float distance = glm::dot(vertex, direction);
		if (distance > maxDistance)
		{
			maxDistance = distance;
			maxPoint = vertex;
		}
	}
	return maxPoint;
}

RayHit MeshCollider::LineCollision(glm::vec3 start, glm::vec3 end) const
{
	RayHit hit;
	auto gameObject = GetGameObject().lock();
	if (!gameObject) return hit;
	Transform& transform = gameObject->GetTransform();
	glm::vec3 gameObjectPosition = transform.GetPosition();

	auto getVertex = [&](unsigned int i) -> glm::vec3
	{
		unsigned int index = m_Indices[i] * 3;
		return glm::vec3(m_Vertices[index], m_Vertices[index+1], m_Vertices[index+2]) + center + gameObjectPosition;
	};

	float closest = std::numeric_limits<float>::max();
	for (unsigned int i = 0; i < m_IndexCount; i+=3)
	{
		glm::vec3 p1 = getVertex(i);
		glm::vec3 p2 = getVertex(i + 1);
		glm::vec3 p3 = getVertex(i + 2);

		if (Math::LineIntersectTriangle(start, end, p1, p2, p3))
		{
			glm::vec3 normal = glm::cross(p2 - p1, p3 - p1);
			float t = glm::dot(p1 - start, normal) / glm::dot(end - start, normal);
			glm::vec3 position = start + t * (end - start);
			float distance = glm::distance(start, position);
			if (distance < closest)
			{
				closest = distance;
				hit.collided = true;
				hit.position = position;
				hit.distance = distance;
				hit.collider = (MeshCollider*) this; //! Hacky
			}
		}
	}
	return hit;
}
