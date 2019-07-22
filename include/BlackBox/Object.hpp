#pragma once

#include <BlackBox/IObject.hpp>
#include <BlackBox/IGeometry.hpp>
#include <BlackBox/IDrawable.hpp>
#include <BlackBox/Resources/ShaderManager.hpp>
#include <BlackBox/Material.hpp>

#include <glm/glm.hpp>

#include <memory>

using MeshList = std::shared_ptr<std::vector<Mesh>>;

struct Transform
{
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;

  Transform();
  Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) :
    position(position), rotation(rotation), scale(scale){}
};

class Object : public IObject, public IDrawable {
  friend class ObjectManager;
protected:
  CBaseShaderProgram *m_Shader;
  OBJType m_type;
  Object();
  Object(MeshList mesh);
  Object(const Object *obj);
  static void parse(std::string filename, std::vector<Vertex> &vs, CBaseShaderProgram **shader);
public:
  MeshList m_Mesh;
  std::string type;
  Material *m_Material = nullptr;
  static int refs;
  std::shared_ptr<std::string> m_path;
  float friction = 0.99f;
  glm::vec3 velocity;
	bool m_transparent = false;
	bool m_visible = true;

  Transform m_transform;

  static Object* load(std::string path);
  virtual void move(glm::vec3 v) override;
  virtual void moveTo(glm::vec3 v);
  virtual void rotate(float angle, glm::vec3 v) override;
  virtual void scale(glm::vec3 v) override;
  virtual void draw(void * camera) override;
  virtual OBJType getType() override { return m_type; }
  virtual void setType(OBJType) override;
  virtual CBaseShaderProgram * getShaderProgram() override;
  virtual glm::mat4 getTransform() override;

	bool visible();
	void setVisibility(bool v);

  // Унаследовано через IObject
  virtual void setShaderProgram(CBaseShaderProgram* shader) override;

  // IObject interface
public:
  virtual void update(float deltatime) override;

  // IObject interface
public:
  virtual void setTexture(Texture *texture, const char *type) override;

  Object operator=(Object &that);
  Object *clone();

  // IObject interface
public:
  virtual Material *getMaterial() override;
  virtual void setMaterial(Material *material) override;
};
