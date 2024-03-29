#include "Input.h"

namespace Zote
{
	GLFWwindow* Input::m_window;

	void Input::Init(GLFWwindow* window)
	{
		m_window = window;
	}

	bool Input::GetKeyPressed(int keycode)
	{
		int state = glfwGetKey(m_window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::GetKeyReleased(int keycode)
	{
		int state = glfwGetKey(m_window, keycode);
		return state == GLFW_RELEASE;
	} 

	bool Input::GetMouseButtonPressed(int button)
	{
		int state = glfwGetMouseButton(m_window, button);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	glm::vec2 Input::GetMousePosition()
	{
		double x, y;
		glfwGetCursorPos(m_window, &x, &y);
		return vec2((float)x, (float)y);
		return vec2();
	}
}
