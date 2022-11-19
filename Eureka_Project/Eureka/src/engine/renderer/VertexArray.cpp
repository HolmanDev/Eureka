#include "pch.h"
#include "VertexArray.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID)); // Vertex Array Objects are basically geometry objects
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::SetLayout(const VertexBufferLayout& layout)
{
	m_Layout = layout;
}

// Bind buffers to the vertex buffer, and by doing so also to the vertex array object
void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) //! This shouldn't be called too many times
{
	Bind();
	vertexBuffer->Bind();

	const auto& elements = m_Layout.GetElements();
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i)); // Needed to interface with the shader
		GLCall(glVertexAttribPointer(i, element.count, element.type, //! It knows to add the data to this vao because this vao is bound
			element.normalized, m_Layout.GetStride(), (const void*)element.offset));
	}

	m_VertexBuffers.push_back(vertexBuffer);
}

void VertexArray::ClearVertexBuffers()
{
	m_VertexBuffers.clear();
}

void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{
	Bind();
	indexBuffer->Bind();

	m_IndexBuffer = indexBuffer;
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
