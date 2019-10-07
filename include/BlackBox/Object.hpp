#pragma once

#include <BlackBox/IObject.hpp>
#include <BlackBox/IGeometry.hpp>
#include <BlackBox/IDrawable.hpp>
#include <BlackBox/Resources/ShaderManager.hpp>
#include <BlackBox/Material.hpp>
#include <BlackBox/Render/Opengl.hpp>
#include <BlackBox/IScriptSystem.hpp>

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
  std::shared_ptr<CShaderProgram> m_Shader;
  OBJType m_type;
  Object();
  Object(MeshList mesh);
  Object(const Object *obj);
  static void parse(std::string filename, std::vector<Vertex> &vs, CBaseShaderProgram **shader);
public:
	static Object* getEmpty()
	{
		return new Object();
	}
	//=============================
	// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
	// Default camera values
	const GLfloat YAW        = -90.0f;
	const GLfloat PITCH      =  0.0f;
	const GLfloat SPEED      =  0.1f;
	const GLfloat SENSITIVTY =  0.25f;
	const GLfloat ZOOM       =  45.0f;
	//*****************************
    // Camera Attributes
		glm::vec3 Front = glm::vec3(0.f, 0.f, -1.0f);
    glm::vec3 Up = glm::vec3(0.f, 1.f, 0.0f);
    glm::vec3 Right = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 WorldUp = glm::vec3(0.f, 1.f, 0.0f);
    // Eular Angles
    GLfloat Yaw = YAW;
    GLfloat Pitch = PITCH;
    // Camera options
    GLfloat MovementSpeed = 2;
	//=============================

	int m_RenderMode = GL_FILL;
  MeshList m_Mesh;
  std::string type;
  Material *m_Material = nullptr;
  static int refs;
  std::string m_path;
  float friction = 0.99f;
  glm::vec3 velocity;
	bool m_transparent = false;
	bool m_visible = true;

  Transform m_transform;

	glm::mat4 uvMatrix;// = glm::mat4(1.0);

	IScriptObject* m_pScript = nullptr;

  static Object* load(std::string path);
  virtual void move(Movement direction) override;
  void move(glm::vec3 v);
  virtual void moveTo(glm::vec3 v);
  virtual void rotate(float angle, glm::vec3 v) override;
  virtual void scale(glm::vec3 v) override;
  virtual void draw(void * camera) override;
  virtual OBJType getType() override { return m_type; }
  virtual void setType(OBJType) override;
  virtual glm::mat4 getTransform() override;
	void updateVectors();

	bool visible();
	void setVisibility(bool v);

  // Унаследовано через IObject

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

	void setRenderMode(int mode);
	int getRenderMode();

	// Inherited via IObject
	virtual void rotateX(float angle) override;
	virtual void rotateY(float angle) override;
	virtual void rotateZ(float angle) override;

	// Унаследовано через IObject
	virtual void SetScriptObject(IScriptObject* pObject) override;
	virtual IScriptObject* GetScriptObject() override;
};
