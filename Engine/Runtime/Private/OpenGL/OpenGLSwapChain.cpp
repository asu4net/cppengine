#include "OpenGLSwapChain.h"

OpenGLSwapChain::OpenGLSwapChain(const SwapChainParams& params)
{
  LOG_INFO("OpenGL Swap Chain created!");
}

OpenGLSwapChain::~OpenGLSwapChain()
{
  LOG_INFO("OpenGL Swap Chain destroyed!");
}

void OpenGLSwapChain::SetWindowHandle(void* windowHandle)
{
  // @Note: We trust :)
  ASSERT(windowHandle != nullptr);
  m_WindowHandle = reinterpret_cast<SDL_Window*>(windowHandle);
}

void OpenGLSwapChain::Present(bool vsync)
{
  ASSERT(m_WindowHandle != nullptr);
  if (m_WindowHandle == nullptr)
  {
    LOG_ERR("SDL OpenGL error: Can't present due to missing window handle!");
    return;
  }

  SDL_GL_SetSwapInterval(vsync ? 1 : 0);
  SDL_GL_SwapWindow(m_WindowHandle);
}
