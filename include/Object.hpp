#pragma once

#include <IObject.hpp>
#include <IGeometry.hpp>
#include <glm/glm.hpp>



class Object : public IObject {
protected:
	glm::vec3 m_Pos;
  Geometry *m_Geom;
  OBJType ty;
  Object();
public:

	static Object* Load(string path);
  virtual void move() override;
  virtual void rotate() override;
  virtual void draw() override;
  virtual OBJType getType() override { return ty; }
};
