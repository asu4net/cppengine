#ifdef ENGINE_SDL
#include "SDL/SDLInput.h"
#include "SDL/SDLInput.cpp"
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
