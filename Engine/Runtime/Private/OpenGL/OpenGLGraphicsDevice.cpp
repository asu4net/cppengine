#include "OpenGLGraphicsDevice.h"

OpenGLGraphicsDevice::OpenGLGraphicsDevice(const GraphicsDeviceParams& params)
{
  ASSERT(params.windowHandle != nullptr);
  SDL_Window* window = reinterpret_cast<SDL_Window*>(params.windowHandle);
  SDL_GLContext context = SDL_GL_CreateContext(window);

  if (!context)
  {
    LOG_ERR("SDL_GL_CreateContext(): {}", SDL_GetError());
    std::exit(EXIT_FAILURE);
  }

  if (!SDL_GL_MakeCurrent(window, context)) 
  {
    LOG_ERR("SDL_GL_MakeCurrent(): {}", SDL_GetError());
    std::exit(EXIT_FAILURE);
  }

  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
  {
    LOG_ERR("Failed to initialize glad!");
    std::exit(EXIT_FAILURE);
  }

  m_SDL_GLContext = context;
  LOG_INFO("SDL OpenGL Context created!");
}

OpenGLGraphicsDevice::~OpenGLGraphicsDevice()
{
  SDL_GL_DestroyContext(m_SDL_GLContext);
  m_SDL_GLContext = {};
  LOG_INFO("SDL OpenGL Context destroyed!");
}
