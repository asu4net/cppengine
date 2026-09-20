#include "SDLOpenGLWindowCreation.h"
#include "SDL3/SDL.h"
#include "glad/glad.h" 

namespace SDLOpenGL
{
  static constexpr std::int32_t c_Major = 4;
  static constexpr std::int32_t c_Minor = 6;
  static constexpr bool c_UseDoubleBuffer = true;

  static SDL_Window* s_Window = nullptr;
  static SDL_GLContext s_Context;

  bool CreateWindowAndContext(std::string_view name, std::uint32_t w, std::uint32_t h)
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

    s_Context = SDL_GL_CreateContext(s_Window);
    if (!s_Context)
    {
      LOG_ERR("SDL_GL_CreateContext(): {}", SDL_GetError());
      SDL_DestroyWindow(s_Window);
    }

    if (!SDL_GL_MakeCurrent(s_Window, s_Context)) 
    {
      LOG_ERR("SDL_GL_MakeCurrent(): {}", SDL_GetError());
      SDL_DestroyWindow(s_Window);
      SDL_Quit();
      return false;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
      LOG_ERR("Failed to initialize glad!");
      DestroyContextAndWindow();
      return false;
    }
    return true;
  }

  void DestroyContextAndWindow()
  {
    SDL_GL_DestroyContext(s_Context);
    SDL_DestroyWindow(s_Window);
    SDL_Quit();
    s_Context = nullptr;
    s_Window = nullptr;
  }

  void SwapBuffers(bool vsync)
  {
    SDL_GL_SwapWindow(s_Window);
  }
}
