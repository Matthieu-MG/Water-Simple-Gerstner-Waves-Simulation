#pragma once

#include <glad/glad.h>
#include "src/Texture/Texture.h"

enum  FBTarget
{
	COLOR, DEPTH, STENCIL, DEPTH_STENCIL
};

struct FBTargetData
{
	int internal_format;
	int format;
	int attachment_type;

	FBTargetData(int int_format, int format, int attachment)
	{
		internal_format = int_format;
		this->format = format;
		attachment_type = attachment;
	}
};

class FrameBuffer
{

public:
	FrameBuffer(unsigned int Res_x, unsigned int Res_y);
	void AttachTexture(FBTarget target);
	void AttachRenderObject(FBTarget target);
	inline void Bind() const { glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID); }
	inline void Unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

	inline unsigned int GetTextureAttachmentID(int index) const { return m_TextureAttachmentsID[index]; }
	inline unsigned int GetWidth() const { return m_ResX; }
	inline unsigned int GetHeight() const { return m_ResY; }

private:
	FBTargetData GetFrameBufferTextureTarget(FBTarget target);
	unsigned int m_RendererID;
	unsigned int m_TextureAttachmentsID[4];
	unsigned int m_ResX;
	unsigned int m_ResY;
};