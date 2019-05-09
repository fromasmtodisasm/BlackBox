#ifndef TEXTUREXTURE_HPP
#define TEXTUREXTURE_HPP

#include <string>
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

  Texture();
  Texture(std::string name);
  void setType(const char* TextureType);
};

#endif // TEXTUREXTURE_HPP
