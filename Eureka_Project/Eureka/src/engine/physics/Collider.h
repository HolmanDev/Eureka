#pragma once

#include "engine/concepts/Attribute.h"
#include "Ray.h"

struct Collider : public Attribute
{
	glm::vec3 center;

	Collider();
	virtual ~Collider();

	std::string Identify() override;

	virtual glm::vec3 FindFurthestPoint(glm::vec3 direction) const = 0;
	virtual RayHit LineCollision(glm::vec3 start, glm::vec3 end) const = 0;
};

struct MeshCollider : public Collider
{
private:
	unsigned int m_VertexCount;
	float* m_Vertices;
	unsigned int m_IndexCount;
	unsigned int* m_Indices;
public:
	MeshCollider();
	~MeshCollider();

	std::string Identify() override;

	float* GetVertices() const { return m_Vertices; }
	unsigned int GetVertexCount() const { return m_VertexCount; }
	unsigned int* GetIndices() const { return m_Indices; }
	unsigned int GetIndexCount() const { return m_IndexCount; }

	void SetVertices(float* const vertices, const unsigned int vertexCount);
	void SetIndices(unsigned int* const indices, const unsigned int indexCount);

	glm::vec3 FindFurthestPoint(glm::vec3 direction) const override;
	RayHit LineCollision(glm::vec3 start, glm::vec3 end) const override;
};