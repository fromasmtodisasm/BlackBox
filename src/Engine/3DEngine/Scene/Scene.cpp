#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/Profiler/Profiler.h>
//#include <BlackBox/Renderer/FrameBufferObject.hpp>
#include <BlackBox/Renderer/IPostProcessor.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/ITechnique.hpp>
#include <BlackBox/3DEngine/StatObject.hpp>
#include <BlackBox/Renderer/MaterialManager.hpp>
#include <BlackBox/Scene/Scene.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/World/World.hpp>

#include <algorithm>
Scene::Scene(std::string name)
	: lighting(true),
	  name(name),
	  m_RenderedScene(-1),
	  m_Technique(nullptr),
	  quadVAO(-1),
	  shadowMapMat(nullptr),
	  skyBox(nullptr)
{
	float quadVertices[] = {
		// positions   // texCoords
		-1.0f, 1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f,

		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f};

	if (!gEnv->IsDedicated())
	{
		texture_speed = GetISystem()->GetIConsole()->CreateVariable("tex_spd", 0.1f, 0, "Speed of texture animation");
	}
}

// IScene
bool Scene::save(const char* as)
{
	Serializator serializator(this);
	return serializator.save(as);
}

bool Scene::load(const char* name, LoadObjectSink* callback)
{
	Serializator serializator(this);
	auto result = serializator.load(name, callback);

#if 0
	auto obj = terrain.load("res/images/heightmaps/heightmap_thin.png");
	if (obj != nullptr)
	{
		obj->m_transform.scale *= Vec3(40,1,40);
		obj->setMaterial(MaterialManager::instance()->getMaterial("terrain"));
		m_Objects.insert(std::make_pair("terrain", obj));
	}
#endif

	GetISystem()->Log("***************");
	return result;
}

CStatObj* Scene::getObject(std::string name)
{
	auto objectIt = m_Objects.find(name);
	if (objectIt != m_Objects.end())
	{
		return objectIt->second;
	}
	return nullptr;
}

void Scene::addObject(std::string name, CStatObj* object)
{
	m_Objects.insert(std::make_pair(name, object));
}

CCamera* Scene::getCurrentCamera()
{
	return m_CurrentCamera->second;
}

size_t Scene::numObjects()
{
	return m_Objects.size();
}

void Scene::setTechnique(ITechnique* technique)
{
	m_Technique = technique;
}

SkyBox* Scene::GetSkyBox()
{
	return skyBox;
}

void Scene::draw(float dt)
{
	//DEBUG_GROUP("Render Loop");
  if (m_Technique)
  {
    if (m_Objects.size() > 0)
    {
      //DEBUG_GROUP("PASS...");
      for (int pass = 0; m_Technique->OnRenderPass(pass); pass++)
        ;
    }
		m_RenderedScene = m_Technique->GetFrame();
  }
}

void Scene::setCamera(std::string name, CCamera* camera)
{
	m_Camera[name]	= camera;
	m_CurrentCamera = m_Camera.find(name);
}

void Scene::begin()
{
	/*
  m_RenderedScene->bind();
  glCheck(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)); // буфер трафарета не используется
  glCheck(glEnable(GL_DEPTH_TEST));
  */
}

void Scene::end()
{
	//m_RenderedScene->unbind();
}

void Scene::ForEachObject(ForEachObjectSink* callback)
{
	for (const auto& object : m_Objects)
	{
		if (!callback->OnObjectFound(object.second))
			break;
	}
}

void Scene::ForEachDirectionLight(ForEachDirectionLightSink* callback)
{
	for (const auto& light : m_DirectionLight)
	{
		if (!callback->OnLightFound(light.second))
			break;
	}
}

void Scene::ForEachPointLight(ForEachPointLightSink* callback)
{
	for (const auto& light : m_PointLights)
	{
		if (!callback->OnLightFound(light.second))
			break;
	}
}

void Scene::ForEachSpotLight(ForEachSpotLightSink* callback)
{
	for (const auto& light : m_SpotLights)
	{
		if (!callback->OnLightFound(light.second))
			break;
	}
}

const DirectionLightList& Scene::GetDirectionLights()
{
	return m_DirectionLight;
}

const PointLightList& Scene::GetPointLights()
{
	return m_PointLights;
}

const SpotLightList& Scene::GetSpotLights()
{
	
	return m_SpotLights;

}
