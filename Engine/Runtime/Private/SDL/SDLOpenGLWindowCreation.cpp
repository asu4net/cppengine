#include "SDLOpenGLWindowCreation.h"
#include "SDL3/SDL.h"
#include "glad/glad.h" 

namespace SDLOpenGL
{
  static constexpr std::int32_t c_Major = 4;
  static constexpr std::int32_t c_Minor = 6;
  static constexpr bool c_UseDoubleBuffer = true;

  static SDL_Window* s_Window = nullptr;

  bool InitWindow(std::string_view name, std::uint32_t w, std::uint32_t h)
  {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, c_Major);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, c_Minor);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, static_cast<int>(c_UseDoubleBuffer));

    s_Window = SDL_CreateWindow(
      name.data(),
      static_cast<int>(w),
      static_cast<int>(h),
      SDL_WINDOW_OPENGL
    );

    if (!s_Window)
    {
      LOG_ERR("SDL_CreateWindow(): {}", SDL_GetError());
      SDL_Quit();
      return false;
    }
  }

  void DeinitWindow()
  {
    SDL_DestroyWindow(s_Window);
    SDL_Quit();
    s_Window = nullptr;
  }

  void* GetHandle()
  {
    return s_Window;
  }

  // @Pending: Move this to the SwapChain object.
  void SwapBuffers(bool vsync)
  {
    SDL_GL_SwapWindow(s_Window);
  }
}
