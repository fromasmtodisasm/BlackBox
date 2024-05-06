#include "pch.hpp"

#include "CryCharInstance.h"
#include "BlackBox/EntitySystem/IEntitySystem.hpp"
#include "BlackBox/EntitySystem/EntityDesc.hpp"

CryCharInstance::CryCharInstance(IEntity* pObj)
	: m_pObj(pObj)
{
}

bool CryCharInstance::SetShaderTemplate(int nTemplate, const char* TemplName, const char* ShaderName, bool bOnlyRegister, int* pnNewTemplateId)
{
	return false;
}

void CryCharInstance::Render(const SRendParams& rParams, const Legacy::Vec3& t, int nLodLevel)
{
	m_pObj->DrawEntity(rParams);
}

void CryCharInstance::Release()
{
}

void CryCharInstance::SetFlags(int nFlags)
{
}

int CryCharInstance::GetFlags()
{
	return 0;
}

bool CryCharInstance::SetShaderTemplateName(const char* TemplName, int Id, const char* ShaderName, IMatInfo* pCustomMaterial, unsigned nFlags)
{
	return false;
}

const char* CryCharInstance::GetShaderTemplateName()
{
	return nullptr;
}

void CryCharInstance::SetShaderFloat(const char* Name, float fVal, const char* ShaderName)
{
}

void CryCharInstance::SetColor(float fR, float fG, float fB, float fA)
{
}

void CryCharInstance::Draw(const SRendParams& RendParams, const Legacy::Vec3& t)
{
	m_pObj->DrawEntity(RendParams);
}

const CDLight* CryCharInstance::GetBoundLight(int nIndex)
{
	return nullptr;
}

void CryCharInstance::RenderShadowVolumes(const SRendParams* rParams, int nLimitLOD)
{
}

void CryCharInstance::GetBBox(Legacy::Vec3& Mins, Legacy::Vec3& Maxs)
{
}

const Legacy::Vec3 CryCharInstance::GetCenter()
{
	return Legacy::Vec3();
}

const float CryCharInstance::GetRadius()
{
	return 0.0f;
}

ICryCharInstance::ObjectBindingHandle  CryCharInstance::AttachObjectToBone(IBindable* pWeaponModel, const char* szBoneName, bool bUseRelativeToDefPoseMatrix, unsigned nFlags)
{
	return ICryCharInstance::ObjectBindingHandle();
}

void CryCharInstance::EnableLastIdleAnimationRestart(unsigned nLayer, bool bEnable)
{
}

const char* CryCharInstance::GetCurAnimation()
{
	return nullptr;
}

int CryCharInstance::GetCurrentAnimation(unsigned nLayer)
{
	return 0;
}

void CryCharInstance::ResetAnimations()
{
}

void CryCharInstance::SetAnimationSpeed(float fSpeed)
{
}

void CryCharInstance::SetAnimationSpeed(int, float fSpeed)
{
}

float CryCharInstance::GetAnimationSpeed()
{
	return 0.0f;
}

void CryCharInstance::EnableTimeUpdate(bool bUpdate)
{
}

void CryCharInstance::SetLayerTime(int nLayer, float fTimeSeconds)
{
}

float CryCharInstance::GetLayerTime(int nLayer)
{
	return 0.0f;
}

void CryCharInstance::UpdatePhysics(float fScale)
{
}

void CryCharInstance::BuildPhysicalEntity(IPhysicalEntity* pent, float mass, int surface_idx, float stiffness_scale, int nLod)
{
}

IPhysicalEntity* CryCharInstance::CreateCharacterPhysics(IPhysicalEntity* pHost, float mass, int surface_idx, float stiffness_scale, int nLod)
{
	return nullptr;
}

int CryCharInstance::CreateAuxilaryPhysics(IPhysicalEntity* pHost, int nLod)
{
	return 0;
}

IPhysicalEntity* CryCharInstance::GetCharacterPhysics()
{
	return nullptr;
}

IPhysicalEntity* CryCharInstance::GetCharacterPhysics(const char* pRootBoneName)
{
	return nullptr;
}

IPhysicalEntity* CryCharInstance::GetCharacterPhysics(int iAuxPhys)
{
	return nullptr;
}

void CryCharInstance::SynchronizeWithPhysicalEntity(IPhysicalEntity* pent, const Legacy::Vec3& posMaster, const Quat& qMaster)
{
}

IPhysicalEntity* CryCharInstance::RelinquishCharacterPhysics()
{
	return nullptr;
}

void CryCharInstance::DestroyCharacterPhysics(int iMode)
{
}

void CryCharInstance::SetCharacterPhysParams(float mass, int surface_idx)
{
}

void CryCharInstance::SetLimbIKGoal(int limbid, vectorf ptgoal, int ik_flags, float addlen, vectorf goal_normal)
{
}

vectorf CryCharInstance::GetLimbEndPos(int limbid)
{
	return vectorf();
}

void CryCharInstance::AddImpact(int partid, vectorf point, vectorf impact)
{
}

int CryCharInstance::TranslatePartIdToDeadBody(int partid)
{
	return 0;
}

vectorf CryCharInstance::GetOffset()
{
	return vectorf();
}

void CryCharInstance::SetOffset(vectorf offset)
{
}

ICryBone* CryCharInstance::GetBoneByName(const char* szName)
{
	return nullptr;
}

bool CryCharInstance::SetAnimationFrame(const char* szString, int nFrame)
{
	return false;
}

void CryCharInstance::AddAnimationEventSink(ICharInstanceSink* pCharInstanceSink)
{
}

void CryCharInstance::RemoveAnimationEventSink(ICharInstanceSink* pCharInstanceSink)
{
}

void CryCharInstance::AddAnimationEventSink(const char* szAnimName, ICharInstanceSink* pCharInstanceSink)
{
}

void CryCharInstance::RemoveAnimationEventSink(const char* szAnimName, ICharInstanceSink* pCharInstanceSink)
{
}

bool CryCharInstance::AddAnimationEvent(const char* szAnimName, int nFrameID, AnimSinkEventData pUserData)
{
	return false;
}

bool CryCharInstance::RemoveAnimationEvent(const char* szAnimName, int nFrameID, AnimSinkEventData pUserData)
{
	return false;
}

ICryCharModel* CryCharInstance::GetModel()
{
	return nullptr;
}

Legacy::Vec3 CryCharInstance::GetTPVWeaponHelper(const char* szHelperName, ObjectBindingHandle nHandle)
{
	return Legacy::Vec3();
}

void CryCharInstance::SetTwiningMode(AnimTwinMode eTwinMode)
{
}

int CryCharInstance::GetDamageTableValue(int nId)
{
	return 0;
}

bool CryCharInstance::IsCharacterActive()
{
	return false;
}

void CryCharInstance::CreateDecal(CryEngineDecalInfo& DecalLCS)
{
}

bool CryCharInstance::IsModelFileEqual(const char* szFileName)
{
	return false;
}

void CryCharInstance::GetMemoryUsage(ICrySizer* pSizer) const
{
}

void CryCharInstance::SetDefaultIdleAnimation(unsigned nLayer, const char* szAnimName)
{
}
