#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"

class VertexBufferLayout;

class VertexArray 
{
private:
	unsigned int m_RendererID;
	std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
	std::shared_ptr<IndexBuffer> m_IndexBuffer;
	VertexBufferLayout m_Layout;
public:
	VertexArray();
	~VertexArray();

	const VertexBufferLayout& GetLayout() { return m_Layout; }
	void SetLayout(const VertexBufferLayout& layout);

	void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
	void ClearVertexBuffers();
	void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

	void Bind() const;
	void Unbind() const;

	const std::vector<std::shared_ptr<VertexBuffer>> GetVertexBuffers() const { return m_VertexBuffers; }
	const std::shared_ptr<IndexBuffer> GetIndexBuffer() const { return m_IndexBuffer; }
};