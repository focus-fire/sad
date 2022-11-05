#pragma once

#include <utility>

#include <SDL2/SDL.h>

namespace sad 
{
	struct WindowProperties 
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;
		float AspectRatio;
		std::pair<unsigned int, unsigned int> MaxDimensions;
		std::pair<unsigned int, unsigned int> MinDimensions;

		WindowProperties(const std::string& title = "sadEngine", 
			unsigned int width = 1600, 
			unsigned int height = 900,
			unsigned int maxWidth = 3840,
			unsigned int maxHeight = 2160,
			unsigned int minWidth = 500,
			unsigned int minHeight = 300)
			: Title(title)
			, Width(width)
			, Height(height)
			, AspectRatio(static_cast<float>(width) / static_cast<float>(height))
			, MaxDimensions(maxWidth, maxHeight)
			, MinDimensions(minWidth, minHeight)
		{ }
	};

	class Window
	{
	public:
		explicit Window(const WindowProperties& properties = WindowProperties());

		void Start();
		void CreateGLContext();
		void Render();
		void Teardown();

		SDL_Window* GetSDLWindow() const { return m_Window; }
		SDL_GLContext GetGLContext() const { return m_GLContext; };


		unsigned int GetWidth() const { return m_Properties.Width; }
		unsigned int GetHeight() const { return m_Properties.Height; }
		float GetAspectRatio() const { return static_cast<float>(m_Properties.Width) / static_cast<float>(m_Properties.Height); }

	private:
		SDL_Window* m_Window = nullptr;
		SDL_GLContext m_GLContext = nullptr;
		WindowProperties m_Properties;
	};
}
