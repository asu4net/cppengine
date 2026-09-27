#include "OpenGLGraphicsContext.h"

OpenGLGraphicsContext::OpenGLGraphicsContext(const GraphicsContextParams& params)
{
  LOG_INFO("OpenGL Context created!");
}

OpenGLGraphicsContext::~OpenGLGraphicsContext()
{
  LOG_INFO("OpenGL Context destroyed!");
}

void OpenGLGraphicsContext::ClearBackBuffer(SwapChain& swapChain, float r, float g, float b)
{
  // @Note: OpenGL is in Peaceful mode.
  UNUSED(swapChain);
  glClearColor(r, g, b, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}
