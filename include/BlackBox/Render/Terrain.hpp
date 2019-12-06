#pragma once
#include <BlackBox/IGeometry.hpp>
#include <BlackBox/Render/IRender.hpp>
#include <vector>

class Terrain
{
public:
  struct Face
  {
    Vertex v[3];
  };
  Terrain();
  Face getFace(size_t v1, size_t v2, size_t v3);
  Object* load(const char* heightmap);
  void draw();

private:
  float getHeight(int x, int y);

  std::vector<Vertex> vb;
  std::unique_ptr<Image> img;
  GLuint VBO, VAO;
  int size;
};