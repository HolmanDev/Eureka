#include "pch.h"
#include "Mesh.h"

#include "engine/renderer/VertexBufferLayout.h"

Mesh::Mesh()
	: m_VAO(std::make_unique<VertexArray>()), m_Vertices(nullptr), m_Indices(nullptr), 
    m_UVs(nullptr), m_Colors(nullptr), m_Normals(nullptr), m_Material(nullptr), m_Data(nullptr)
{
    UpdateMesh();
}

Mesh::~Mesh()
{
    delete[] m_Vertices;
    delete[] m_Indices;
    delete[] m_UVs;
    delete[] m_Colors;
    delete[] m_Normals;
    delete[] m_Data;
}

std::string Mesh::Identify()
{
    return typeid(*this).name();
}

void Mesh::SetVertices(float* const vertices, const unsigned int vertexCount)
{
    if (m_Vertices != nullptr)
        delete[] m_Vertices;
    m_Vertices = vertices;
    m_VertexCount = vertexCount;
}

void Mesh::SetIndices(unsigned int* const indices, const unsigned int indexCount)
{
    if (m_Indices != nullptr)
        delete[] m_Indices;
    m_Indices = indices;
    m_IndexCount = indexCount;
}

void Mesh::SetUVs(float* const UVs, const unsigned int uvCount)
{
    if(m_UVs != nullptr)
        delete[] m_UVs;
    m_UVs = UVs;
    m_UVCount = uvCount;
}

void Mesh::SetColors(float* const colors, const unsigned int colorsCount)
{
    if (m_Colors != nullptr)
        delete[] m_Colors;
    m_Colors = colors;
    m_ColorsCount = colorsCount;
}

void Mesh::SetNormals(float* const normals, const unsigned int normalCount)
{
    if (m_Normals != nullptr)
        delete[] m_Normals;
    m_Normals = normals;
    m_NormalCount = normalCount;
}

void Mesh::SetMaterial(std::shared_ptr<Material> material)
{
    m_Material = material;
}

void Mesh::Clear()
{
    delete[] m_Vertices;
    delete[] m_Indices;
    delete[] m_UVs;
    delete[] m_Colors;
    delete[] m_Normals;
    m_VertexCount = 0;
    m_IndexCount = 0;
    m_UVCount = 0;
    m_ColorsCount = 0;
    m_NormalCount = 0;
    m_Material = nullptr;
    delete[] m_Data;
}

void Mesh::UpdateMesh()
{
    VertexBufferLayout layout;
    layout.Push<float>(3); // Positions
    layout.Push<float>(2); // Texcoords
    layout.Push<float>(3); // Colors
    layout.Push<float>(3); // Normals

    if (m_Data != nullptr)
        delete[] m_Data;
    m_Data = new float[m_VertexCount * 11];
    int iPos = 0;
    int iUV = 0;
    int iColor = 0;
    int iNormal = 0;
    for (unsigned int i = 0; i < m_VertexCount * 11; i+=11) {
        m_Data[i] = m_Vertices[iPos];
        m_Data[i + 1] = m_Vertices[iPos + 1];
        m_Data[i + 2] = m_Vertices[iPos + 2];
        m_Data[i + 3] = m_UVs[iUV];
        m_Data[i + 4] = m_UVs[iUV + 1];
        m_Data[i + 5] = m_Colors[iColor];
        m_Data[i + 6] = m_Colors[iColor + 1];
        m_Data[i + 7] = m_Colors[iColor + 2];
        m_Data[i + 8] = m_Normals[iNormal];
        m_Data[i + 9] = m_Normals[iNormal + 1];
        m_Data[i + 10] = m_Normals[iNormal + 2];
        iPos += 3;
        iUV += 2;
        iColor += 3;
        iNormal += 3;
    }
    m_VertexBuffer = std::make_shared<VertexBuffer>(m_Data, m_VertexCount * 11 * sizeof(float)); // Creates a vertex buffer of the right size
    m_IndexBuffer = std::make_unique<IndexBuffer>(m_Indices, m_IndexCount); // Create index buffer of the right size
    m_VAO->ClearVertexBuffers();
    m_VAO->SetLayout(layout); //! Do this somewhere else
    m_VAO->AddVertexBuffer(m_VertexBuffer);
    m_VAO->SetIndexBuffer(m_IndexBuffer);
}
