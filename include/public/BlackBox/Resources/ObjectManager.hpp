#pragma once

class Object;
struct Mesh;

#include <map>
#include <memory>
#include <string>
#include <vector>

struct LoadObjectSink
{
	virtual Object* OnLoad(Object* object, std::string type) = 0;
};

class ObjectManager
{
	static ObjectManager* manager;
	std::map<std::string, std::shared_ptr<std::vector<Mesh>>> cache;
	//ObjectManager();
  public:
	static ObjectManager* instance();
	Object* getObject(std::string Object, std::string type, LoadObjectSink* callback);
	std::string getPathByPointer(Object* object);
	Object* objectFactory(Object* object, std::string type, LoadObjectSink* callback);
	//Object *getPrimitive(Primitive::Type type, CShaderProgram *program);
};