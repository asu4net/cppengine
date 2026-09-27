#include "WindowCreation.h"

#ifdef ENGINE_SDL
#ifdef ENGINE_OPENGL
#include "SDL/SDL_OpenGL_WindowCreation.h"
#include "SDL/SDL_OpenGL_WindowCreation.cpp"
#elif  ENGINE_D3D11
#include "SDL/SDL_D3D11_WindowCreation.h"
#include "SDL/SDL_D3D11_WindowCreation.cpp"
#else
#error "Missing SDL implementation for Window Creation!"
#endif
#else
#error "Missing implementation for Window Creation!"
#endif

namespace WindowCreation
{
  bool InitWindow(std::string_view name, std::uint32_t w, std::uint32_t h)
  {
    #ifdef ENGINE_SDL
    #ifdef ENGINE_OPENGL
    return SDL_OpenGL::InitWindow(name, w, h);
    #elif  ENGINE_D3D11
    return SDL_D3D11::InitWindow(name, w, h);
    #else
    #error "Missing SDL implementation for Window Creation!"
    #endif
    #else
    #error "Missing implementation for Window Creation!"
    #endif
  }
  void DeinitWindow()
  {
    #ifdef ENGINE_SDL
    #ifdef ENGINE_OPENGL
    SDL_OpenGL::DeinitWindow();
    #elif  ENGINE_D3D11
    SDL_D3D11::DeinitWindow();
    #else
    #error "Missing SDL implementation for Window Creation!"
    #endif
    #else
    #error "Missing implementation for Window Creation!"
    #endif

  }
  void* GetHandle()
  {
    #ifdef ENGINE_SDL
    #ifdef ENGINE_OPENGL
    return SDL_OpenGL::GetHandle();
    #elif  ENGINE_D3D11
    return SDL_D3D11::GetHandle();
    #else
    #error "Missing SDL implementation for Window Creation!"
    #endif
    #else
    #error "Missing implementation for Window Creation!"
    #endif
  }
}
