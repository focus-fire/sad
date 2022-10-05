#pragma once 

#include <Editor/Cap.h>

#include "Window.h"
#include "Renderer/Renderer.h"

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
		sad::rad::Renderer* m_Renderer = nullptr;
		cap::Cap* m_Editor = nullptr;
	};
}