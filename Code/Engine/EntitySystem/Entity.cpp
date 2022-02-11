#include "Entity.hpp"
#include "EntitySystem.hpp"
#include "IStatObj.h"

CEntity::CEntity()
{
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
	return EntityId();
}

void CEntity::Update(SEntityUpdateContext& updateContext)
{
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
	return nullptr;
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
	mins = m_BoxMin * m_Scale;
	maxs = m_BoxMax * m_Scale;
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
	glm::vec3 pos       = GetPos();
	glm::vec3 rotation  = GetAngles();
	glm::vec3 scale     = GetScale();
	auto      transform = glm::mat4(1);
//glm::mat4 transform = glm::rotate(glm::mat4(1), );
#if 1
	transform = glm::translate(transform, pos);
	transform = glm::scale(transform, scale);
#endif
	SRendParams rp(EntDrawParams);
	rp.pMatrix = &transform;
	rp.texture = object->GetTexture();

	object->Render(rp, {});
	return true;
}

bool CEntity::CreateParticleEntity(float size, float mass, Legacy::Vec3 heading, float acc_thrust, float k_air_resistance, float acc_lift, float gravity, int surface_idx, bool bSingleContact)
{
	return false;
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
}

void CEntity::SetNeedUpdate(bool needUpdate)
{
}

bool CEntity::NeedUpdate()
{
	return false;
}

void CEntity::SetRegisterInSectors(bool needToRegister)
{
}

void CEntity::SetUpdateRadius(float fUpdateRadius)
{
}

float CEntity::GetUpdateRadius() const
{
	return 0.0f;
}

void CEntity::ForceRegisterInSectors()
{
}

bool CEntity::IsMoving() const
{
	return false;
}

bool CEntity::IsBound()
{
	return false;
}

void CEntity::Bind(EntityId id, unsigned char cBind, const bool bClientOnly, const bool bSetPos)
{
}

void CEntity::Unbind(EntityId id, unsigned char cBind, const bool bClientOnly)
{
}

void CEntity::ForceBindCalculation(bool bEnable)
{
}

void CEntity::SetParentLocale(const Legacy::Matrix44& matParent)
{
}

void CEntity::CalculateInWorld(void)
{
}

void CEntity::AttachToBone(EntityId id, const char* boneName)
{
}

BoneBindHandle CEntity::AttachObjectToBone(int slot, const char* boneName, bool bMultipleAttachments, bool bUseZOffset)
{
	return BoneBindHandle();
}

void CEntity::DetachObjectToBone(const char* boneName, BoneBindHandle objectBindHandle)
{
}

void CEntity::SetScriptObject(IScriptObject* pObject)
{
	m_pScriptOject = pObject;
}

IScriptObject* CEntity::GetScriptObject()
{
	return m_pScriptOject;
}

bool CEntity::Write(CStream&, EntityCloneState* cs)
{
	return false;
}

bool CEntity::Read(CStream&, bool bNoUpdate)
{
	return false;
}

bool CEntity::PostLoad()
{
	return false;
}

bool CEntity::Save(CStream& stm, IScriptObject* pStream)
{
	return false;
}

bool CEntity::Load(CStream& stm, IScriptObject* pStream)
{
	return false;
}

bool CEntity::LoadRELEASE(CStream& stm, IScriptObject* pStream)
{
	return false;
}

bool CEntity::LoadPATCH1(CStream& stm, IScriptObject* pStream)
{
	return false;
}

IEntityContainer* CEntity::GetContainer() const
{
	return nullptr;
}

void CEntity::SetContainer(IEntityContainer* pContainer)
{
}

IEntityCharacter* CEntity::GetCharInterface() const
{
	return nullptr;
}

bool CEntity::StartAnimation(int pos, const char* animname, int iLayerID, float fBlendTime, bool bStartWithLayer0Phase)
{
	return false;
}

void CEntity::SetAnimationSpeed(const float scale)
{
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
}

void CEntity::OnTimer(int nTimerId)
{
}

void CEntity::OnDamage(IScriptObject* pObj)
{
}

void CEntity::OnEnterArea(IEntity* entity, const int areaID)
{
}

void CEntity::OnPhysicsBBoxOverlap(IEntity* pOther)
{
}

void CEntity::OnPhysicsStateChange(int nNewSymClass, int nPrevSymClass)
{
}

void CEntity::SetPhysicsState(const char* sPhysicsState)
{
}

void CEntity::OnLeaveArea(IEntity* entity, const int areaID)
{
}

void CEntity::OnProceedFadeArea(IEntity* entity, const int areaID, const float fadeCoeff)
{
}

void CEntity::OnBind(IEntity* entity, const char par)
{
}

void CEntity::OnUnBind(IEntity* entity, const char par)
{
}

void CEntity::SetTimer(int msec)
{
}

void CEntity::KillTimer()
{
}

void CEntity::SetScriptUpdateRate(float fUpdateEveryNSeconds)
{
}

void CEntity::ApplyForceToEnvironment(const float radius, const float force)
{
}

int CEntity::GetSide(const Legacy::Vec3& direction)
{
	return 0;
}

void CEntity::Hide(bool b)
{
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
}

void CEntity::InitEntityRenderState()
{
}

void CEntity::ActivatePhysics(bool activate)
{
}

void CEntity::SetCommonCallbacks(IScriptSystem* pScriptSystem)
{
}

void CEntity::GetMemoryUsage(ICrySizer* pSizer) const
{
	pSizer->AddObject(this, sizeof(this));
	pSizer->AddObject(m_Desc);
}

void CEntity::SetWaterDensity(float fWaterDensity)
{
}

float CEntity::GetWaterDensity()
{
	return 0.0f;
}

void CEntity::SetUpdateVisLevel(EEntityUpdateVisLevel nUpdateVisLevel)
{
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
}

void CEntity::SwitchLights(bool bLights)
{
}

void CEntity::SinkRebind(IEntitySystemSink* pSink)
{
}

void CEntity::OnStartAnimation(const char* sAnimation)
{
}

void CEntity::OnAnimationEvent(const char* sAnimation, AnimSinkEventData data)
{
}

void CEntity::OnEndAnimation(const char* sAnimation)
{
}
