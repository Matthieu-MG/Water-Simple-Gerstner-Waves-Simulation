#pragma once

#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>
#include <iostream>

#define LOG(x) std::cout << x << std::endl

enum KEYS
{
	R=0, D=1, EMPTY=-1	
};

class Input
{
public:
	static void Init(GLFWwindow* window);
	static bool IsKeyPressed(int keycode);
	static bool IsKeyHeld(int keycode);

private:
	static unsigned char m_PressedFlags[2];
	static GLFWwindow* m_window;
	static void SetInputReleaseCallback();
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static unsigned char GetKeyIndex(int keycode);
};

#endif // !INPUT_H