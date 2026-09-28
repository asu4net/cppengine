#include "OpenGLGraphicsDevice.h"

#ifdef CONFIG_DEBUG
  static void APIENTRY OpenGLDebugCallback(
      GLenum source, 
      GLenum type, 
      GLuint id, 
      GLenum severity, 
      GLsizei length, 
      const GLchar* message, 
      const void* userParam
  )
  {
    LOG_INFO("[OpenGL Debug] {}", message);
  }
#endif

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
  LOG_INFO("SDL OpenGL Device Context created!");

#ifdef CONFIG_DEBUG
    using cstring = const char*;
    cstring vendor   = reinterpret_cast<cstring>(glGetString(GL_VENDOR));
    cstring renderer = reinterpret_cast<cstring>(glGetString(GL_RENDERER));
    cstring version  = reinterpret_cast<cstring>(glGetString(GL_VERSION));
    cstring shading  = reinterpret_cast<cstring>(glGetString(GL_SHADING_LANGUAGE_VERSION));

    LOG_INFO("OpenGL Vendor:   {}", vendor);
    LOG_INFO("OpenGL Renderer: {}", renderer);
    LOG_INFO("OpenGL Version:  {}", version);
    LOG_INFO("GLSL Version:    {}", shading);

    LOG_INFO("OpenGL Debug callback registered!");
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(OpenGLDebugCallback, nullptr);
#endif
}

OpenGLGraphicsDevice::~OpenGLGraphicsDevice()
{
  ASSERT(m_SwapChain != nullptr);
  if (m_SwapChain != nullptr)
  {
    auto& graphicsStorage = Application::GetInstance().GetGraphicsStorage();
    graphicsStorage.Remove<SwapChain>(m_SwapChainHandle);
    m_SwapChainHandle = {};
    m_SwapChain = nullptr;
  }
  SDL_GL_DestroyContext(m_SDL_GLContext);
  m_SDL_GLContext = {};
  LOG_INFO("SDL OpenGL Device Context destroyed!");
}

SwapChain& OpenGLGraphicsDevice::GetSwapChain()
{
  ASSERT(m_SwapChain != nullptr);
  return *m_SwapChain;
}
