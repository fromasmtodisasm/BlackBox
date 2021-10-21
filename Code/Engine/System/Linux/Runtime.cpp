#include <SDL2/SDL.h>

unsigned int GetCurrentTimeInternal()
{
  return SDL_GetTicks();
}
