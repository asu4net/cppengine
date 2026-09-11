#include "Runtime/Application.h"
#include "SDL3/SDL.h"

void Application::Init()
{
  SDL_Init(SDL_INIT_VIDEO);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  SDL_Window* window = SDL_CreateWindow(
      "SDL3 + OpenGL",
      1280,
      720,
      SDL_WINDOW_OPENGL
      );

  SDL_GLContext context = SDL_GL_CreateContext(window);

  bool running = true;

  OnInit();

  while (running)
  {
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_EVENT_QUIT)
        running = false;
    }

    //glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT);

    SDL_GL_SwapWindow(window);
  }

  SDL_GL_DestroyContext(context);
  SDL_DestroyWindow(window);
  SDL_Quit();
};

void Application::Exit()
{
  OnExit();
};

void Application::OnInit()
{
  std::print("Application init! \n");
}

void Application::OnExit()
{
  std::print("Application exit! \n");
}
