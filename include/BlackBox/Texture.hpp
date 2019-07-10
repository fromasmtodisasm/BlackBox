#ifndef TEXTUREXTURE_HPP
#define TEXTUREXTURE_HPP

#include <string>
#include <memory>
#include <glad/glad.h>
#include <SFML/Graphics.hpp>

enum TextureType
{
    DIFFUSE,
    SPECULAR,
    BUMP,
    NORMAL,
    MASK,
    UNKNOWN
};

class Texture
{
public:
  int width;
  int height;
  TextureType type;
  GLuint id;
  std::shared_ptr<std::string> path;

  Texture();
  Texture(std::string name, bool alphaDistMips = true);
  void setType(const char* TextureType);
  std::string typeToStr();
};

#endif // TEXTUREXTURE_HPP
