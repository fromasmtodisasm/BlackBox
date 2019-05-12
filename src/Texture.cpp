#include <BlackBox/Texture.hpp>
#include <iostream>

using namespace std;

Texture::Texture()
{

}

Texture::Texture(std::string name)
{
  sf::Texture text;
  text.loadFromFile("res/images/" + name);
  id = text.getNativeHandle();
  sf::Image img_data;
  if (!img_data.loadFromFile("res/images/" + name))
  {
      ;//return false;
  }
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);

  glTexImage2D(
    GL_TEXTURE_2D, 0, GL_RGBA,
    img_data.getSize().x, img_data.getSize().y,
    0,
    GL_RGBA, GL_UNSIGNED_BYTE, img_data.getPixelsPtr()
  );
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setType(const char *type)
{
  std::string t(type);

  if (t == "diffuse")
    this->type = DIFFUSE;
  else if (t == "specular")
    this->type = SPECULAR;
  else if (t == "bump")
    this->type = BUMP;
  else if (t == "normal")
    this->type = NORMAL;
  else if (t == "mask")
    this->type = MASK;
  else
    this->type = UNKNOWN;
}

std::string Texture::typeToStr()
{
  std::string result;
  switch(type)
    {
    case TextureType::DIFFUSE:
      return "diffuse";
    case TextureType::SPECULAR:
      return "specular";
    case TextureType::BUMP:
      return "bump";
    case TextureType::NORMAL:
      return "normal";
    case TextureType::MASK:
      return "mask";
    default:
    {
      cout << "Error: unknown texture type" << endl;
      return "";
    }
    }
}
