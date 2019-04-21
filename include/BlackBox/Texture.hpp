#ifndef TEXTUREXTURE_HPP
#define TEXTUREXTURE_HPP

#include <string>
#include <glad/glad.h>
#include <SFML/Graphics.hpp>

class Texture
{
public:
  int width;
  int height;
  GLuint id;

  Texture();
  Texture(std::string name);
};

#endif // TEXTUREXTURE_HPP
