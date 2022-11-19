#include "pch.h"
#include "VertexBuffer.h"

#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    GLCall(glGenBuffers(1, &m_RendererID)); // Generates one buffer
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID)); // Binds buffer to the OpenGL state machine. buffer is now in the GL_ARRAY_BUFFER slot
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW)); // Gives position data to the buffer, but OpenGL doesn't know how to interpret it. Does the data ever get deleted?
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}