#pragma once

#include "Runtime/GraphicsDevice.h"

#if ENGINE_SDL
#include "SDL3/SDL.h"
#else
#error "Missing OS Context wrapper implementation for OpenGL."
#endif

class OpenGLSwapChain;

class OpenGLGraphicsDevice : public GraphicsDevice
{
  public:
    OpenGLGraphicsDevice(const GraphicsDeviceParams& params);
    ~OpenGLGraphicsDevice();

    OpenGLGraphicsDevice(const OpenGLGraphicsDevice&) = delete;
    OpenGLGraphicsDevice& operator=(const OpenGLGraphicsDevice&) noexcept = delete;
    OpenGLGraphicsDevice(OpenGLGraphicsDevice&&) = delete;
    OpenGLGraphicsDevice& operator=(OpenGLGraphicsDevice&&) noexcept = delete;

    SwapChain& GetSwapChain() override;

    void SetSwapChain(GraphicsHandle swapChainHandle, OpenGLSwapChain* swapChain) 
    { 
      m_SwapChainHandle = swapChainHandle;
      m_SwapChain = swapChain;
    };

  private:
#if ENGINE_SDL
    SDL_GLContext m_SDL_GLContext;
#endif
    GraphicsHandle m_SwapChainHandle;
    OpenGLSwapChain* m_SwapChain = nullptr;
};

