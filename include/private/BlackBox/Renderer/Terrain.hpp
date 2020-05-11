#if 0
#pragma once
#include <BlackBox/Renderer/IGeometry.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <vector>

class CStatObj;

class Terrain
{
public:
  struct Face
  {
    Vertex v[3];
  };
  Terrain();
  Face getFace(size_t v1, size_t v2, size_t v3);
  CStatObj* load(const char* heightmap);
  void draw();

private:
  float getHeight(int x, int y);

  std::vector<Vertex> vb;
  std::unique_ptr<Image> img;
  uint VBO, VAO;
  int size;
};
#endif