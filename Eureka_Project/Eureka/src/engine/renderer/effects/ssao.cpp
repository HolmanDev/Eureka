#include "pch.h"
#include "ssao.h"

#include "engine/glCore.h"

#include "engine/core/WindowInfo.h"
#include "engine/concepts/AssetManager.h"

SSAO::SSAO(unsigned int width, unsigned int height)
    : m_Framebuffer(nullptr)
{
    m_Framebuffer = std::make_unique<Framebuffer>();
	m_Framebuffer->Bind();
	unsigned int colorAttachment = m_Framebuffer->GenColorAttachment(WindowInfo::width, WindowInfo::height);
	m_Framebuffer->AddColorAttachment(colorAttachment);
	unsigned int depthAttachment = m_Framebuffer->GenDepthAttachment(WindowInfo::width, WindowInfo::height);
	m_Framebuffer->AddDepthAttachment(depthAttachment);
	m_Framebuffer->Unbind();
}

SSAO::~SSAO()
{
}

void SSAO::Run(const std::vector<Model>& renderQueue, const glm::mat4 viewProjectionMatrix)
{
	m_Framebuffer->Bind();
	glClearColor(1.0f, 0.0f, 1.0f, 1.0f); //! hmmm
	RenderCommand::Clear();

	// Draw scene with replacement shader
	std::shared_ptr<Shader> replacementShader_ptr = AssetManager::GetShader("ReplacementShader");
	Shader& replacementShader = *replacementShader_ptr;
	replacementShader.Bind();
	replacementShader.SetUniformMat4f("u_ViewProj", viewProjectionMatrix);
	replacementShader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
	for (const Model& model : renderQueue)
	{
		replacementShader.SetUniformMat4f("u_Transform", model.Transform);
		RenderCommand::Draw(model.VertexArray);
	}
	replacementShader.Unbind();

	m_Framebuffer->Unbind();
}

void SSAO::BindColor()
{
	GLCall(glActiveTexture(GL_TEXTURE0 + 1));
	GLCall(glEnable(GL_TEXTURE_2D));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_Framebuffer->GetColorAttachmentRendererID()));
}

void SSAO::BindDepth()
{
	GLCall(glActiveTexture(GL_TEXTURE0 + 1));
	GLCall(glEnable(GL_TEXTURE_2D));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_Framebuffer->GetDepthAttachmentRendererID()));
}
