#pragma once

#include "engine/renderer/VertexArray.h"
#include "engine/renderer/VertexBuffer.h"
#include "engine/renderer/IndexBuffer.h"
#include "engine/renderer/Material.h"
#include "engine/concepts/Attribute.h"

class Mesh : public Attribute
{
private:
	std::shared_ptr<VertexArray> m_VAO;
	std::shared_ptr<VertexBuffer> m_VertexBuffer;
	std::shared_ptr<IndexBuffer> m_IndexBuffer;
	unsigned int m_VertexCount = 0;
	float* m_Vertices; // Should these be arrays instead?
	unsigned int m_UVCount = 0;
	float* m_UVs;
	unsigned int m_ColorsCount = 0;
	float* m_Colors;
	unsigned int m_NormalCount = 0;
	float* m_Normals;
	unsigned int m_IndexCount = 0;
	unsigned int* m_Indices;
	std::shared_ptr<Material> m_Material; //! This shouldn't be here
	float* m_Data;
public:
	Mesh();
	~Mesh();
	//! Implement good copy constructor
	
	std::string Identify() override;

	inline const std::shared_ptr<VertexArray> GetVertexArray() const { return m_VAO; }
	inline const std::shared_ptr<VertexBuffer> GetVertexBuffer() const { return m_VertexBuffer; }
	inline const std::shared_ptr<IndexBuffer> GetIndexBuffer() const { return m_IndexBuffer; }
	inline const std::shared_ptr<Material> GetMaterial() const { return m_Material; }
	inline float* const GetVertices() const { return m_Vertices; }
	inline const unsigned int GetVertexCount() const { return m_VertexCount; }
	inline unsigned int* const GetIndices() const { return m_Indices; }
	inline const unsigned int GetIndexCount() const { return m_IndexCount; }
	inline float* const GetUVs() const{ return m_UVs; }
	inline const unsigned int GetUVCount() const { return m_UVCount; }
	inline float* const GetColors() const { return m_Colors; }
	inline const unsigned int GetColorsCount() const { return m_ColorsCount; }
	inline float* const GetNormals() const { return m_Normals; }
	inline const unsigned int GetNormalCount() const { return m_NormalCount; }

	void SetVertices(float* const vertices, const unsigned int vertexCount);
	void SetIndices(unsigned int* const indices, const unsigned int indexCount);
	void SetUVs(float* const UVs, const unsigned int uvCount);
	void SetColors(float* const colors, const unsigned int colorsCount);
	void SetNormals(float* const normals, const unsigned int normalCount);
	void SetMaterial(std::shared_ptr<Material> material);
	
	void Clear();
	void UpdateMesh();
};