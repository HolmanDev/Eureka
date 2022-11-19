#pragma once

class VertexBuffer
{
private:
	unsigned int m_RendererID; // An ID of a buffer. //! Is the renderer a buffer?
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};