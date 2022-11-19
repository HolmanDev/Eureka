#include "pch.h"
#include "Framebuffer.h"

#include "Renderer.h"

Framebuffer::Framebuffer()
    : m_ColorAttachment(0), m_DepthAttachment(0)
{
    glCreateFramebuffers(1, &m_RendererID);
}

Framebuffer::~Framebuffer()
{
	GLCall(glDeleteFramebuffers(1, &m_RendererID))
}

void Framebuffer::Bind() const
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
}

void Framebuffer::Unbind() const
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

unsigned int Framebuffer::GenColorAttachment(unsigned int width, unsigned int height)
{
    unsigned int attachment;
    glCreateTextures(GL_TEXTURE_2D, 1, &attachment);
    glBindTexture(GL_TEXTURE_2D, attachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // For downscaling
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    return attachment;
}

unsigned int Framebuffer::GenDepthAttachment(unsigned int width, unsigned int height)
{
    unsigned int attachment;
    glCreateTextures(GL_TEXTURE_2D, 1, &attachment);
    glBindTexture(GL_TEXTURE_2D, attachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // For downscaling.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    return attachment;
}

void Framebuffer::AddColorAttachment(unsigned int attachment)
{
    m_ColorAttachment = attachment;
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer is incomplete!" << std::endl;
}

void Framebuffer::AddDepthAttachment(unsigned int attachment)
{
    m_DepthAttachment = attachment;
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer is incomplete!" << std::endl;
}
