#include "Input.h"

GLFWwindow* Input::m_window = nullptr;
unsigned char Input::m_PressedFlags[];

//// CLEAN UP THE FUNCTIONS IsKeyPressed and GetKeyIndex
//// TRY TO FIND BETTER WAY TO STORE STATES OF KEYS

void Input::Init(GLFWwindow* window)
{
	m_window = window;
	SetInputReleaseCallback();
	m_PressedFlags[R] = 0;
	m_PressedFlags[D] = 0;
}

bool Input::IsKeyPressed(int keycode)
{
	int action = glfwGetKey(m_window, keycode);
	if (action == GLFW_PRESS)
	{
		unsigned char key = GetKeyIndex(keycode);
		if (key != -1)
		{
			// If key is pressed already return
			if (m_PressedFlags[key] == 1)
				action = 0;
			else
				m_PressedFlags[key] = 1;
		}
	}
	return action == GLFW_PRESS;
}

bool Input::IsKeyHeld(int keycode)
{
	int action = glfwGetKey(m_window, keycode);
	return action == GLFW_PRESS || action == GLFW_REPEAT;
}

unsigned char Input::GetKeyIndex(int keycode)
{
	switch (keycode)
	{
	case GLFW_KEY_R:
		return R;
	case GLFW_KEY_D:
		return D;
	default:
		return -1;
	}
}

void Input::SetInputReleaseCallback()
{
	glfwSetKeyCallback(m_window, KeyCallback);
}

void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_RELEASE)
	{
		if (key == GLFW_KEY_R || key == GLFW_KEY_D)
		{
			m_PressedFlags[GetKeyIndex(key)] = 0;
		}
	}
}
