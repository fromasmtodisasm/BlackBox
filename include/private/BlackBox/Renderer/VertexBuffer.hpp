#pragma once
#include <BlackBox/Renderer/IRender.hpp>

#include <map>

class VertexArrayObject
{
public:
  enum Attribute
  {
    POSITION,
    NORMAL,
    UV,
    TANGENT,
    BTANGENT
  };
  using Attributes =
    struct {
    int stride;
    std::map<Attribute, int> attributes;
  };

  VertexArrayObject(const void* data, int size, RenderPrimitive primitive, const Attributes &attributes);
  ~VertexArrayObject();
  void draw() const;
  void draw_indexed() const;
  void setAttributes(Attributes& attributes);
  Attributes& getAttributes();
  uint getId() { return id; }
protected:
  virtual bool init();
  void setup_attributes();
protected:
  uint VBO;
  uint id;
  const void* m_Data;
  int m_Count;
  int m_Mode;
  Attributes m_attributes;
};