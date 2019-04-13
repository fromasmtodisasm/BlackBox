#include "Texture.hpp"

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
