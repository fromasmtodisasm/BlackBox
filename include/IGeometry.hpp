// #include <GLFW/glfw3.h>
// #include <glm/glm.hpp>

#include <common.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using mat4 = glm::mat4;
using vec3 = glm::vec3;
using std::vector;

struct IGeometry {
protected:
	vector<vec3> verts;
public:
	vector<vec3>& getVerts() { return verts; }
	vec3* getGLVerts() { return verts.empty() ? nullptr : &verts[0]; }
	virtual bool kernel(std::vector<UniType>);
};

/*

// 3,3,3; 3,3,3

Geometry g;

vec3 v = g[0];

v = {...,...,...};

g[0] = {10.2f,10.9f,100.90f};



*/