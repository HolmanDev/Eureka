#pragma once

class Framebuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_ColorAttachment, m_DepthAttachment;
public:
	Framebuffer();
	~Framebuffer();

	void Bind() const;
	void Unbind() const;

	const unsigned int GetID() const { return m_RendererID; }
	const unsigned int GetColorAttachmentRendererID() const { return m_ColorAttachment; }
	const unsigned int GetDepthAttachmentRendererID() const { return m_DepthAttachment; }

	unsigned int GenColorAttachment(unsigned int width, unsigned int height);
	unsigned int GenDepthAttachment(unsigned int width, unsigned int height);
	void AddColorAttachment(unsigned int attachment);
	void AddDepthAttachment(unsigned int attachment);
};