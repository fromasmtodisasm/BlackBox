#include "pch.hpp"
#include "Entity.hpp"
#include "EntitySystem.hpp"
#include "IStatObj.h"

#include "PhysicalEntity.hpp"
#include <BlackBox/Renderer/IRenderAuxGeom.hpp>

#include <BlackBox/ScriptSystem/IScriptSystem.hpp>
#include <BlackBox/3DEngine/I3DEngine.hpp>

CEntity::CEntity(CEntityDesc& desc)
    : m_pEntitySystem((CEntitySystem*)Env::EntitySystem())
    , m_Desc(desc)
{
	m_EntityCharacter = new CEntityCharacter;
	m_Pos             = desc.pos;
	m_Angles          = desc.angles;
	m_ClassName       = desc.className;
	m_Name            = desc.name;
	m_Id              = desc.id;
	m_pISystem				= Env::System();
}

CEntity::~CEntity()
{
	SAFE_DELETE(m_EntityCharacter);
}

void CEntity::GetRenderBBox(Legacy::Vec3& mins, Legacy::Vec3& maxs)
{
}

float CEntity::GetRenderRadius() const
{
	return 0.0f;
}

bool CEntity::IsStatic() const
{
	return false;
}

bool CEntity::IsEntityHasSomethingToRender()
{
	return false;
}

bool CEntity::IsEntityAreasVisible()
{
	return false;
}

void CEntity::SetMaterial(IMatInfo* pMatInfo)
{
}

IMatInfo* CEntity::GetMaterial() const
{
	return nullptr;
}

void CEntity::PreloadInstanceResources(Legacy::Vec3 vPrevPortalPos, float fPrevPortalDistance, float fTime)
{
}

EntityId CEntity::GetId() const
{
	return m_Id;
}

void CEntity::Update(SEntityUpdateContext& updateContext)
{
	// update container
	if (m_bUpdateContainer)
	{
		//if (m_pContainer && m_pEntitySystem->m_pUpdateContainer->GetIVal())
		{
			ENTITY_PROFILER_NAME("CEntity:Update:Container")
				m_pContainer->Update();
		}
	}
}

void CEntity::Reset()
{
}

bool CEntity::GetNetPresence() const
{
	return false;
}

void CEntity::SetNetPresence(bool bPresent)
{
}

void CEntity::SetName(const char* name)
{
	m_Name = name;
}

void CEntity::SetClassName(const char* name)
{
	m_ClassName = name;
}

const char* CEntity::GetName() const
{
	return m_Name.data();
}

const char* CEntity::GetEntityClassName() const
{
	return m_ClassName.data();
}

void CEntity::GetEntityDesc(CEntityDesc& desc) const
{
	desc = m_Desc;
}

void CEntity::GetHelperPosition(const char* helper, Legacy::Vec3& pos, bool objectspace)
{
}

EntityClassId CEntity::GetClassId()
{
	return m_ClassId;
}

void CEntity::SetClassId(const EntityClassId ClassId)
{
	m_ClassId = ClassId;
}

void CEntity::ShutDown()
{
}

bool CEntity::IsGarbage()
{
	return m_IsGarbage;
}

int CEntity::GetObjectsFlags(int nSubObj)
{
	return 0;
}

void CEntity::SetFlags(unsigned int flags)
{
}

void CEntity::ClearFlags(unsigned int flags)
{
}

unsigned int CEntity::GetFlags()
{
	return 0;
}

IEntityCamera* CEntity::GetCamera() const
{
	return m_pEntityCamera;
}

void CEntity::SetCamera(IEntityCamera* cam)
{
	m_pEntityCamera = cam;
}

CDLight* CEntity::GetLight()
{
	return nullptr;
}

bool CEntity::InitLight(const char* img, const char* shader, bool bUseAsCube, float fAnimSpeed, int nLightStyle, float fCoronaScale)
{
	return false;
}

void CEntity::SendScriptEvent(EScriptEventId Event, IScriptObject* pParamters, bool* pRet)
{
}

void CEntity::SendScriptEvent(EScriptEventId Event, const char* str, bool* pRet)
{
}

void CEntity::SendScriptEvent(EScriptEventId Event, int nParam, bool* pRet)
{
}

void CEntity::CallEventHandler(const char* sEvent)
{
}

IPhysicalEntity* CEntity::GetPhysics() const
{
	return m_pPhysics;
}

void CEntity::SetPhysics(IPhysicalEntity* physic)
{
}

void CEntity::DestroyPhysics()
{
}

int CEntity::CreatePhysicalEntityCallback(int iForeignFlags)
{
	return 0;
}

int CEntity::DestroyPhysicalEntityCallback(IPhysicalEntity* pent)
{
	return 0;
}

void CEntity::EnablePhysics(bool enable)
{
}

void CEntity::AddImpulse(int ipart, Legacy::Vec3 pos, Legacy::Vec3 impulse, bool bPos, float fAuxScale)
{
}

bool CEntity::CreateRigidBody(pe_type type, float density, float mass, int surface_id, Legacy::Vec3* pInitialVelocity, int slot, bool bPermanent)
{
	return false;
}

bool CEntity::CreateLivingEntity(float mass, float height, float eye_height, float sphere_height, float radius, int nSurfaceID, float fGravity, float fAirControl, bool collide)
{
	return false;
}

bool CEntity::CreateStaticEntity(float mass, int surface_idx, int slotToUse, bool bPermanent)
{
	return false;
}

bool CEntity::CreateSoftEntity(float mass, float density, bool bCloth, IPhysicalEntity* pAttachTo, int iAttachToPart)
{
	return false;
}

bool CEntity::LoadVehicle(const char* objfile, pe_cargeomparams* pparts, pe_params_car* params, bool bDestroy)
{
	return false;
}

bool CEntity::LoadBoat(const char* objfile, float mass, int surfaceID)
{
	return false;
}

void CEntity::SetDamage(const int dmg)
{
}

bool CEntity::LoadObject(unsigned int slot, const char* fileName, float scale, const char* geomName)
{
	return false;
}

bool CEntity::GetObjectPos(unsigned int slot, Legacy::Vec3& pos)
{
	return false;
}

bool CEntity::SetObjectPos(unsigned int slot, const Legacy::Vec3& pos)
{
	return false;
}

bool CEntity::GetObjectAngles(unsigned int slot, Legacy::Vec3& ang)
{
	return false;
}

bool CEntity::SetObjectAngles(unsigned int slot, const Legacy::Vec3& ang)
{
	return false;
}

void CEntity::LoadBreakableObject(const char* fileName)
{
}

bool CEntity::SetEntityObject(unsigned int slot, const CEntityObject& object)
{
	m_EntityObject = object;
	return true;
}

bool CEntity::GetEntityObject(unsigned int slot, CEntityObject& object)
{
	object = m_EntityObject;
	return true;
}

int CEntity::GetNumObjects()
{
	return 0;
}

IStatObj* CEntity::GetIStatObj(unsigned int pos)
{
	return m_EntityObject.object;
}

void CEntity::SetIStatObj(IStatObj* obj, unsigned int pos)
{
	m_EntityObject.object = obj;
	SetBBox(obj->GetBoxMin(), obj->GetBoxMax());
}

void CEntity::PlaySound(ISound* pSound, float fSoundScale, Legacy::Vec3& Offset)
{
}

void CEntity::DrawObject(unsigned int pos, int mode)
{
}

void CEntity::DrawObject(int mode)
{
}

void CEntity::DrawCharacter(int pos, int mode)
{
}

void CEntity::NeedsUpdateCharacter(int pos, bool updt)
{
}

void CEntity::SetBBox(const Legacy::Vec3& mins, const Legacy::Vec3& maxs)
{
	m_BoxMin = mins;
	m_BoxMax = maxs;
}

void CEntity::GetBBox(Legacy::Vec3& mins, Legacy::Vec3& maxs)
{
	mins = m_BoxMin/* * m_Scale*/;
	maxs = m_BoxMax/* * m_Scale*/;
}

void CEntity::GetLocalBBox(Legacy::Vec3& min, Legacy::Vec3& max)
{
}

void CEntity::InvalidateBBox()
{
}

void CEntity::TrackColliders(bool bEnable)
{
}

bool CEntity::DrawEntity(const SRendParams& EntDrawParams)
{
	auto object = m_EntityObject.object;
	if (object == nullptr)
		return false;
	auto      pos       = GetPos();
	auto      rotation  = GetAngles();
	auto      scale     = GetScale();
	glm::mat4 transform = glm::mat4(1);
#if 1
	transform = glm::translate(transform, pos);

	transform = glm::rotate(transform, glm::radians(rotation.x), {1, 0, 0});
	transform = glm::rotate(transform, glm::radians(rotation.y), {0, 1, 0});
	transform = glm::rotate(transform, glm::radians(rotation.z), {0, 0, 1});
	auto rotate = transform;

	transform = glm::scale(transform, scale);
#endif
	SRendParams rp(EntDrawParams);
	rp.pMatrix = &transform;
	rp.texture = object->GetTexture();

	object->Render(rp, {});
	Legacy::Vec3 min, max;
	GetBBox(min, max);
	transform = rotate;
	auto tmin = transform * glm::vec4(min, 1);
	auto tmax = transform * glm::vec4(max, 1);
	//Env::AuxGeomRenderer()->DrawAABB(tmin, tmax, UCol({1, 1, 1}));
	return true;
}

bool CEntity::CreateParticleEntity(float size, float mass, Legacy::Vec3 heading, float acc_thrust, float k_air_resistance, float acc_lift, float gravity, int surface_idx, bool bSingleContact)
{
	NOT_IMPLEMENTED_V;
}

void CEntity::SetPos(const Legacy::Vec3& pos, bool bWorldOnly)
{
	m_Pos = pos;
}

const Legacy::Vec3& CEntity::GetPos(bool bWorldOnly) const
{
	return m_Pos;
}

void CEntity::SetPhysAngles(const Legacy::Vec3& angl)
{
	NOT_IMPLEMENTED;
}

void CEntity::SetAngles(const Legacy::Vec3& pos, bool bNotifyContainer, bool bUpdatePhysics, bool forceInWorld)
{
	m_Angles = pos;
}

const Legacy::Vec3& CEntity::GetAngles(int realA) const
{
	return m_Angles;
}

void CEntity::SetScale(glm::vec3 scale)
{
	m_Scale = scale;
}

glm::vec3 CEntity::GetScale() const
{
	return m_Scale;
}

void CEntity::SetRadius(float r)
{
	NOT_IMPLEMENTED;
}

float CEntity::GetRadius() const
{
	return 0.0f;
}

float CEntity::GetRadiusPhys() const
{
	return 0.0f;
}

void CEntity::SetSleep(bool bSleep)
{
	NOT_IMPLEMENTED;
}

void CEntity::SetNeedUpdate(bool needUpdate)
{
	m_bUpdate = needUpdate;
	if (needUpdate) m_bSleeping = false;
}

bool CEntity::NeedUpdate()
{
	return false;
}

void CEntity::SetRegisterInSectors(bool needToRegister)
{
	if (needToRegister)
	{
		if (!m_pEntityRenderState)
			InitEntityRenderState();
		ClearFlags(ETY_FLAG_NOT_REGISTER_IN_SECTORS);
		if (!m_registeredInSector)
		{
			RegisterInSector();
		}
	}
	else
	{
		SetFlags(ETY_FLAG_NOT_REGISTER_IN_SECTORS);
		if (m_registeredInSector)
		{
			UnregisterInSector();
		}
	}

}

void CEntity::SetUpdateRadius(float fUpdateRadius)
{
	NOT_IMPLEMENTED;
}

float CEntity::GetUpdateRadius() const
{
	return 0.0f;
}

void CEntity::ForceRegisterInSectors()
{
	NOT_IMPLEMENTED;
}

bool CEntity::IsMoving() const
{
	NOT_IMPLEMENTED_V;
}

bool CEntity::IsBound()
{
	NOT_IMPLEMENTED_V;
}

void CEntity::Bind(EntityId id, unsigned char cBind, const bool bClientOnly, const bool bSetPos)
{
	NOT_IMPLEMENTED;
}

void CEntity::Unbind(EntityId id, unsigned char cBind, const bool bClientOnly)
{
	NOT_IMPLEMENTED;
}

void CEntity::ForceBindCalculation(bool bEnable)
{
	NOT_IMPLEMENTED;
}

void CEntity::SetParentLocale(const Legacy::Matrix44& matParent)
{
	NOT_IMPLEMENTED;
}

void CEntity::CalculateInWorld(void)
{
	NOT_IMPLEMENTED;
}

void CEntity::AttachToBone(EntityId id, const char* boneName)
{
	NOT_IMPLEMENTED;
}

BoneBindHandle CEntity::AttachObjectToBone(int slot, const char* boneName, bool bMultipleAttachments, bool bUseZOffset)
{
	NOT_IMPLEMENTED_V;
}

void CEntity::DetachObjectToBone(const char* boneName, BoneBindHandle objectBindHandle)
{
	NOT_IMPLEMENTED;
}

void CEntity::SetScriptObject(IScriptObject* pObject)
{
	m_pScriptOject = pObject;
}

IScriptObject* CEntity::GetScriptObject()
{
	return m_pScriptOject ? m_pScriptOject : (m_pScriptOject = Env::ScriptSystem()->CreateObject());
}

bool CEntity::Write(CStream&, EntityCloneState* cs)
{
	NOT_IMPLEMENTED_V;
}

bool CEntity::Read(CStream&, bool bNoUpdate)
{
	NOT_IMPLEMENTED_V;
}

bool CEntity::PostLoad()
{
	return false;
}

bool CEntity::Save(CStream& stm, IScriptObject* pStream)
{
	NOT_IMPLEMENTED_V;
}

bool CEntity::Load(CStream& stm, IScriptObject* pStream)
{
	NOT_IMPLEMENTED_V;
}

bool CEntity::LoadRELEASE(CStream& stm, IScriptObject* pStream)
{
	NOT_IMPLEMENTED_V;
}

bool CEntity::LoadPATCH1(CStream& stm, IScriptObject* pStream)
{
	NOT_IMPLEMENTED_V;
}

IEntityContainer* CEntity::GetContainer() const
{
	return m_pContainer;
}

void CEntity::SetContainer(IEntityContainer* pContainer)
{
	m_pContainer = pContainer;
	if (m_pContainer)
		m_bUpdateContainer = true;
	else
		m_bUpdateContainer = false;
}

IEntityCharacter* CEntity::GetCharInterface() const
{
	return m_EntityCharacter;
}

bool CEntity::StartAnimation(int pos, const char* animname, int iLayerID, float fBlendTime, bool bStartWithLayer0Phase)
{
	NOT_IMPLEMENTED_V;
}

void CEntity::SetAnimationSpeed(const float scale)
{
	NOT_IMPLEMENTED;
}

int CEntity::GetCurrentAnimation(int pos, int iLayerID)
{
	return 0;
}

float CEntity::GetAnimationLength(const char* animation)
{
	return 0.0f;
}

bool CEntity::IsAnimationPresent(int pos, const char* animation)
{
	return false;
}

void CEntity::ResetAnimations(int pos)
{
}

void CEntity::SetDefaultIdleAnimation(int pos, const char* szAnimation)
{
}

void CEntity::ForceCharacterUpdate(int pos)
{
}

bool CEntity::RegisterInAISystem(unsigned short type, const AIObjectParameters& params)
{
	return false;
}

IAIObject* CEntity::GetAI()
{
	return nullptr;
}

void CEntity::EnableAI(bool enable)
{
}

void CEntity::EnableSave(bool bEnable)
{
}

bool CEntity::IsSaveEnabled()
{
	return false;
}

bool CEntity::IsTrackable()
{
	return false;
}

bool CEntity::GotoState(const char* sState)
{
	return false;
}

bool CEntity::GotoState(int nState)
{
	return false;
}

bool CEntity::IsInState(const char* sState)
{
	return false;
}

bool CEntity::IsInState(int nState)
{
	return false;
}

const char* CEntity::GetState()
{
	return nullptr;
}

int CEntity::GetStateIdx()
{
	return 0;
}

void CEntity::RegisterState(const char* sState)
{
}

bool CEntity::IsStateClientside() const
{
	return false;
}

void CEntity::SetStateClientside(const bool bEnable)
{
	NOT_IMPLEMENTED;
}

void CEntity::OnTimer(int nTimerId)
{
	NOT_IMPLEMENTED;
}

void CEntity::OnDamage(IScriptObject* pObj)
{
	NOT_IMPLEMENTED;
}

void CEntity::OnEnterArea(IEntity* entity, const int areaID)
{
	NOT_IMPLEMENTED;
}

void CEntity::OnPhysicsBBoxOverlap(IEntity* pOther)
{
	NOT_IMPLEMENTED;
}

void CEntity::OnPhysicsStateChange(int nNewSymClass, int nPrevSymClass)
{
	NOT_IMPLEMENTED;
}

void CEntity::SetPhysicsState(const char* sPhysicsState)
{
	NOT_IMPLEMENTED;
}

void CEntity::OnLeaveArea(IEntity* entity, const int areaID)
{
	NOT_IMPLEMENTED;
}

void CEntity::OnProceedFadeArea(IEntity* entity, const int areaID, const float fadeCoeff)
{
	NOT_IMPLEMENTED;
}

void CEntity::OnBind(IEntity* entity, const char par)
{
	NOT_IMPLEMENTED;
}

void CEntity::OnUnBind(IEntity* entity, const char par)
{
	NOT_IMPLEMENTED;
}

void CEntity::SetTimer(int msec)
{
	NOT_IMPLEMENTED;
}

void CEntity::KillTimer()
{
	NOT_IMPLEMENTED;
}

void CEntity::SetScriptUpdateRate(float fUpdateEveryNSeconds)
{
	NOT_IMPLEMENTED;
}

void CEntity::ApplyForceToEnvironment(const float radius, const float force)
{
	NOT_IMPLEMENTED;
}

int CEntity::GetSide(const Legacy::Vec3& direction)
{
	return 0;
}

void CEntity::Hide(bool b)
{
	NOT_IMPLEMENTED;
}

bool CEntity::IsHidden() const
{
	return false;
}

void CEntity::SetDestroyable(bool b)
{
}

bool CEntity::IsDestroyable() const
{
	return false;
}

void CEntity::SetGarbageFlag(bool bGarbage)
{
	NOT_IMPLEMENTED;
}

bool CEntity::WasVisible()
{
	return false;
}

bool CEntity::HasChanged()
{
	return false;
}

int CEntity::GetSteeringWheelSlot()
{
	return 0;
}

int CEntity::GetBoneHitZone(int boneIdx) const
{
	return 0;
}

void CEntity::GetHitParms(int& deathType, int& deathDir, int& deathZone) const
{
	NOT_IMPLEMENTED;
}

void CEntity::InitEntityRenderState()
{
	if (!m_pEntityRenderState)
	{
		m_pEntityRenderState = m_pISystem->GetI3DEngine()->MakeEntityRenderState();
#if 0
		CheckEntityLightSourcesInEntityObjects();
#endif
	}
}

void CEntity::ActivatePhysics(bool activate)
{
	NOT_IMPLEMENTED;
}

void CEntity::SetCommonCallbacks(IScriptSystem* pScriptSystem)
{
	NOT_IMPLEMENTED;
}

void CEntity::GetMemoryUsage(ICrySizer* pSizer) const
{
	pSizer->AddObject(this, sizeof(this));
	pSizer->AddObject(m_Desc);
}

void CEntity::SetWaterDensity(float fWaterDensity)
{
	NOT_IMPLEMENTED;
}

float CEntity::GetWaterDensity()
{
	return 0.0f;
}

void CEntity::SetUpdateVisLevel(EEntityUpdateVisLevel nUpdateVisLevel)
{
	NOT_IMPLEMENTED;
}

EEntityUpdateVisLevel CEntity::GetUpdateVisLevel()
{
	return EEntityUpdateVisLevel();
}

void CEntity::SetHandsIKTarget(const Legacy::Vec3* target)
{
}

void CEntity::Remove()
{
	m_IsGarbage = true;
	delete this;
}

void CEntity::SetShaderFloat(const char* Name, float Val)
{
	NOT_IMPLEMENTED;
}

void CEntity::SwitchLights(bool bLights)
{
	NOT_IMPLEMENTED;
}

void CEntity::SinkRebind(IEntitySystemSink* pSink)
{
	NOT_IMPLEMENTED;
}

void CEntity::Physicalize(bool bInstant)
{
	m_pPhysics = new CPhysicalEntity(this);
	m_pEntitySystem->AddToPhysicalWorld(m_pPhysics);
}

void CEntity::RegisterInSector()
{
	// register this entity for rendering
	if (!m_registeredInSector &&
		GetRadius() &&
		!IsHidden() &&
		m_pEntityRenderState /*&&
												 IsEntityHasSomethingToRender()*/)
	{
		m_pISystem->GetI3DEngine()->RegisterEntity(this);
		m_registeredInSector = true;
	}
}


void CEntity::UnregisterInSector()
{
	// unregister this entity from the list of sectors
	if (m_registeredInSector)
	{
		m_pISystem->GetI3DEngine()->UnRegisterEntity(this);
		m_registeredInSector = false;
	}
}

void CEntity::OnStartAnimation(const char* sAnimation)
{
	NOT_IMPLEMENTED;
}

void CEntity::OnAnimationEvent(const char* sAnimation, AnimSinkEventData data)
{
	NOT_IMPLEMENTED;
}

void CEntity::OnEndAnimation(const char* sAnimation)
{
	NOT_IMPLEMENTED;
}
