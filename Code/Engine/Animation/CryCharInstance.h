#pragma once

#include "BlackBox/3DEngine/IBindable.hpp"
#include "ICryAnimation.h"

class CryCharInstance : public ICryCharInstance
{
public:
	CryCharInstance(IEntity* pObj);
	// Inherited via ICryCharInstance
	bool SetShaderTemplate(int nTemplate, const char* TemplName, const char* ShaderName, bool bOnlyRegister, int* pnNewTemplateId) override;
	void Render(const SRendParams& rParams, const Legacy::Vec3& t, int nLodLevel) override;
	void Release() override;
	void SetFlags(int nFlags) override;
	int GetFlags() override;
	bool SetShaderTemplateName(const char* TemplName, int Id, const char* ShaderName, IMatInfo* pCustomMaterial, unsigned nFlags) override;
	const char* GetShaderTemplateName() override;
	void SetShaderFloat(const char* Name, float fVal, const char* ShaderName) override;
	void SetColor(float fR, float fG, float fB, float fA) override;
	void Draw(const SRendParams& RendParams, const Legacy::Vec3& t) override;
	const CDLight* GetBoundLight(int nIndex) override;
	void RenderShadowVolumes(const SRendParams* rParams, int nLimitLOD) override;
	void GetBBox(Legacy::Vec3& Mins, Legacy::Vec3& Maxs) override;
	const Legacy::Vec3 GetCenter() override;
	const float GetRadius() override;
	ObjectBindingHandle AttachObjectToBone(IBindable* pWeaponModel, const char* szBoneName, bool bUseRelativeToDefPoseMatrix, unsigned nFlags) override;
	void EnableLastIdleAnimationRestart(unsigned nLayer, bool bEnable) override;
	const char* GetCurAnimation() override;
	int GetCurrentAnimation(unsigned nLayer) override;
	void ResetAnimations() override;
	void SetAnimationSpeed(float fSpeed) override;
	void SetAnimationSpeed(int, float fSpeed) override;
	float GetAnimationSpeed() override;
	void EnableTimeUpdate(bool bUpdate) override;
	void SetLayerTime(int nLayer, float fTimeSeconds) override;
	float GetLayerTime(int nLayer) override;
	void UpdatePhysics(float fScale) override;
	void BuildPhysicalEntity(IPhysicalEntity* pent, float mass, int surface_idx, float stiffness_scale, int nLod) override;
	IPhysicalEntity* CreateCharacterPhysics(IPhysicalEntity* pHost, float mass, int surface_idx, float stiffness_scale, int nLod) override;
	int CreateAuxilaryPhysics(IPhysicalEntity* pHost, int nLod) override;
	IPhysicalEntity* GetCharacterPhysics() override;
	IPhysicalEntity* GetCharacterPhysics(const char* pRootBoneName) override;
	IPhysicalEntity* GetCharacterPhysics(int iAuxPhys) override;
	void SynchronizeWithPhysicalEntity(IPhysicalEntity* pent, const Legacy::Vec3& posMaster, const Quat& qMaster) override;
	IPhysicalEntity* RelinquishCharacterPhysics() override;
	void DestroyCharacterPhysics(int iMode) override;
	void SetCharacterPhysParams(float mass, int surface_idx) override;
	void SetLimbIKGoal(int limbid, vectorf ptgoal, int ik_flags, float addlen, vectorf goal_normal) override;
	vectorf GetLimbEndPos(int limbid) override;
	void AddImpact(int partid, vectorf point, vectorf impact) override;
	int TranslatePartIdToDeadBody(int partid) override;
	vectorf GetOffset() override;
	void SetOffset(vectorf offset) override;
	ICryBone* GetBoneByName(const char* szName) override;
	bool SetAnimationFrame(const char* szString, int nFrame) override;
	void AddAnimationEventSink(ICharInstanceSink* pCharInstanceSink) override;
	void RemoveAnimationEventSink(ICharInstanceSink* pCharInstanceSink) override;
	void AddAnimationEventSink(const char* szAnimName, ICharInstanceSink* pCharInstanceSink) override;
	void RemoveAnimationEventSink(const char* szAnimName, ICharInstanceSink* pCharInstanceSink) override;
	bool AddAnimationEvent(const char* szAnimName, int nFrameID, AnimSinkEventData pUserData) override;
	bool RemoveAnimationEvent(const char* szAnimName, int nFrameID, AnimSinkEventData pUserData) override;
	ICryCharModel* GetModel() override;
	Legacy::Vec3 GetTPVWeaponHelper(const char* szHelperName, ObjectBindingHandle nHandle) override;
	void SetTwiningMode(AnimTwinMode eTwinMode) override;
	int GetDamageTableValue(int nId) override;
	bool IsCharacterActive() override;
	void CreateDecal(CryEngineDecalInfo& DecalLCS) override;
	bool IsModelFileEqual(const char* szFileName) override;
	void GetMemoryUsage(ICrySizer* pSizer) const override;
	void SetDefaultIdleAnimation(unsigned nLayer, const char* szAnimName) override;
private:
	IEntity* m_pObj;
};