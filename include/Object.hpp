#pragma once

#include <IObject.hpp>
#include <IGeometry.hpp>
#include <glm/glm.hpp>

class Object : public IObject {
protected:
	glm::vec3 m_Pos;
  //glm::mat4 mv;
  Mesh *m_Mesh;
  OBJType ty;
  Object();
public:

	static Object* Load(string path);
  virtual void move(glm::vec3 v) override;
  virtual void rotate(float angle, glm::vec3 v) override;
  virtual void scale(glm::vec3 v) override;
  virtual void draw() override;
  virtual OBJType getType() override { return ty; }
};
