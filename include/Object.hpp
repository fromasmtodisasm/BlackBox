#include <IObject.hpp>
#include <IGeometry.hpp>
#include <glm/glm.hpp>

class Object : public IObject {
protected:
	glm::vec3 m_Pos;
  Geometry *m_Geom;
  Object();
public:
	static Object* Load(string path);
  virtual void move();
  virtual void rotate();
  virtual void draw();

};
