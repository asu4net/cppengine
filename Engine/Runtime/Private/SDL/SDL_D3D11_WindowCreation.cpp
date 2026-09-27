#include "SDL_D3D11_WindowCreation.h"
#include "SDL3/SDL.h"

#define WIN32_MEAN_AND_LEAN
#include <Windows.h>

namespace SDL_D3D11
{
  static SDL_Window* s_Window = nullptr;

  bool InitWindow(std::string_view name, std::uint32_t w, std::uint32_t h)
  {
    SDL_Init(SDL_INIT_VIDEO);

    s_Window = SDL_CreateWindow(
      name.data(),
      static_cast<int>(w),
      static_cast<int>(h),
      0
    );

    if (!s_Window)
    {
      LOG_ERR("SDL_CreateWindow(): {}", SDL_GetError());
      SDL_Quit();
      return false;
    }

    return true;
  }

  void DeinitWindow()
  {
    SDL_DestroyWindow(s_Window);
    SDL_Quit();
    s_Window = nullptr;
  }

  void* GetHandle()
  {
    SDL_PropertiesID properties = SDL_GetWindowProperties(s_Window);
    HWND window = static_cast<HWND>(
      SDL_GetPointerProperty(
        properties,
        SDL_PROP_WINDOW_WIN32_HWND_POINTER,
        nullptr
      )
    );
    return window;
  }
}
