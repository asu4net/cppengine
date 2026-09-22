#pragma once

#include "Runtime/GraphicsDevice.h"

#if ENGINE_SDL
#include "SDL3/SDL.h"
#else
#error "Missing OS Context wrapper implementation for OpenGL."
#endif

class OpenGLGraphicsDevice : public GraphicsDevice
{
  public:
    OpenGLGraphicsDevice(const GraphicsDeviceParams& params);
    ~OpenGLGraphicsDevice();

    OpenGLGraphicsDevice(const OpenGLGraphicsDevice&) = delete;
    OpenGLGraphicsDevice& operator=(const OpenGLGraphicsDevice&) noexcept = delete;
    OpenGLGraphicsDevice(OpenGLGraphicsDevice&&) = delete;
    OpenGLGraphicsDevice& operator=(OpenGLGraphicsDevice&&) noexcept = delete;

  private:
#if ENGINE_SDL
    SDL_GLContext m_SDL_GLContext;
#endif
};

