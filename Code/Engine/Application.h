#pragma once 

#include <Editor/Editor.h>

#include <glm/glm.hpp>

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
		
		// TODO: Abstract view projection matrix into camera
		static glm::mat4 GetViewProjectionMatrix();

	private:
		sad::rad::Renderer* m_Renderer = nullptr;
		cap::Editor* m_Editor = nullptr;
	};
}