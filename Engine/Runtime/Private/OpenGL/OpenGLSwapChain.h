#pragma once

#include "Runtime/SwapChain.h"

#if ENGINE_SDL
#include "SDL3/SDL.h"
#else
#error "Missing OS SwapBuffers wrapper implementation for OpenGL."
#endif

class OpenGLSwapChain : public SwapChain
{
  public:
    OpenGLSwapChain(const SwapChainParams& params);
    ~OpenGLSwapChain();

    OpenGLSwapChain(const OpenGLSwapChain&) = delete;
    OpenGLSwapChain& operator=(const OpenGLSwapChain&) noexcept = delete;
    OpenGLSwapChain(OpenGLSwapChain&&) = delete;
    OpenGLSwapChain& operator=(OpenGLSwapChain&&) noexcept = delete;

    void SetWindowHandle(void* windowHandle);

    void Present(bool vsync = false) override;
  private:
#if ENGINE_SDL
    SDL_Window* m_WindowHandle = nullptr;
#endif
};
