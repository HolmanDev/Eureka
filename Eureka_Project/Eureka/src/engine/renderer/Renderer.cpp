#include "pch.h"
#include "Renderer.h"

#include "engine/core/WindowInfo.h"
#include "engine/core/Time.h"
#include "engine/renderer/effects/ssao.h"

std::unique_ptr<Renderer::SceneData> Renderer::s_SceneData = std::make_unique<Renderer::SceneData>();
std::vector<Model> Renderer::s_RenderQueue;
std::vector<UI> Renderer::s_UIRenderQueue;;
std::unique_ptr<SSAO> Renderer::s_SSAO = nullptr;
bool Renderer::s_VisibleColliders = false;

void Renderer::Init()
{
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glEnable(GL_CULL_FACE));
	GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
	RenderCommand::Clear();
}

void Renderer::Close()
{
	s_SSAO.reset();
}

void Renderer::Render()
{
	Init();

	// Generate the necessary data. DRAW FROM QUEUE
	s_SSAO->Run(s_RenderQueue, s_SceneData->vpMatrix);

	// Activate default frame buffer and access depth data
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	RenderCommand::Clear();
	s_SSAO->BindDepth();

	// Draw scene again but with correct materials
	for (const Model& model : s_RenderQueue)
	{
		model.Material->UpdateMaterial(); // Select
		std::shared_ptr<Shader> shader = model.Material->GetShader();

		shader->Bind(); //! Andra gången på denna
		shader->SetUniformMat4f("u_ViewProj", s_SceneData->vpMatrix);
		shader->SetUniformMat4f("u_Transform", model.Transform);
		shader->SetUniform1i("u_DepthTexture", 1);
		shader->SetUniform1f("u_Time", Time::GetTime());
		RenderCommand::Draw(model.VertexArray);
		shader->Unbind();
	}

	s_RenderQueue.clear();
}

//! Switch to rasterized rectangles
void Renderer::RenderUI()
{
	// Bind frame buffer 0 as draw
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	// Loop through UI elements
	for (const UI& uiElement : s_UIRenderQueue)
	{
		//std::shared_ptr<Shader> shader = uiElement.Shader;
		glBindFramebuffer(GL_READ_FRAMEBUFFER, uiElement.Framebuffer);
		float aspectRatio = uiElement.TextureSize.x / uiElement.TextureSize.y;
		float x = uiElement.Pos.x * WindowInfo::height * aspectRatio;
		float y = uiElement.Pos.y * WindowInfo::height;
		float width = uiElement.Size.x * WindowInfo::height * aspectRatio;
		float height = uiElement.Size.y * WindowInfo::height;
		glBlitFramebuffer(0, 0, (int) uiElement.TextureSize.x, (int) uiElement.TextureSize.y, 
			(int) x, (int) y, (int)round(width), (int)round(height), GL_COLOR_BUFFER_BIT, GL_LINEAR);
	}

	// Unbind and clear
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	s_UIRenderQueue.clear();
}

void Renderer::BeginScene(Camera& camera)
{
	s_SceneData->vpMatrix = camera.GetProjectionMatrix() * camera.GetViewMatrix();
}

void Renderer::EndScene()
{
}

void Renderer::Submit(const std::shared_ptr<Material>& material, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
{
	Model model = { material, vertexArray, transform };
	s_RenderQueue.push_back(model);
}

void Renderer::SubmitUI(const unsigned int framebuffer, const glm::vec2 pos, const glm::vec2 size, glm::vec2 textureSize)
{
	UI ui = { framebuffer, pos, size, textureSize};
	s_UIRenderQueue.push_back(ui);
}

void Renderer::EnableVisibleColliders()
{
	s_VisibleColliders = true;
}

void Renderer::DisableVisibleColliders()
{
	s_VisibleColliders = false;
}

void Renderer::SetSSAO(std::unique_ptr<SSAO> ssao)
{
	s_SSAO = std::move(ssao);
}
