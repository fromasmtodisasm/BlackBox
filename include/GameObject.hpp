#pragma once
#include "Primitives.hpp"

class GameObject : Object 
{
  Object *m_obj;

public:
  GameObject(Primitive::Type type);
};
