#include "Object.hpp"
#include <fstream>
#include <cctype>

using namespace std;

Object::Object() //: mv(glm::mat4x4(1.0f))
{

}

void Object::draw() {}

void Object::move(glm::vec3 v) {}

void Object::rotate(float angle, glm::vec3 v) {}

void Object::scale(glm::vec3 v) {}

Object * Object::Load(string path)
{
	Object *obj;
	

	
	return obj;
}
