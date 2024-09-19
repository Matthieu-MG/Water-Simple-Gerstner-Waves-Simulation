#include "Framebuffer.h"

FrameBuffer::FrameBuffer(unsigned int Res_x, unsigned int Res_y) : m_ResX(Res_x), m_ResY(Res_y)
{
	glGenFramebuffers(1, &m_RendererID);
	for (size_t i = 0; i < 4; i++)
	{
		m_TextureAttachmentsID[i] = 0;
	}
}

void FrameBuffer::AttachTexture(FBTarget target)
{
	FBTargetData data = GetFrameBufferTextureTarget(target);

	// TODO: CHANGE FBTargetData to also contain type -> COLOR: UBYTE, DEPTH: DEPTH GL_UNSIGNED_INT_24_8
	Texture2D ScreenTexture(m_ResX, m_ResY, data.internal_format, data.format);
	ScreenTexture.SetFilters(GL_LINEAR, GL_LINEAR);
	Bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, data.attachment_type, GL_TEXTURE_2D, ScreenTexture.GetRendererID(), 0);
	m_TextureAttachmentsID[target] = ScreenTexture.GetRendererID();

	return;
}

void FrameBuffer::AttachRenderObject(FBTarget target)
{
	FBTargetData data = GetFrameBufferTextureTarget(target);
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);

	// TODO: GET VIEWPORT ESOLUTION FROM APP::WINDOW
	glRenderbufferStorage(GL_RENDERBUFFER, data.internal_format, m_ResX, m_ResY);
	
	Bind();
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, data.attachment_type, GL_RENDERBUFFER, rbo);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	return;
}

FBTargetData FrameBuffer::GetFrameBufferTextureTarget(FBTarget target)
{
	switch (target)
	{
	case COLOR:
		return FBTargetData(GL_RGB, GL_RGB, GL_COLOR_ATTACHMENT0);
	case DEPTH:
		return FBTargetData(GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT);
	case STENCIL:
		return FBTargetData(GL_STENCIL_INDEX, GL_STENCIL_INDEX, GL_STENCIL_ATTACHMENT);
		break;
	case DEPTH_STENCIL:
		// internal format, format, type, attachment type
		return FBTargetData(GL_DEPTH24_STENCIL8, GL_UNSIGNED_INT_24_8, GL_DEPTH_STENCIL_ATTACHMENT);
	}
}
