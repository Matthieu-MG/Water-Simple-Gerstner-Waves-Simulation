#include "Application.h"

Application& Application::Get()
{
	static Application instance;
	return instance;
}

void Application::Init(const int width,const int height, const char* windowName)
{
	m_BufferBit = GL_COLOR_BUFFER_BIT;
	m_status = 0;
	m_Width = width;
	m_Height = height;
	m_backgroundColor = COLOR(1.0f, 0.45f, 0.8f);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(m_Width, m_Height, windowName, NULL, NULL);
	if (m_window == NULL)
	{
		glfwTerminate();
		m_status = -1;
	}
	glfwMakeContextCurrent(m_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		m_status = -1;
	}

	glfwSetFramebufferSizeCallback(m_window, ViewportResizeCallback);
	glViewport(0, 0, m_Width, m_Height);
}

Application::~Application()
{
	glfwTerminate();
}

unsigned int Application::GetStatus()
{
	return m_status;
}

GLFWwindow* Application::GetWindow()
{
	return m_window;
}

void Application::EnableDepthTest()
{
	glEnable(GL_DEPTH_TEST);
	m_BufferBit |= GL_DEPTH_BUFFER_BIT;
}

void Application::DisableDepthTest()
{
	glDisable(GL_DEPTH_TEST);
	m_BufferBit &= ~GL_DEPTH_BUFFER_BIT;
}

void Application::EnableStencilBuffer()
{
	glEnable(GL_STENCIL_TEST);
	m_BufferBit |= GL_STENCIL_BUFFER_BIT;
}

void Application::SetStencilMask(int a)
{
	glStencilMask(a);
}

void Application::StencilFunc(int func, int ref, int mask)
{
	glStencilFunc(func, ref, mask);
}

void Application::StencilOp(int sfail, int dfail, int dpass)
{
	glStencilOp(sfail, dfail, dpass);
}

void Application::UpdateAndClearBuffers()
{
	glClearColor(m_backgroundColor.Red, m_backgroundColor.Green, m_backgroundColor.Blue, 1.0f);
	glClear(m_BufferBit);
}

void Application::CloseWindow(bool state)
{
	glfwSetWindowShouldClose(m_window ,state);
}

void Application::SetBackgroundColor(float r, float g, float b)
{
	m_backgroundColor.Red = r;
	m_backgroundColor.Green = g;
	m_backgroundColor.Blue = b;
}

void Application::ViewportResizeCallback(GLFWwindow* window, int width, int height)
{
	Application& app = Application::Get();
	glViewport(0, 0, width, height);
	app.m_Height = height;
	app.m_Width = width;
}
