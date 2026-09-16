#include "Runtime/Application.h"
#include "SDL3/SDL.h"
#include "glad/gl.h" 

// Test code for graphis storage.
#include "Runtime/VertexBuffer.h"
#include "Runtime/Shader.h"

void Application::Init()
{
  SDL_Init(SDL_INIT_VIDEO);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  SDL_Window* window = SDL_CreateWindow(
      "SDL3 + OpenGL",
      1280,
      720,
      SDL_WINDOW_OPENGL
      );

  SDL_GLContext context = SDL_GL_CreateContext(window);

  if (!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress))
  {
    LOG_ERR("Failed to initialize glad!");
    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return;
  }

  bool running = true;

  OnInit();

  // Test code for graphis storage.
  VertexBufferParams params;
  auto handle = m_GraphicsStorage.Emplace<VertexBuffer>(params);
  VertexBuffer* vb = m_GraphicsStorage.Get<VertexBuffer>(handle);
  m_GraphicsStorage.IsValid<VertexBuffer>(handle);

  ShaderParams sparams;
  m_GraphicsStorage.Emplace<Shader>(sparams);

  while (running)
  {
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_EVENT_QUIT)
        running = false;
    }

    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

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
  LOG_INFO("Application init!");
}

void Application::OnExit()
{
  LOG_INFO("Application exit!");
}
