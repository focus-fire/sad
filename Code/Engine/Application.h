#pragma once 

#include "Window.h"

struct SDL_Window;

namespace sad
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Start();
		virtual void Teardown();

	private:
		Window* m_Window = nullptr;
	};
}