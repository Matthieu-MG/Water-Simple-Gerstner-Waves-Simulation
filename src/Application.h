#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

struct COLOR
{
	float Red;
	float Green;
	float Blue;

	COLOR() = default;
	COLOR(float red, float green, float blue)
	{
		this->Red = red;
		this->Green = green;
		this->Blue = blue;
	};
};


// Singleton
class Application
{
public:
	// Delete copy constructor
	Application(const Application&) = delete;

	static Application& Get();
	void Init(const int width, const int height, const char* windowName);

	unsigned int GetStatus();
	GLFWwindow* GetWindow();
	inline unsigned int GetWidth() { return m_Width; }
	inline unsigned int GetHeight() { return m_Height; }
	// DEPTH TEST
	void EnableDepthTest();
	void DisableDepthTest();
	// BLENDING
	inline void EnableBlending() const
	{
		glEnable(GL_BLEND);
	}
	inline void BlendFunc(const int src_fac, const int dst_fac) const
	{
		glBlendFunc(src_fac, dst_fac);
	}
	// FACE CULLING
	inline void EnableFaceCulling() const { glEnable(GL_CULL_FACE); }
	inline void CullFace(const int FACE) const { glCullFace(FACE); }
	inline void SetFaceCullWindingOrder(const int WO) const { glFrontFace(WO); }
	// STENCIL TEST
	void EnableStencilBuffer();
	void SetStencilMask(int a);
	void StencilFunc(int func, int ref, int mask);
	void StencilOp(int sfail, int dfail, int dpass);

	void UpdateAndClearBuffers();
	void CloseWindow(bool state);
	void SetBackgroundColor(float r, float g, float b);

private:
	Application() {}
	~Application();

	GLFWwindow* m_window;
	GLbitfield m_BufferBit;
	unsigned int m_Width;
	unsigned int m_Height;
	unsigned int m_status;
	COLOR m_backgroundColor;
private:
	static void ViewportResizeCallback(GLFWwindow* window, int width, int height);
};

#endif