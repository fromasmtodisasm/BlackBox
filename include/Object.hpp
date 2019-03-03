#include <IGeometry.hpp>
#include <regex>

#ifndef _LOAD_OBJ_
#define _LOAD_OBJ_ 1
#endif

class Object : public IGeometry {
private:
	glm::vec3 pos;
	glm::vec3 size;
public:
//	bool Load(string path); // Load("")
	static Object* Load(string path);


};