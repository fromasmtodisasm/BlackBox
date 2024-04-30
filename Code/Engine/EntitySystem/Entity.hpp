#pragma once

#include <BlackBox/EntitySystem/IEntitySystem.hpp>

#include "EntityCharacter.hpp"

#define ENTITY_PROFILER FUNCTION_PROFILER_FAST(PROFILE_ENTITY,g_bProfilerEnabled );
#define ENTITY_PROFILER_NAME(str) FRAME_PROFILER_FAST( str,PROFILE_ENTITY,g_bProfilerEnabled );


class CEntity;
class CEntitySystem;
class CPhysicalEntity;
class CEntityMotionState;

class CEntity : public IEntity
{
	friend class CEntityMotionState;
public:
	CEntity() = default;
	CEntity(CEntityDesc& desc);
	~CEntity();
	// Inherited via IEntity
	virtual void                  GetRenderBBox(Legacy::Vec3& mins, Legacy::Vec3& maxs) override;
	virtual float                 GetRenderRadius() const override;
	virtual bool                  IsStatic() const override;
	virtual bool                  IsEntityHasSomethingToRender() override;
	virtual bool                  IsEntityAreasVisible() override;
	virtual void                  SetMaterial(IMatInfo* pMatInfo) override;
	virtual IMatInfo*             GetMaterial() const override;
	virtual void                  PreloadInstanceResources(Legacy::Vec3 vPrevPortalPos, float fPrevPortalDistance, float fTime) override;
	virtual EntityId              GetId() const override;
	virtual void                  Update(SEntityUpdateContext& updateContext) override;
	virtual void                  Reset() override;
	virtual bool                  GetNetPresence() const override;
	virtual void                  SetNetPresence(bool bPresent) override;
	virtual void                  SetName(const char* name) override;
	virtual void                  SetClassName(const char* name) override;
	virtual const char*           GetName() const override;
	virtual const char*           GetEntityClassName() const override;
	virtual void                  GetEntityDesc(CEntityDesc& desc) const override;
	virtual void                  GetHelperPosition(const char* helper, Legacy::Vec3& pos, bool objectspace = false) override;
	virtual EntityClassId         GetClassId() override;
	virtual void                  SetClassId(const EntityClassId ClassId) override;
	virtual void                  ShutDown() override;
	virtual bool                  IsGarbage() override;
	virtual int                   GetObjectsFlags(int nSubObj) override;
	virtual void                  SetFlags(unsigned int flags) override;
	virtual void                  ClearFlags(unsigned int flags) override;
	virtual unsigned int          GetFlags() override;
	virtual IEntityCamera*        GetCamera() const override;
	virtual void                  SetCamera(IEntityCamera* cam) override;
	virtual CDLight*              GetLight() override;
	virtual bool                  InitLight(const char* img = NULL, const char* shader = NULL, bool bUseAsCube = false, float fAnimSpeed = 0, int nLightStyle = 0, float fCoronaScale = 0) override;
	virtual void                  SendScriptEvent(EScriptEventId Event, IScriptObject* pParamters, bool* pRet = NULL) override;
	virtual void                  SendScriptEvent(EScriptEventId Event, const char* str, bool* pRet = NULL) override;
	virtual void                  SendScriptEvent(EScriptEventId Event, int nParam, bool* pRet = NULL) override;
	virtual void                  CallEventHandler(const char* sEvent) override;
	virtual IPhysicalEntity*      GetPhysics() const override;
	virtual void                  SetPhysics(IPhysicalEntity* physic) override;
	virtual void                  DestroyPhysics() override;
	virtual int                   CreatePhysicalEntityCallback(int iForeignFlags) override;
	virtual int                   DestroyPhysicalEntityCallback(IPhysicalEntity* pent) override;
	virtual void                  EnablePhysics(bool enable) override;
	virtual void                  AddImpulse(int ipart, Legacy::Vec3 pos, Legacy::Vec3 impulse, bool bPos = true, float fAuxScale = 1.0f) override;
	virtual bool                  CreateRigidBody(pe_type type, float density, float mass, int surface_id, Legacy::Vec3* pInitialVelocity = NULL, int slot = -1, bool bPermanent = false) override;
	virtual bool                  CreateLivingEntity(float mass, float height, float eye_height, float sphere_height, float radius, int nSurfaceID, float fGravity, float fAirControl, bool collide = false) override;
	virtual bool                  CreateStaticEntity(float mass, int surface_idx, int slotToUse = -1, bool bPermanent = false) override;
	virtual bool                  CreateSoftEntity(float mass, float density, bool bCloth = true, IPhysicalEntity* pAttachTo = WORLD_ENTITY, int iAttachToPart = -1) override;
	virtual bool                  LoadVehicle(const char* objfile, pe_cargeomparams* pparts, pe_params_car* params, bool bDestroy = false) override;
	virtual bool                  LoadBoat(const char* objfile, float mass, int surfaceID) override;
	virtual void                  SetDamage(const int dmg) override;
	virtual bool                  LoadObject(unsigned int slot, const char* fileName, float scale, const char* geomName = NULL) override;
	virtual bool                  GetObjectPos(unsigned int slot, Legacy::Vec3& pos) override;
	virtual bool                  SetObjectPos(unsigned int slot, const Legacy::Vec3& pos) override;
	virtual bool                  GetObjectAngles(unsigned int slot, Legacy::Vec3& ang) override;
	virtual bool                  SetObjectAngles(unsigned int slot, const Legacy::Vec3& ang) override;
	virtual void                  LoadBreakableObject(const char* fileName) override;
	virtual bool                  SetEntityObject(unsigned int slot, const CEntityObject& object) override;
	virtual bool                  GetEntityObject(unsigned int slot, CEntityObject& object) override;
	virtual int                   GetNumObjects() override;
	virtual IStatObj*             GetIStatObj(unsigned int pos) override;
	virtual void                  SetIStatObj(IStatObj* obj, unsigned int pos) override;
	virtual void                  PlaySound(ISound* pSound, float fSoundScale, Legacy::Vec3& Offset) override;
	virtual void                  DrawObject(unsigned int pos, int mode) override;
	virtual void                  DrawObject(int mode) override;
	virtual void                  DrawCharacter(int pos, int mode) override;
	virtual void                  NeedsUpdateCharacter(int pos, bool updt) override;
	virtual void                  SetBBox(const Legacy::Vec3& mins, const Legacy::Vec3& maxs) override;
	virtual void                  GetBBox(Legacy::Vec3& mins, Legacy::Vec3& maxs) override;
	virtual void                  GetLocalBBox(Legacy::Vec3& min, Legacy::Vec3& max) override;
	virtual void                  InvalidateBBox() override;
	virtual void                  TrackColliders(bool bEnable) override;
	virtual bool                  DrawEntity(const SRendParams& EntDrawParams) override;
	virtual bool                  CreateParticleEntity(float size, float mass, Legacy::Vec3 heading, float acc_thrust = 0, float k_air_resistance = 0, float acc_lift = 0, float gravity = -9.8, int surface_idx = 0, bool bSingleContact = true) override;
	virtual void                  SetPos(const Legacy::Vec3& pos, bool bWorldOnly = true) override;
	virtual const Legacy::Vec3&   GetPos(bool bWorldOnly = true) const override;
	virtual void                  SetPhysAngles(const Legacy::Vec3& angl) override;
	virtual void                  SetAngles(const Legacy::Vec3& pos, bool bNotifyContainer = true, bool bUpdatePhysics = true, bool forceInWorld = false) override;
	virtual const Legacy::Vec3&   GetAngles(int realA = 0) const override;
	virtual void                  SetScale(glm::vec3 scale) override;
	virtual glm::vec3             GetScale() const override;
	virtual void                  SetRadius(float r) override;
	virtual float                 GetRadius() const override;
	virtual float                 GetRadiusPhys() const override;
	virtual void                  SetSleep(bool bSleep) override;
	virtual void                  SetNeedUpdate(bool needUpdate) override;
	virtual bool                  NeedUpdate() override;
	virtual void                  SetRegisterInSectors(bool needToRegister) override;
	virtual void                  SetUpdateRadius(float fUpdateRadius) override;
	virtual float                 GetUpdateRadius() const override;
	virtual void                  ForceRegisterInSectors() override;
	virtual bool                  IsMoving() const override;
	virtual bool                  IsBound() override;
	virtual void                  Bind(EntityId id, unsigned char cBind = 0, const bool bClientOnly = false, const bool bSetPos = false) override;
	virtual void                  Unbind(EntityId id, unsigned char cBind, const bool bClientOnly = false) override;
	virtual void                  ForceBindCalculation(bool bEnable) override;
	virtual void                  SetParentLocale(const Legacy::Matrix44& matParent) override;
	virtual void                  CalculateInWorld(void) override;
	virtual void                  AttachToBone(EntityId id, const char* boneName) override;
	virtual BoneBindHandle        AttachObjectToBone(int slot, const char* boneName, bool bMultipleAttachments = false, bool bUseZOffset = false) override;
	virtual void                  DetachObjectToBone(const char* boneName, BoneBindHandle objectBindHandle = -1) override;
	virtual void                  SetScriptObject(IScriptObject* pObject) override;
	virtual IScriptObject*        GetScriptObject() override;
	virtual bool                  Write(CStream&, EntityCloneState* cs = NULL) override;
	virtual bool                  Read(CStream&, bool bNoUpdate = false) override;
	virtual bool                  PostLoad() override;
	virtual bool                  Save(CStream& stm, IScriptObject* pStream = NULL) override;
	virtual bool                  Load(CStream& stm, IScriptObject* pStream = NULL) override;
	virtual bool                  LoadRELEASE(CStream& stm, IScriptObject* pStream = NULL) override;
	virtual bool                  LoadPATCH1(CStream& stm, IScriptObject* pStream = NULL) override;
	virtual IEntityContainer*     GetContainer() const override;
	virtual void                  SetContainer(IEntityContainer* pContainer) override;
	virtual IEntityCharacter*     GetCharInterface() const override;
	virtual bool                  StartAnimation(int pos, const char* animname, int iLayerID = 0, float fBlendTime = 1.5f, bool bStartWithLayer0Phase = false) override;
	virtual void                  SetAnimationSpeed(const float scale = 1.0f) override;
	virtual int                   GetCurrentAnimation(int pos, int iLayerID) override;
	virtual float                 GetAnimationLength(const char* animation) override;
	virtual bool                  IsAnimationPresent(int pos, const char* animation) override;
	virtual void                  ResetAnimations(int pos) override;
	virtual void                  SetDefaultIdleAnimation(int pos, const char* szAnimation = NULL) override;
	virtual void                  ForceCharacterUpdate(int pos) override;
	virtual bool                  RegisterInAISystem(unsigned short type, const AIObjectParameters& params) override;
	virtual IAIObject*            GetAI() override;
	virtual void                  EnableAI(bool enable) override;
	virtual void                  EnableSave(bool bEnable) override;
	virtual bool                  IsSaveEnabled() override;
	virtual bool                  IsTrackable() override;
	virtual bool                  GotoState(const char* sState) override;
	virtual bool                  GotoState(int nState) override;
	virtual bool                  IsInState(const char* sState) override;
	virtual bool                  IsInState(int nState) override;
	virtual const char*           GetState() override;
	virtual int                   GetStateIdx() override;
	virtual void                  RegisterState(const char* sState) override;
	virtual bool                  IsStateClientside() const override;
	virtual void                  SetStateClientside(const bool bEnable) override;
	virtual void                  OnTimer(int nTimerId) override;
	virtual void                  OnDamage(IScriptObject* pObj) override;
	virtual void                  OnEnterArea(IEntity* entity, const int areaID) override;
	virtual void                  OnPhysicsBBoxOverlap(IEntity* pOther) override;
	virtual void                  OnPhysicsStateChange(int nNewSymClass, int nPrevSymClass) override;
	virtual void                  SetPhysicsState(const char* sPhysicsState) override;
	virtual void                  OnLeaveArea(IEntity* entity, const int areaID) override;
	virtual void                  OnProceedFadeArea(IEntity* entity, const int areaID, const float fadeCoeff) override;
	virtual void                  OnBind(IEntity* entity, const char par) override;
	virtual void                  OnUnBind(IEntity* entity, const char par) override;
	virtual void                  SetTimer(int msec) override;
	virtual void                  KillTimer() override;
	virtual void                  SetScriptUpdateRate(float fUpdateEveryNSeconds) override;
	virtual void                  ApplyForceToEnvironment(const float radius, const float force) override;
	virtual int                   GetSide(const Legacy::Vec3& direction) override;
	virtual void                  Hide(bool b) override;
	virtual bool                  IsHidden() const override;
	virtual void                  SetDestroyable(bool b) override;
	virtual bool                  IsDestroyable() const override;
	virtual void                  SetGarbageFlag(bool bGarbage) override;
	virtual bool                  WasVisible() override;
	virtual bool                  HasChanged() override;
	virtual int                   GetSteeringWheelSlot() override;
	virtual int                   GetBoneHitZone(int boneIdx) const override;
	virtual void                  GetHitParms(int& deathType, int& deathDir, int& deathZone) const override;
	virtual void                  InitEntityRenderState() override;
	virtual void                  ActivatePhysics(bool activate) override;
	virtual void                  SetCommonCallbacks(IScriptSystem* pScriptSystem) override;
	virtual void                  GetMemoryUsage(ICrySizer* pSizer) const override;
	virtual void                  SetWaterDensity(float fWaterDensity) override;
	virtual float                 GetWaterDensity() override;
	virtual void                  SetUpdateVisLevel(EEntityUpdateVisLevel nUpdateVisLevel) override;
	virtual EEntityUpdateVisLevel GetUpdateVisLevel() override;
	virtual void                  SetHandsIKTarget(const Legacy::Vec3* target = NULL) override;
	virtual void                  Remove() override;
	virtual void                  SetShaderFloat(const char* Name, float Val) override;
	virtual void                  SwitchLights(bool bLights) override;
	virtual void                  SinkRebind(IEntitySystemSink* pSink) override;

	virtual void                  Physicalize(bool bInstant = false) override;
	private:
		//! Put entity into terrain sector.
		void RegisterInSector();
		//! Remove entity from terrain sector.
		void UnregisterInSector();

	private:
		//////////////////////////////////////////////////////////////////////////
// VARIABLES.
//////////////////////////////////////////////////////////////////////////
		friend class CEntitySystem;

		//////////////////////////////////////////////////////////////////////////
		// Flags first (Reduce cache misses on access to entity data).
		//////////////////////////////////////////////////////////////////////////
		unsigned int m_bUpdate : 1;
		unsigned int m_bSleeping : 1;
		unsigned int m_bGarbage : 1;
		unsigned int m_bIsBound : 1;
		//	unsigned int m_bForceBBox : 1; // moved into IEntityRender
		unsigned int m_bRecalcBBox : 1;
		unsigned int m_bInitialized : 1;						//!< If this entity already Initialized.
		unsigned int m_netPresence : 1;							//!< Where entity should be present.
		unsigned int m_bHidden : 1;
		unsigned int m_bTrackable : 1;							//!< Trackable in MotionTracker...
		unsigned int m_bHandIK : 1;
		unsigned int m_bForceBindCalculation : 1;
		unsigned int m_bSave : 1;										//!< Should be saved on disk(when the level is saved) by default is true
		unsigned int m_bEntityHasLights : 1;				//!< entity objects has light sources
		unsigned int m_bEntityLightsOn : 1;					//!< if the entity objects light sources are enabled
		unsigned int m_bTrackColliders : 1;					//!< If entity want to track collider and generate Enter/Leave events.
		unsigned int m_bUpdateSounds : 1;						//!< If true will update attached sounds.
		unsigned int m_bUpdateAI : 1;								//!< If set will update AI objects.
		unsigned int m_bUpdateEmitters : 1;					//!< Particle emitters present and active.
		unsigned int m_bUpdateScript : 1;						//!< True if script update function should be called.
		unsigned int m_bUpdateContainer : 1;				//!< True if container must be updated.
		unsigned int m_bUpdateCharacters : 1;				//!< True if characters must be updated.
		unsigned int m_bUpdateCamera : 1;						//!< True if must update entity camera.
		unsigned int m_bUpdateBinds : 1;						//!< True if must update binded entities.
		unsigned int m_bUpdateOnContact : 1;				//!< True if must check for OnContact collisions.
		unsigned int m_bIsADeadBody : 1;						//!< True is entity is a dead body (set in KillCharacter)
		unsigned int m_bVisible : 1;								//!< Remembers visibility state from the last update
		unsigned int m_bWasVisible : 1;							//!< Remembers visibility state from the update before the last one
		unsigned int m_bHasEnvLighting : 1;					//!< 
		unsigned int m_bStateClientside : 1;				//!< prevents error when state changes on the client and does not sync state changes to the client 

	public:


	EntityId                      m_Id;
	EntityClassId                 m_ClassId;
	//string_view	  m_Name;
	string                        m_Name{"Unnamed"};
	//string_view	  m_ClassName;
	string                        m_ClassName{"Unknown"};

	IMatInfo*                     m_pMatInfo;
	IEntityCamera*                m_pEntityCamera{};
	CEntityObject                 m_EntityObject;
	IScriptObject*                m_pScriptOject;
	#if 0
	IEntityCharacter*             m_pCharacter;
	#endif

	bool                          m_bIsStatic;
	Legacy::Vec3                  m_Angles{0};
	Legacy::Vec3                  m_Pos    = Legacy::Vec3(0);
	glm::vec3                     m_Scale  = glm::vec3(1.f);
	glm::vec3                     m_BoxMin = {};
	glm::vec3                     m_BoxMax = {};

	CEntityDesc                   m_Desc;

	// Inherited via IEntity
	virtual void                  OnStartAnimation(const char* sAnimation) override;
	virtual void                  OnAnimationEvent(const char* sAnimation, AnimSinkEventData data) override;
	virtual void                  OnEndAnimation(const char* sAnimation) override;

	bool                          m_IsGarbage = false;

	CEntitySystem*                m_pEntitySystem;
	CPhysicalEntity*              m_pPhysics{};
	CEntityCharacter*             m_EntityCharacter{};

	IEntityContainer* m_pContainer;

	ISystem* m_pISystem{};


	// Flags.
	uint m_registeredInSector : 1; //when we get entity & script from server
	//	uint m_static : 1; // when Entity is dtatic or moving object.
	uint m_physicEnabled : 1; // when Entity have physics enabled.


};
