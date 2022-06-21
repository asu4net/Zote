#pragma once

#include<iostream>
#include <string>
#include<glew.h>
#include<glfw3.h>
#include"Maths/Color.h"
#include "Event.h"
#include "Core.h"

#define LOG(a) std::cout << a << std::endl;

namespace Zote
{
	struct ZOTE_API OnRenderFrameArgs 
	{
		float aspect;
		float deltaTime;
		OnRenderFrameArgs(float aspect, float deltaTime) 
			: aspect(aspect), deltaTime(deltaTime) {}
	};

	class ZOTE_API Window
	{
		const std::string name;
		int width, height;
		const Color color;
		
		GLFWwindow* glfwWindow = 0;
		int bufferWidth = 0;
		int bufferHeight = 0;
		bool initialized = false;
		float deltaTime = 0;
		float lastTime = 0;

		static const int glMinorVersion;
		static const int glMajorVersion;

		void SetProperties();
		void CalculateDeltaTime();

	public:
		Event<OnRenderFrameArgs> OnRenderFrame;

		Window();
		Window(const std::string name, int width, int height, const Color color);
		~Window();

		bool Init();
		void StartLoop();

		int GetBufferWidth() const { return bufferWidth; }
		int GetBufferHeight() const { return bufferHeight; }
		float GetAspect() const { return ((float) bufferWidth / (float) bufferHeight); }

		static void APIENTRY OnDebugMessage(GLenum source, GLenum type, unsigned int id, GLenum severity,
			GLsizei length, const char* message, const void* userParam);
	};
}

