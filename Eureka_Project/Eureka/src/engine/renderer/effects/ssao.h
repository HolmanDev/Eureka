#pragma once

#include "engine/renderer/Material.h"
#include "engine/renderer/Framebuffer.h"
#include "engine/renderer/Renderer.h"

class SSAO
{
private:
	std::unique_ptr<Framebuffer> m_Framebuffer;
public:
	SSAO(unsigned int width, unsigned int height);
	~SSAO();

	inline const Framebuffer& GetFramebuffer() { return *m_Framebuffer; } //! const?

	void Run(const std::vector<Model>& renderQueue, const glm::mat4 vp);
	void BindColor();
	void BindDepth();
};