#include "core\window.h"
#include"sdl2/SDL.h"
#include"imgui/imgui.h"
#include"imgui/imgui_impl_sdl.h"
#include"imgui/imgui_impl_opengl3.h"
#include<glad/glad.h>
#include<iostream>

namespace core
{
	WindowProperties::WindowProperties()
	{
		title = "MClone!";
		x = SDL_WINDOWPOS_CENTERED;
		y = SDL_WINDOWPOS_CENTERED;
		w = 1024;
		h = 576;
		wMin = 320;
		hMin = 180;
		flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
		aspectRatio = 16.0f / 9.0f;
		clearColor = glm::vec4(153.0 / 255.0, 204.0 / 255.0, 1.0,1.0);
	}
	Window::Window()
		:m_Window(nullptr),
		m_Context(nullptr)
	{}

	void Window::create(const WindowProperties& props)
	{
		m_props = props;
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		{
			std::cout << "error init SDL" << std::endl;
		}

		m_Window = SDL_CreateWindow(props.title.c_str(), props.x, props.y, props.w, props.h,props.flags);
		if (!m_Window)
		{
			std::cout << "error window creation" << std::endl;
		}
		SDL_SetWindowMinimumSize(m_Window, props.wMin, props.hMin);
		
		
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		//SDL_WarpMouseInWindow(m_Window, props.w/2, props.h/2);
		//SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE);
		//SDL_SetRelativeMouseMode(SDL_TRUE);

		m_Context = SDL_GL_CreateContext(m_Window);

		int major, minor;
		SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
		SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &minor);
		std::cout<<"GLcontext version :"<< major<<"."<<minor<<std::endl;

		gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
		glViewport(0, 0, props.w, props.h);
		glClearColor(props.clearColor.r, props.clearColor.g, props.clearColor.b, props.clearColor.a);
		glEnable(GL_DEPTH_TEST);

		ImWindow.create(m_Window, m_Context);
	}

	void Window::shutdown()
	{
		SDL_DestroyWindow(m_Window);
		SDL_GL_DeleteContext(m_Context);
	}

	void Window::setClearColor(glm::vec4 cc)
	{
		m_props.clearColor = cc;
		glClearColor(cc.r, cc.g, cc.b, cc.a);
	}

	void Window::beginRender()
	{
		clearScreen();
		pollEvents();
		ImWindow.beginRender(m_Window);
	}

	void Window::endRender()
	{
		ImWindow.endRender(m_Window, m_Context);
		swapbuffers();
	}

	void Window::clearScreen()
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	}

	void Window::swapbuffers()
	{
		SDL_GL_SwapWindow(m_Window);
	}

	void Window::pollEvents()
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				m_ShouldClose = true;
				break;
			case SDL_KEYDOWN:
			{
				switch (e.key.keysym.scancode)
				{
				case SDL_SCANCODE_ESCAPE:
					m_ShouldClose = true;
					break;
				default:
					break;
				}
				break;
			}
			default:
				break;
			}
			ImWindow.handleSDLEvent(e);
		}
		
	}

	void ImguiWindow::create(SDL_Window* window, SDL_GLContext context)
	{
		IMGUI_CHECKVERSION();

		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		
		ImGui_ImplSDL2_InitForOpenGL(window,context);
		ImGui_ImplOpenGL3_Init("#version 140");
	}

	void ImguiWindow::shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}

	void ImguiWindow::handleSDLEvent(SDL_Event& e)
	{
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	bool ImguiWindow::wantCaptureMouse()
	{
		return ImGui::GetIO().WantCaptureMouse;
	}

	bool ImguiWindow::wantCaptureKeyboard()
	{
		return ImGui::GetIO().WantCaptureKeyboard;
	}

	void ImguiWindow::beginRender(SDL_Window* window)
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();
	}

	void ImguiWindow::endRender(SDL_Window* window,SDL_GLContext context)
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			SDL_GL_MakeCurrent(window, context);
		}
	}

}

