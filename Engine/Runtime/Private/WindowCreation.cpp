#include "WindowCreation.h"

#ifdef ENGINE_SDL
#ifdef ENGINE_OPENGL
#include "SDL/SDLOpenGLWindowCreation.h"
#include "SDL/SDLOpenGLWindowCreation.cpp"
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
    return SDLOpenGL::CreateWindowAndContext(name, w, h);
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
    SDLOpenGL::DestroyContextAndWindow();
    #else
    #error "Missing SDL implementation for Window Creation!"
    #endif
    #else
    #error "Missing implementation for Window Creation!"
    #endif

  }
  void Present(bool vsync)
  {
    #ifdef ENGINE_SDL
    #ifdef ENGINE_OPENGL
    SDLOpenGL::SwapBuffers(vsync);
    #else
    #error "Missing SDL implementation for Window Creation!"
    #endif
    #else
    #error "Missing implementation for Window Creation!"
    #endif
  }
}
