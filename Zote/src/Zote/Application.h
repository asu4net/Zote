#pragma once

#include "Core.h"

namespace Zote
{
	class ZOTE_API Application
	{
	public:
		Application();
		virtual ~Application();
		virtual void Run();
	};

	//To be defined in the client
	Application* CreateApplication();
}