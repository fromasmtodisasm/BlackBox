#pragma once

#include <IObject.hpp>
#include <IGeometry.hpp>
#include <IDrawable.hpp>
#include <CShader.hpp>
#include <glm/glm.hpp>

typedef glm::vec3 Vertex;

class Object : public IObject, public IDrawable {
protected:
	glm::vec3 m_Pos;
  glm::vec3 angle;
  //glm::mat4 mv;
  Mesh *m_Mesh;
  CShaderProgram *m_Shader;
  OBJType m_type;
  Object();
  static void parse(std::string filename, std::vector<Vertex> &vs, CShaderProgram **shader);
public:

	static Object* load(string path);
  virtual void move(glm::vec3 v) override;
  virtual void rotate(float angle, glm::vec3 v) override;
  virtual void scale(glm::vec3 v) override;
  virtual void draw() override;
  virtual OBJType getType() override { return m_type; }

  // Унаследовано через IObject
  virtual void setType(OBJType) override;

  // Унаследовано через IObject
  virtual CShaderProgram * getShaderProgram() override;
};
