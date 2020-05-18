#if 0
#include <BlackBox/Core/IGame.hpp>
#include <BlackBox/Core/MathHelper.hpp>
#include <BlackBox/Renderer/Camera.hpp>
#include <BlackBox/Renderer/Object.hpp>
#include <BlackBox/Renderer/FrameBufferObject.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/Pipeline.hpp>
#include <BlackBox/Renderer/ShadowMapTechnique.hpp>
#include <BlackBox/Renderer/MaterialManager.hpp>
#include <BlackBox/Scene/IScene.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/Renderer/BaseTexture.hpp>
#include <BlackBox/Renderer/AuxRenderer.hpp>

#include <sstream>
ShadowMapping::ShadowMapping()
	: m_Scene(nullptr), m_DepthBuffer(nullptr)
{
}

ShadowMapping::~ShadowMapping()
{
}

bool ShadowMapping::Init(IScene* scene, FrameBufferObject* renderTarget)
{
	m_pRender = GetISystem()->GetIRenderer();
	m_Scene	  = dynamic_cast<Scene*>(scene);
	//
	m_DepthBuffer = FrameBufferObject::create(FrameBufferObject::BufferType::DEPTH_BUFFER, width, height, 1, false);
	m_DepthTexture = new Texture(m_DepthBuffer->texture[0]);
	m_DepthTexture->setUnit(6);

	if (renderTarget == nullptr)
	{
		m_RenderedScene = FrameBufferObject::create(
			FrameBufferObject::BufferType::SCENE_BUFFER, m_pRender->GetWidth(), m_pRender->GetHeight(), 0, false);
	}
	else
	{
		m_RenderedScene = renderTarget;
	}

	m_ShadowMapShader = gEnv->pRenderer->Sh_Load("ShadowPass", 0);

	//==============
	lightPosX = CREATE_CVAR("lpx", -1.f, 0, "light pos x");
	lightPosY = CREATE_CVAR("lpy", 15.f, 0, "light pos y");
	lightPosZ = CREATE_CVAR("lpz", -1.f, 0, "light pos z");
	lighting  = CREATE_CVAR("lighting", bLighting, 0, "light pos z");
	s_divider = CREATE_CVAR("sd", 10.0f, 0, "ortho divider");

	cam_width  = GET_CVAR("r_cam_w");
	cam_height = GET_CVAR("r_cam_h");

	//
	return true;
}

bool ShadowMapping::OnRenderPass(int pass)
{
	DEBUG_GROUP(__FUNCTION__);
	if (pass == 0)
	{
		frame_id++;
	}
	renderPass = static_cast<Pass>(pass);
	switch (pass)
	{
	case RENDER_DEPTHMAP:
		OnDepthPass();
		return true;
	case RENDER_SHADOWS:
		OnRenderPass();
		return true;
	default:
		return false;
	}
	return false;
}

void ShadowMapping::DepthPass()
{
	DEBUG_GROUP(__FUNCTION__);
	float m	 = s_divider->GetFVal();
	lightPos = glm::vec3(lightPosX->GetFVal(), lightPosY->GetFVal(), lightPosZ->GetFVal());
	m_DepthBuffer->bind();
	m_DepthBuffer->clear();
	m_ShadowMapShader->Use();
	glm::mat4 proj = glm::ortho(-1366.f * m, 1366.f * m, -768.f * m, 768.f * m, -1.0f, 5000.f);

	lightSpaceMatrix = proj *
					   glm::lookAt(lightPos,
								   glm::vec3(0.0f, 0.0f, 0.0f),
								   glm::vec3(0.0f, 1.0f, 0.0f));
	m_ShadowMapShader->Uniform(lightSpaceMatrix, "lightSpaceMatrix");

	renderStage = RENDER_DEPTH;
	m_Scene->ForEachObject(this);

	m_ShadowMapShader->Unuse();
	m_DepthBuffer->unbind();
}

void ShadowMapping::RenderPass()
{
	DEBUG_GROUP(__FUNCTION__);

	// Render opaque objects
	renderStage = RENDER_OPAQUE;
	m_Scene->ForEachObject(this);

	auto obj = m_Scene->selectedObject();
    {
        DEBUG_GROUP("DRAW_BB");
        for (auto& mesh : *obj->second->m_Mesh)
        {
            auto t = obj->second->getTransform();
            //TODO: now this is not AABB but OBB
            //need convert it to AABB
            gEnv->pRenderer->GetIRenderAuxGeom()->DrawAABB(t * Vec4(mesh.bb.min, 1.f), t * Vec4(mesh.bb.max, 1.f));
        }
    }

	// Render transparent objects
	renderStage = RENDER_TRANSPARENT;
	m_Scene->ForEachObject(this);
}

void ShadowMapping::RenderDepth(CStatObj* object)
{
	DEBUG_GROUP(__FUNCTION__);
	if (object->visible())
	{
		SRenderParams renderParams;
		//m_ShadowMapShader->Uniform(object->getTransform(), "model");
		m_ShadowMapShader->setup();
		object->Render(renderParams);
	}
}

void ShadowMapping::RenderOpaque(CStatObj* object)
{
	DEBUG_GROUP(__FUNCTION__);
	auto camera = m_Scene->getCurrentCamera();
	if (!object->m_transparent && (object->visible())
#if 0
 &&
		glm::abs(glm::distance(camera->GetPos(), object->m_transform.position)) < camera->zFar->GetFVal()
#endif
  )
	{
		SRenderParams renderParams;

		add_uniform(renderParams, "lightSpaceMatrix", lightSpaceMatrix);
		add_uniform(renderParams, "lightPos", lightPos);
		add_uniform(renderParams, "alpha", object->m_Material->alpha);
		add_uniform(renderParams, "frame_id", frame_id);

		add_uniform(renderParams, "model", object->getTransform());
		add_uniform(renderParams, "view", gEnv->pRenderer->GetCamera().GetViewMatrix());
		add_uniform(renderParams, "projection", gEnv->pRenderer->GetCamera().getProjectionMatrix());
		add_uniform(renderParams, "viewPos", gEnv->pRenderer->GetCamera().transform.position);
		add_uniform(renderParams, "material.shininess", 128.0f);
		add_uniform(renderParams, "shadoMap", m_DepthTexture);

		renderParams.Transform = object->getTransform();
		renderParams.Material = object->getMaterial();
		SetupLights(renderParams);
		object->Render(renderParams);
	}
}

void ShadowMapping::RenderTransparent(CStatObj* object)
{
	DEBUG_GROUP(__FUNCTION__);
	m_pRender->SetState(IRenderer::State::BLEND, true);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (object->m_transparent && (object->visible()))
	{
		SRenderParams renderParams;
		add_uniform(renderParams, "lightSpaceMatrix", lightSpaceMatrix);
		add_uniform(renderParams, "lightPos", lightPos);
		add_uniform(renderParams, "alpha", object->m_Material->alpha);
		add_uniform(renderParams, "bloomThreshold", GetISystem()->GetIConsole()->GetCVar("bt")->GetFVal());

		SetupLights(renderParams);
		object->Render(renderParams);
	}
	m_pRender->SetState(IRenderer::State::BLEND, false);
}

void ShadowMapping::OnDepthPass()
{
	DEBUG_GROUP(__FUNCTION__);
	m_pRender->SetCullMode(IRenderer::CullMode::FRONT);
	DepthPass();
	m_pRender->SetCullMode(IRenderer::CullMode::BACK);
}

void ShadowMapping::OnRenderPass()
{
	DEBUG_GROUP(__FUNCTION__);
	m_RenderedScene->bind({0, 0, cam_width->GetIVal(), cam_height->GetIVal()});
	glm::vec4 fog = glm::vec4(
		GET_CVAR("fogR")->GetFVal(),
		GET_CVAR("fogG")->GetFVal(),
		GET_CVAR("fogB")->GetFVal(),
		1.f);
	//auto w		 = cam_width->GetIVal();
	//auto h		 = cam_height->GetIVal();
	//pSystem->GetIRenderer()->SetState(IRenderer::State::SCISSOR_TEST, true);
	//pSystem->GetIRenderer()->SetScissor(0, 0, w, h);
	//auto render = GetISystem()->GetIRenderer();
	//render->ClearDepthBuffer();
	m_RenderedScene->clear(gl::Color(fog));
	//pSystem->GetIRenderer()->SetState(IRenderer::State::SCISSOR_TEST, false);
	RenderPass();
}

void ShadowMapping::SetupLights(SRenderParams& renderParams)
{
	DEBUG_GROUP(__FUNCTION__);
	if (!bLighting)
		return;
	currentLight				 = 0;
	// direction lights
	SetupDirectionLights(renderParams);
	// point lights
	//SetupPointLights(renderParams);
	// spotLight
	SetupSpotLights(renderParams);

	add_uniform(renderParams, "countOfPointLights", currentLight + 1);
}

void ShadowMapping::SetupDirectionLights(SRenderParams& renderParams)
{
	for (const auto& light : m_Scene->GetDirectionLights())
	{
		OnLightFound(light.second, renderParams);
	}
}

void ShadowMapping::SetupPointLights(SRenderParams& renderParams)
{
	for (const auto& light : m_Scene->GetPointLights())
	{
		OnLightFound(light.second, renderParams);
	}

}

void ShadowMapping::SetupSpotLights(SRenderParams& renderParams)
{
#if 0
	for (const auto& light : m_Scene->GetSpotLights())
	{
		OnLightFound(light.second, renderParams);
	}
#endif
}

void ShadowMapping::InitLights()
{
	DEBUG_GROUP(__FUNCTION__);
	for (const auto& lightit : m_Scene->GetPointLights())
	{
		auto light	 = lightit.second;
		auto program = m_ShadowMapShader;
		//PointLightValues plv;
		//UNREFERENCED_PARAMETER(plv);

		//program->getUniformValue("pointLights[%d].position");
		program->Uniform(light->position, "pointLights[%d].position", currentLight);
		program->Uniform(light->ambient, "pointLights[%d].ambient", currentLight);
		program->Uniform(light->diffuse, "pointLights[%d].diffuse", currentLight);
		program->Uniform(light->specular, "pointLights[%d].specular", currentLight);
		program->Uniform(light->constant, "pointLights[%d].constant", currentLight);
		program->Uniform(light->linear, "pointLights[%d].linear", currentLight);
		program->Uniform(light->quadratic, "pointLights[%d].quadratic", currentLight);
	}
}

int ShadowMapping::SetRenderTarget(FrameBufferObject* renderTarget)
{
	m_RenderedScene = renderTarget;
	return 0;
}

bool ShadowMapping::OnObjectFound(CStatObj* object)
{
	DEBUG_GROUP(__FUNCTION__);
	switch (renderStage)
	{
	case ShadowMapping::RENDER_DEPTH:
		RenderDepth(object);
		return true;
	case ShadowMapping::RENDER_OPAQUE:
		RenderOpaque(object);
		return true;
	case ShadowMapping::RENDER_TRANSPARENT:
		RenderTransparent(object);
		return true;
	default:
		return false;
	}
	return true;
}

int ShadowMapping::GetFrame()
{
	return m_RenderedScene->texture[0];
}

bool ShadowMapping::OnLightFound(const DirectionLight* light, SRenderParams& renderParams)
{
	DEBUG_GROUP(__FUNCTION__);
	add_uniform(renderParams, "dirLight.direction", light->direction);
	add_uniform(renderParams, "dirLight.ambient", light->ambient);
	add_uniform(renderParams, "dirLight.diffuse", light->diffuse);
	add_uniform(renderParams, "dirLight.specular", light->specular);
	return true;
}

bool ShadowMapping::OnLightFound(const PointLight* light, SRenderParams& renderParams)
{
	DEBUG_GROUP(__FUNCTION__);
	if (light->enabled)
	{
		std::stringstream ss;
		add_uniform(renderParams, (ss << "pointLights[" << currentLight << "].position", ss.str().c_str()),  light->position );
		add_uniform(renderParams, (ss << "pointLights[" << currentLight << "].ambient", ss.str().c_str()),  light->ambient );
		add_uniform(renderParams, (ss << "pointLights[" << currentLight << "].diffuse", ss.str().c_str()),  light->diffuse );
		add_uniform(renderParams, (ss << "pointLights[" << currentLight << "].specular", ss.str().c_str()),  light->specular );
		add_uniform(renderParams, (ss << "pointLights[" << currentLight << "].constant", ss.str().c_str()),  light->constant );
		add_uniform(renderParams, (ss << "pointLights[" << currentLight << "].linear", ss.str().c_str()),  light->linear );
		add_uniform(renderParams, (ss << "pointLights[" << currentLight << "].quadratic", ss.str().c_str()),  light->quadratic );
		++currentLight;

		//program->Uniform(light.second->position, "lightPos", currentLight);
		//program->Uniform(glm::vec3(lightPosX->GetFVal(), lightPosY->GetFVal(), lightPosZ->GetFVal()), "lightPos", currentLight);
	}
	return true;
}

bool ShadowMapping::OnLightFound(const SpotLight* light, SRenderParams& renderParams)
{
	DEBUG_GROUP(__FUNCTION__);
#if 0
	program->Uniform(m_Scene->getCurrentCamera()->GetPos(), "spotLight.position");
	program->Uniform(m_Scene->getCurrentCamera()->Front, "spotLight.direction");
	program->Uniform(light->ambient, "spotLight.ambient");
	program->Uniform(light->diffuse, "spotLight.diffuse");
	program->Uniform(light->diffuse, "spotLight.specular");
	program->Uniform(light->constant, "spotLight.constant");
	program->Uniform(light->linear, "spotLight.linear");
	program->Uniform(light->quadratic, "spotLight.quadratic");
	program->Uniform(glm::cos(glm::radians(light->cutOff)), "spotLight.cutOff");
	program->Uniform(glm::cos(glm::radians(light->outerCutOff)), "spotLight.outerCutOff");
#endif
	return true;
}

bool ShadowMapping::PreRenderPass()
{
	bLighting = static_cast<bool>(lighting->GetIVal());
	return true;
}

void ShadowMapping::PostRenderPass()
{
}
#endif
