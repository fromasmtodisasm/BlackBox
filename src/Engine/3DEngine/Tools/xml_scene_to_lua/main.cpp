#include <tinyxml2.h> 
#include <vector>
#include <string>
#include <string_view>
#include <memory>
#include <fstream>
#include <sstream>
#include <variant>
#include <BlackBox/Core/MathHelper.hpp>

using namespace tinyxml2;
#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return false; }

struct Mesh
{
	std::string m_Path;
};

struct Transform
{
	Vec3 Position;	
	Vec3 Rotation;	
	Vec3 Scale;	
};

struct Object
{
	std::string Name;
	std::string Type;
	std::string Material;
	bool Visible;
	bool Transparent;
	Mesh Geometry;
	Transform Transform;
};

struct Camera
{
	std::string Name;
	Transform Transform;
};

struct Scene
{
	Scene(std::string& name)
		: Name(name)
	{
	}
	std::vector<std::shared_ptr<Object>> Objects;

	const std::string& Name;
};

struct LuaTable
{
	std::string Name;
	std::string Value;
	LuaTable* 
};
struct LuaDoc
{

	void SaveFile(const std::string& file)
	{
		std::ofstream of(file); 		
		if (!of.is_open())
			return;

	}

	template<typename T>
	std::stringstream& operator >> (T& t)
	{
		return content << t;	
	}

	std::stringstream content;
};

template<typename Reader, typename Writer>
class Serializator
{
	public:
	Serializator(Scene& scene)
		  : m_Scene(scene) {}
	bool Save(const char* as = "")
	{
		std::stringstream sceneName;
		tinyxml2::XMLDocument xmlDoc;
		XMLNode* pScene = xmlDoc.NewElement("scene");

		for (auto& obj : m_Scene->m_Objects)
		{
			SaveObject(xmlDoc, objectManager, obj, pScene);
		}

		saveLights(xmlDoc, pScene);

		for (auto& camera : m_Scene->m_Camera)
		{
			XMLElement* cameraElement = saveCamera(xmlDoc, camera.second);
			cameraElement->SetAttribute("name", camera.first.c_str());
			pScene->InsertEndChild(cameraElement);
		}
		xmlDoc.InsertFirstChild(pScene);

		#if 0
		if (as == "")
			sceneName << "res/scenes/" << name << ".xml";
		else
		#endif
		sceneName << "res/scenes/" << as;
		XMLError eResult = xmlDoc.SaveFile(sceneName.str().c_str());
		XMLCheckResult(eResult);
	}
	bool Load()
	{
			
		tinyxml2::XMLDocument xmlDoc;
		XMLError eResult = xmlDoc.LoadFile(m_Scene.Name);
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

		transform = loadTransform(*object);
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
	Transform loadTransform(tinyxml2::XMLElement& object)
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
	//Scene scene(std::string(argv[1]));
	Serializator<void,void> serializtor(scene);
	serializtor.Load();
	serializtor.Save();
}