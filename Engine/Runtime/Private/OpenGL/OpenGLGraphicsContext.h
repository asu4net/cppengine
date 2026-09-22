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
};

