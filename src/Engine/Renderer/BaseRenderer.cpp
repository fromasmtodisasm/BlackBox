#include <BlackBox/Renderer/BaseRenderer.hpp>
#include <SDL2/SDL.h>

int CRenderer::EnumDisplayFormats(SDispFormat* formats)
{
  static int display_in_use = 0; /* Only using first display */
  int numModes = 0;
  int i = 0;
  //TODO: FIX IT
#if SFML_WINDOW 
  auto videoModes = sf::VideoMode::getFullscreenModes();

  numModes = static_cast<int>(videoModes.size());
  if (formats != nullptr)
  {
    for (auto& mode : videoModes)
    {
      formats[i].m_BPP = mode.bitsPerPixel;
      formats[i].m_Width = mode.width;
      formats[i].m_Height = mode.height;
      i++;
    }
  }
#endif
//#elif SDL_WINDOW
  SDL_DisplayMode mode;
  Uint32 f;

  SDL_Log("SDL_GetNumVideoDisplays(): %i", SDL_GetNumVideoDisplays());

  numModes = SDL_GetNumDisplayModes(display_in_use);
  if (numModes < 1) {
    SDL_Log("SDL_GetNumDisplayModes failed: %s", SDL_GetError());
    return 1;
  }
  SDL_Log("SDL_GetNumDisplayModes: %i", numModes);

  if (formats != nullptr)
  {
    for (i = 0; i < numModes; ++i) {
      if (SDL_GetDisplayMode(display_in_use, i, &mode) != 0) {
        SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
        return 1;
      }
      f = mode.format;

      formats[i].m_BPP = SDL_BITSPERPIXEL(f);
      formats[i].m_Width = mode.w;
      formats[i].m_Height = mode.h;
    }
  }

//#endif

  return numModes;
}

RenderCVars::RenderCVars()
{
  translateImageY = CREATE_CVAR("ty", 0.0f, 0);
  translateImageX = CREATE_CVAR("tx", 0.0f, 0);
  scaleImageX = CREATE_CVAR("sx", 1.0f, 0);
  scaleImageY = CREATE_CVAR("sy", 1.0f, 0);
  needTranslate = CREATE_CVAR("nt", 1, 0, "Translate or not 2d background of console");
  needFlipY = CREATE_CVAR("nfy", 1, 0, "Flip or not 2d background of console");
  test_proj = CREATE_CVAR("test_proj", "test proj empty", 0);
  render_via_viewport = CREATE_CVAR("rvv", 0, 0, "Rendering use view port, if 1 else with projection matrix");

  r_debug = GET_CVAR("r_debug");
  cam_width = GET_CVAR("r_cam_w");
  cam_height = GET_CVAR("r_cam_h");
}

RenderCVars::~RenderCVars()
{

}
