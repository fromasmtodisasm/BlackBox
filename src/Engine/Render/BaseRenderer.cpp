#include <BlackBox\Render\BaseRenderer.hpp>

int CRenderer::EnumDisplayFormats(SDispFormat* formats)
{
  int numModes = 0;
  //TODO: FIX IT
#if 0
  auto videoModes = sf::VideoMode::getFullscreenModes();

  numModes = static_cast<int>(videoModes.size());
  int i = 0;
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

  return numModes;
}
