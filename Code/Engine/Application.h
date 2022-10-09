#pragma once 

#include <Editor/Editor.h>

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

	public:
		static Window* s_MainWindow;

	private:
		sad::rad::Renderer* m_Renderer = nullptr;
		cap::Editor* m_Editor = nullptr;
	};
}