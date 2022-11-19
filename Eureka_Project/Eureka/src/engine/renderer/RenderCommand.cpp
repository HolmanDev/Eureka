#include "pch.h"
#include "RenderCommand.h"

#include "engine/glCore.h"

#include "VertexArray.h"

void RenderCommand::Clear() 
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void RenderCommand::Draw(std::shared_ptr<VertexArray> vertexArray) {
    vertexArray->Bind();
    std::shared_ptr<IndexBuffer> indexBuffer = vertexArray->GetIndexBuffer();
    GLCall(glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr));
    vertexArray->Unbind();
}