#include <BlackBox/Core/MathHelper.hpp>

#include <tinyxml2.h> 

#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

using namespace tinyxml2;
#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return false; }
std::ostream& operator<<(std::ostream& ss, const Vec3& v)
{
	return ss << "{ x = " << v.x << ", y = " << v.y << ", z = " << v.z << "}";
}

struct Mesh
{
	std::string m_Path;
	Mesh()				= default;
	Mesh(const Mesh& m) = default;
	Mesh(Mesh&& m) = default;
	friend std::ostream& operator<<(std::ostream& ss, const Mesh& scene);
	std::ostream& format(std::ostream& ss) const
	{
		return ss << m_Path;	
	}
};

std::ostream& operator<<(std::ostream& ss, const Mesh& o)
{
	return o.format(ss);
}

struct Transform
{
	Vec3 Position;	
	Vec3 Rotation;	
	Vec3 Scale;	

	friend std::ostream& operator<<(std::ostream& ss, const Transform& scene);

	std::ostream& format(std::ostream& ss) const
	{
		return ss 
			<< "position = " << Position << ", " 
			<< "rotation = " << Rotation << ", " 
			<< "scale = " << Scale;	
	}
};

std::ostream& operator<<(std::ostream& ss, const Transform& o)
{
	return o.format(ss);
}

struct Object
{
	std::string Name;
	std::string Type;
	std::string Material;
	bool Visible;
	bool Transparent;
	Mesh Geometry;
	Transform Transform;

	friend std::ostream& operator<<(std::ostream& ss, const Object& o);

	template<typename T>
	void format(std::ostream& ss, const T& field, const char* name, bool is_end, bool is_table = false) const
	{
		using namespace std;
		ss << "\t\t" << name << " =  " ;
		if (!is_table)
			ss << "\"" << field << "\"";
		else
			ss << "{" << field << "}";
		if (!is_end)
		{
			ss << ",";
		}
		ss << "\n";
	}
	std::ostream& format(std::ostream& ss) const
	{
		ss << "\t" << Name << " = {\n";
		format(ss, Geometry, "mesh", false);
		format(ss, Transform, "transform", false, true);
		format(ss, Type, "type", false);
		format(ss, Material, "material", false);
		format(ss, Visible, "visible", false);
		format(ss, Transparent, "transparent", true);
		ss << "\t}";
		return ss;
	}


};

std::ostream& operator<<(std::ostream& ss, const Object& o)
{
	return o.format(ss);
}

struct Camera
{
	std::string Name;
	Transform Transform;
};

struct Scene
{
	Scene(const std::string& name)
		: Name(name)
	{
	}
	std::vector<std::shared_ptr<Object>> Objects;
	std::string Name;

	friend std::ostream& operator<<(std::ostream& ss, const Scene& scene);

	std::ostream& format(std::ostream& ss) const
	{
		ss << "objects = {\n";
		size_t i = 0;
		for (const auto& o : Objects)
		{
			ss << *o;	
			if (i < Objects.size() -1)
			{
				ss << ",";	
			}
			ss << "\n";
			i++;
		}
		ss << "}";
		return ss;
	
	}
};

std::ostream& operator<<(std::ostream& ss, const Scene& scene)
{
	return scene.format(ss);
}

template<typename Reader, typename Writer>
class Serializator
{
	public:
	Serializator(Scene& scene)
		  : m_Scene(scene) {}
	void Save()
	{
		auto replace_ext = [&](const std::string& str, const std::string& ext) -> std::string {
			std::cout << "Splitting: " << str << '\n';
			auto new_str = std::string(str);
			new_str.erase(str.find_last_of(".") + 1, std::string::npos).insert(new_str.size(), ext);
			return new_str;
		};

		std::ofstream scene(replace_ext(m_Scene.Name, "lua"));
		if (scene.is_open())
		{
			scene << m_Scene;
		}
		//std::cout << scene.str();
	}
	template<typename T>
	void Save(const T& value)
	{
		value.to_string();
	}


	bool Load()
	{
			
		tinyxml2::XMLDocument xmlDoc;
		XMLError eResult = xmlDoc.LoadFile(m_Scene.Name.c_str());
		XMLCheckResult(eResult);

		XMLNode* pScene = xmlDoc.FirstChild();
		if (pScene == nullptr) return false;

		XMLElement* objects = pScene->FirstChildElement("object");

		if (objects == nullptr) return false;
		while (objects != nullptr)
		{
			LoadObject(objects);
			objects = objects->NextSiblingElement("object");
		}

		XMLElement* terrain = pScene->FirstChildElement("terrain");
		if (terrain != nullptr)
		{
			LoadTerrain(terrain);
		}

		XMLElement* lights = pScene->FirstChildElement("light");
		while (lights != nullptr)
		{
			lights = lights->NextSiblingElement("light");
		}


		#ifdef SAVE_LOAD_TAGPOINTS
		XMLElement* tag_points = pScene->FirstChildElement("tag_points");
		if (tag_points == nullptr) return false;
		while (tag_points != nullptr)
		{
		loadTagPoint(tag_points);
		tag_points = tag_points->NextSiblingElement("tagpoint");
		}
		#endif // SAVE_LOAD_TAGPOINTS

		XMLElement* cameras = pScene->FirstChildElement("camera");
		if (cameras == nullptr) return false;
		while (cameras != nullptr)
		{
			LoadCamera(cameras);
			cameras = cameras->NextSiblingElement("camera");
		}

	}
	bool LoadDefault()
	{
	
	}

	void LoadTerrain(tinyxml2::XMLElement* terrain)
	{
	
	}
	void LoadObject(tinyxml2::XMLElement* object)
	{
		std::shared_ptr<Object> obj = std::make_shared<Object>();
		std::string material("default");
		const char* objectName = nullptr;
		const char* objectType = nullptr;
		bool objectTransparent = false;
		bool objectVisible = false;
		const char* meshPath = nullptr;
		const char* materialName = nullptr;
		XMLElement* mesh = nullptr;
		XMLElement* materialElement = nullptr;
		Transform transform;

		objectName = object->Attribute("name");
		if (objectName == nullptr)
			return;
		objectType = object->Attribute("type");
		if (objectType == nullptr)
			objectType = "object";
		{
			if (object->BoolAttribute("transparent"))
			  objectTransparent = true;
			if (object->BoolAttribute("visible"))
			  objectVisible = true;
			mesh = object->FirstChildElement("mesh");
			if (mesh == nullptr)
			  return;
			meshPath = mesh->Attribute("name");
			if (meshPath == nullptr)
			  return;
			obj->Geometry.m_Path = meshPath;
			//obj = ObjectManager::instance()->getObject(meshPath, objectType, callback);
			obj->Name;
		}
		if (obj == nullptr)
		return;
		materialElement = object->FirstChildElement("material");
		materialName = materialElement->Attribute("name");
		if (materialName != nullptr)
		  material = materialName;

		auto uv = object->FirstChildElement("uv");
		if (uv != nullptr)
		{
			auto scaleX = uv->FloatAttribute("scaleX", 1.0);
			auto scaleY = uv->FloatAttribute("scaleY", 1.0);
			//obj->uvMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scaleX, scaleY, 1.0f));
		}
		else
		{
			//obj->uvMatrix = glm::mat4(1.0f);
		}

		transform = LoadTransform(*object);
		obj->Name = objectName;
		obj->Type = objectType;
		obj->Transform = transform;
		obj->Transparent = objectTransparent;
		obj->Visible = objectVisible;
		obj->Material = material;
		m_Scene.Objects.push_back(obj);
	
	}
	void LoadMesh(tinyxml2::XMLElement* mesh)
	{
	
	}
	void LoadLight(tinyxml2::XMLElement* light)
	{
	
	}
	Transform LoadTransform(tinyxml2::XMLElement& object)
	{
		return Transform();
	}
	void LoadCamera(tinyxml2::XMLElement* element)
	{
	
	}
	void LoadTagPoint(tinyxml2::XMLElement* element)
	{

	}

	void SaveLights(tinyxml2::XMLDocument& xmlDoc, tinyxml2::XMLNode* pScene)
	{
	
	}
	void SaveObject(tinyxml2::XMLDocument& xmlDoc, Object& obj, tinyxml2::XMLNode* pScene)
	{
			
	}
	tinyxml2::XMLElement* SaveTransform(tinyxml2::XMLDocument& xmlDoc, Transform* transform)
	{
	
	}
	//tinyxml2::XMLElement* saveLight(tinyxml2::XMLDocument& xmlDoc, BaseLight* light);
	tinyxml2::XMLElement* SaveCamera(tinyxml2::XMLDocument& xmlDoc, const Camera& camera)
	{
	
	}
	private:
	Scene& m_Scene;
};

int main(int argc, char* argv[])
{
	argc = 2;
	if (argc < 2)
		return -1;
	Scene scene(std::string("./res/scenes/test.xml"));
	//scene_name = argv[1]
	Serializator<void,void> serializtor(scene);
	serializtor.Load();
	serializtor.Save();
}