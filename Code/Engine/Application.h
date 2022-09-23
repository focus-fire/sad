#pragma once 

struct SDL_Window;

namespace sad
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Start();
		virtual void End();

	private:
		SDL_Window* m_Window = nullptr;
	};
}