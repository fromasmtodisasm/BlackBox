#include <BlackBox/Renderer/BaseTexture.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

bool Image::load(const char* name, bool* hasAlpha)
{
  std::string path = std::string(name);

#ifndef NVTT
  // Load the image and get a pointer to the pixels in memory
  int channels = 4;
  int dc = STBI_rgb;
  unsigned char* ptr = stbi_load(path.c_str(), &width, &height, &channels, dc);
  if (channels == 4 && dc != STBI_rgb_alpha)
  {
    stbi_image_free(ptr);
	  ptr = nullptr;
    ptr = stbi_load(path.c_str(), &width, &height, &channels, dc = STBI_rgb_alpha);
  }

  if (ptr)
  {
    // Assign the image properties
    if (width && height)
    {
      if (hasAlpha != nullptr && channels == 4)
        *hasAlpha = true;
      if (dc == STBI_rgb_alpha)
        channels = 4;
      // Copy the loaded pixels to the pixel buffer
      data = new unsigned char[width * height * channels];
      memcpy(data, ptr, width * height * channels);
    }

    // Free the loaded pixels (they are now in our own pixel buffer)
    stbi_image_free(ptr);
  }
#else
  ::srand(time(0));
  nvtt::Surface surface;
  if (!surface.load(path.c_str()))
  {
    // TODO: LOG IT
    return false;
  }
  w = surface.width();
  h = surface.height();
  surface.flipY();
  float* pix = new float[w * h * sizeof(float)];// surface.data();
  const float* r, * g, * b, * a;
  r = surface.channel(0);
  g = surface.channel(1);
  b = surface.channel(2);

  ::srand(time(0));
  nvtt::Surface surface;
  //surface.setAlphaMode(nvtt::AlphaMode::AlphaMode_Transparency);
  if (!surface.load(path.c_str(), &has_alpha))
  {
    // TODO: LOG IT
    return false;
  }
  w = surface.width();
  h = surface.height();
  surface.flipY();
  auto pix = std::make_unique<float[]>(w * h * sizeof(float));
  const float* r, * g, * b, * a;
  r = surface.channel(0);
  g = surface.channel(1);
  b = surface.channel(2);

  int step = 3;
  int format = surface.alphaMode();
  if (surface.channel(3) != nullptr)
  {
    has_alpha = true;
    a = surface.channel(3);
    step = 4;
    inputFormat = GL_RGBA;
    internalFormat = GL_RGBA;
  }

  for (int i = 0; i < w * h; i++)
  {
    pix[i * step] = r[i];
    pix[i * step + 1] = g[i];
    pix[i * step + 2] = b[i];
    if (has_alpha)
      pix[i * step + 3] = a[i];
  }
  pixels = pix.Get();
  //pixels = surface.data();
  //inputFormat = GL_BGRA;
  inputDataType = GL_FLOAT;
#endif
  return true;
}

Image::~Image()
{
  free();
}

void Image::free()
{
  delete[] data;
}

int BaseTexture::getWidth() const 
{
  return width;
}

int BaseTexture::getHeight() const 
{
  return height;
}

int BaseTexture::getId() const 
{
  return id;
}