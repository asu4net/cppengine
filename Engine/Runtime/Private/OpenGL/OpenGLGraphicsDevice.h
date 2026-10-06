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

    // @Note: This will remain unimplemented, since OpenGL can
    // throw this messages via debug callback.
    void DumpDebugMessages() override {}

    void ClearBackBuffer(float r = 0, float g = 0, float b = 0) override;

    // @Pending: Implement this stuff for OpenGL.
    void SetGraphicsState(const GraphicsState& state) override {}
    void ImmediateDraw() override {}

  private:
#if ENGINE_SDL
    SDL_GLContext m_SDL_GLContext;
#endif
    GraphicsHandle m_SwapChainHandle;
    OpenGLSwapChain* m_SwapChain = nullptr;
};

