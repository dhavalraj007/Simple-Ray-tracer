#pragma once
#include<string>
#include<glm/glm.hpp>
#include"sdl2/SDL.h"

typedef struct SDL_Window SDL_Window;
typedef void* SDL_GLContext;
namespace core
{
	class ImguiWindow
	{
	public:
		ImguiWindow() {}
		~ImguiWindow() {}

		void create(SDL_Window* window, SDL_GLContext context);
		void shutdown();

		void handleSDLEvent(SDL_Event& e);

		bool wantCaptureMouse();
		bool wantCaptureKeyboard();

		void beginRender(SDL_Window* window);
		void endRender(SDL_Window* window, SDL_GLContext context);
	};
	struct WindowProperties
	{
		std::string title;
		int x, y, w, h;
		int wMin, hMin;
		float aspectRatio;
		int flags;
		glm::vec4 clearColor;
		WindowProperties();
	};

	class Window
	{
	public:
		Window();
		~Window() {}
		void create(const WindowProperties& props);
		void shutdown();
		void setClearColor(glm::vec4 cc);
		void beginRender();
		void endRender();
		void clearScreen();
		void swapbuffers();
		void pollEvents();
		inline SDL_Window* getSDLWindow() { return m_Window; }
		inline SDL_GLContext getGLContext() { return m_Context; }
		inline WindowProperties getProps() const { return m_props; }
		bool m_ShouldClose = false;
	private:
		SDL_Window* m_Window;
		SDL_GLContext m_Context;
		WindowProperties m_props;
		ImguiWindow ImWindow;
	}; 
	
	
}