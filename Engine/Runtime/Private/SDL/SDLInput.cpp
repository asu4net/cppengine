#include "SDLInput.h"
#include "SDL3/SDL.h"

namespace SDLInput
{
  bool PollQuitEvent()
  {
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_EVENT_QUIT)
      {
        return true;
      }
    }
    return false;
  }
}
