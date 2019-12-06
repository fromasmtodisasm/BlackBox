#pragma once
#include <BlackBox/Render/IRender.hpp>

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

  VertexArrayObject(const void* data, int size, int type, Attributes attributes);
  ~VertexArrayObject();
  void draw();
  void setAttributes(Attributes& attributes);
  Attributes& getAttributes();
  uint getId() { return id; }
protected:
  virtual bool init();
protected:
  uint VBO;
  uint id;
  const void* m_Data;
  int m_Count;
  int m_Type;
  Attributes m_attributes;
};