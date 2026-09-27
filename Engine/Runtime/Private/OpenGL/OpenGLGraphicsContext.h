#pragma once

#include "Runtime/GraphicsContext.h"

class OpenGLGraphicsContext : public GraphicsContext
{
  public:
    OpenGLGraphicsContext(const GraphicsContextParams& params);
    ~OpenGLGraphicsContext();

    OpenGLGraphicsContext(const OpenGLGraphicsContext&) = delete;
    OpenGLGraphicsContext& operator=(const OpenGLGraphicsContext&) noexcept = delete;
    OpenGLGraphicsContext(OpenGLGraphicsContext&&) = delete;
    OpenGLGraphicsContext& operator=(OpenGLGraphicsContext&&) noexcept = delete;

    void ClearBackBuffer(SwapChain& swapChain, float r = 0, float g = 0, float b = 0) override;
};

