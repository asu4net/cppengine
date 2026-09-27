#ifdef ENGINE_SDL
#include "SDL/SDL_Input.h"
#include "SDL/SDL_Input.cpp"
#else
#error "Missing implementation for Input!"
#endif

namespace Input
{
  bool ShouldCloseWindow()
  {
    #ifdef ENGINE_SDL
    return SDLInput::PollQuitEvent();
    #else
    #error "Missing implementation for Input!"
    #endif
  }
}
