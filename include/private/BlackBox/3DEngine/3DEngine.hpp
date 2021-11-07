#pragma once
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/EntitySystem/IEntityRenderState.hpp>

#include <BlackBox/3DEngine/I3DEngine.hpp>
#include <BlackBox/3DEngine/SceneManager.hpp>
#include <BlackBox/World/World.hpp>

#include <BlackBox/3DEngine/StatObject.hpp>

class C3DEngine : public I3DEngine
	, public ISystemUserCallback
{
	typedef void (*RenderCallback)(void* pParams);

  public:
#if BB_PLATFORM_WINDOWS
	virtual bool OnSaveDocument()
	{
		return false;
	}
#endif

	//! Notifies user that system wants to switch out of current process.
	//! Example: Called when pressing ESC in game mode to go to Menu.
	virtual void OnProcessSwitch()
	{
		auto proc  = gEnv->pSystem->GetIProcess();
		auto flags = proc->GetFlags();
		if (flags & PROC_MENU)
		{
			gEnv->pSystem->SetIProcess(this);
		}
		else
		{
			m_Process = proc;
			gEnv->pSystem->SetIProcess(m_Process);
		}
	}

	//! Notifies user, usually editor, about initialization progress in system.
	virtual void OnInitProgress(const char* sProgressMsg){};
	virtual void GetMemoryUsage(ICrySizer* pSizer) override
	{
	}
	IProcess* m_Process{};

  public:
	C3DEngine(ISystem* pSystem, const char* szInterfaceVersion);
	~C3DEngine();
	virtual void		 Enable(bool bEnable) override;
	virtual bool		 Init() override;
	virtual void		 SetLevelPath(const char* szFolderName) override;
	virtual bool		 LoadLevel(const char* szFolderName, const char* szMissionName, bool bEditorMode = false) override;
	virtual void		 Update() override;
	virtual void		 SetCamera(const CCamera& cam, bool bToTheScreen = true) override;
	virtual void		 Draw() override;
	virtual void		 ShutDown(bool bEditorMode = false) override;
	virtual void		 Release() override;
	virtual void		 ActivateLight(const char* szName, bool bActivate) override;
	virtual void		 GetMemoryUsage(ICrySizer* pSizer) const override;
	virtual IStatObj*	 MakeObject(const char* szFileName, const char* szGeomName = 0, EVertsSharing eVertsSharing = evs_ShareAndSortForCache, bool bLoadAdditinalInfo = true, bool bKeepInLocalSpace = false) override;
	virtual bool		 ReleaseObject(IStatObj* pObject) override;
	virtual IStatObj*	 MakeObject() override;
	virtual void		 RegisterEntity(IEntityRender* pEntity) override;
	virtual bool		 UnRegisterEntity(IEntityRender* pEntity) override;
	virtual bool		 UnRegisterInAllSectors(IEntityRender* pEntity = NULL) override;
	virtual void		 SetRenderCallback(RenderCallback pFunc, void* pParams) override;
	virtual void		 SetOutdoorAmbientColor(Legacy::Vec3 vColor) override;
	virtual void		 SetSkyBox(const char* szShaderName) override;
	virtual void		 SetMaxViewDistance(float fMaxViewDistance) override;
	virtual float		 GetMaxViewDistance() override;
	virtual void		 SetFogColor(const Legacy::Vec3& vFogColor) override;
	virtual void		 SetFogStart(const float fFogStart) override;
	virtual void		 SetFogEnd(const float fFogEnd) override;
	virtual Legacy::Vec3 GetFogColor() override;
	virtual float		 GetFogStart() override;
	virtual float		 GetFogEnd() override;
	virtual float		 GetTerrainElevation(float x, float y) override;
	virtual float		 GetTerrainZ(int x, int y) override;
	virtual int			 GetHeightMapUnitSize() override;
	virtual int			 GetTerrainSize() override;
	virtual int			 GetTerrainSectorSize() override;
	virtual float		 GetMaxViewDist() override;
	virtual bool		 AddStaticObject(int nObjectID, const Legacy::Vec3& vPos, const float fScale, unsigned char ucBright = 255) override;
	virtual bool		 RemoveStaticObject(int nObjectID, const Legacy::Vec3& vPos) override;
	virtual bool		 PhysicalizeStaticObject(void* pForeignData, int iForeignData, int iForeignFlags) override;
	virtual void		 RemoveAllStaticObjects() override;
	virtual void		 SetTerrainSurfaceType(int x, int y, int nType) override;
	virtual int			 GetTerrainSurfaceType(int x, int y) override;
	virtual void		 SetTerainHightMapBlock(int x1, int y1, int nSizeX, int nSizeY, unsigned short* TerrainBlock, unsigned short nUpdateMask = (((unsigned short)-1) & (~31))) override;
	virtual bool		 IsTerainHightMapModifiedByGame() override;
	virtual int			 LockTerrainSectorTexture(int nSectorOriginX, int nSectorOriginY, int& nTexDim) override;
	virtual bool		 SetStatInstGroup(int nGroupId, const IStatInstGroup& siGroup) override;
	virtual bool		 GetStatInstGroup(int nGroupId, IStatInstGroup& siGroup) override;
	virtual int			 GetTerrainTextureDim() override;
	virtual void		 SetTerrainBurnedOut(int x, int y, bool bBurnedOut) override;
	virtual bool		 IsTerrainBurnedOut(int x, int y) override;
	virtual void		 SetScreenShader(const char* szShaderName) override;
#if 0
	virtual void SetPhysMaterialEnumerator(IPhysMaterialEnumerator* pPhysMaterialEnumerator) override;
	virtual IPhysMaterialEnumerator* GetPhysMaterialEnumerator() override;
#endif
	virtual void		 SetupDistanceFog() override;
	virtual void		 LoadEnvironmentSettingsFromXML(const char* szMissionName, bool bEditorMode, const char* szMissionXMLString = 0, bool bUpdateLightingOnVegetations = true) override;
	virtual void		 LoadTerrainSurfacesFromXML(void* pDoc = NULL) override;
	virtual void		 UpdateBeaches() override;
	virtual bool		 IsPointInWater(Legacy::Vec3 vPos) override;
	virtual void		 AddDynamicLightSource(const CDLight& LSource, IEntityRender* pEnt, int nEntityLightId = -1, const Legacy::Matrix44* pMatrix = NULL) override;
	virtual void		 ApplyForceToEnvironment(Legacy::Vec3 vPos, float fRadius, float fAmountOfForce) override;
	virtual Legacy::Vec3 GetSunPosition(bool bMoveUp = true) override;
	virtual unsigned int GetLightMaskFromPosition(const Legacy::Vec3& vPos, float fRadius = 1.f) override;
	virtual Legacy::Vec3 GetAmbientColorFromPosition(const Legacy::Vec3& vPos, float fRadius = 1.f) override;
	virtual int			 GetFogVolumeIdFromBBox(const Legacy::Vec3& vBoxMin, const Legacy::Vec3& vBoxMax) override;
	virtual bool		 MakeSectorLightMap(int nSectorOriginX, int nSectorOriginY, unsigned char* pImage, int nImageSize) override;
	virtual float		 GetDistanceToSectorWithWater() override;
	virtual void		 SetSkyBoxAlpha(float fAlpha) override;
	virtual const char*	 GetLevelFilePath(const char* szFileName) override;
	virtual void		 DisplayInfo(float& fTextPosX, float& fTextPosY, float& fTextStepY) override;
	virtual IVisArea*	 CreateVisArea() override;
	virtual void		 DeleteVisArea(IVisArea* pVisArea) override;
	virtual void		 SetScreenMask(ITexPic* pMask) override;
	virtual void		 SetScreenFx(const char* pEffectName, int iActive) override;
	virtual void		 SetScreenFxParam(const char* pEffectName, const char* pEffectParam, void* pValue) override;
	virtual int			 GetScreenFx(const char* pEffectName) override;
	virtual int			 GetScreenFxParam(const char* pEffectName, const char* pEffectParam, void*& pValue) override;
	virtual void		 ResetScreenFx(void) override;
	virtual void		 SetFlags(int flags) override;
	virtual int			 GetFlags() override;

	void ClearRenderResources(bool bEditor = false);

  private:
	ISystem*					m_pSystem;
	World*						m_pWorld;
	SceneManager				m_SceneManager;
	CCamera						m_Camera;
	bool						m_Enabled;
	string						m_LevelPath;
	CVertexBuffer*				m_TestScene;
	CVertexBuffer*				m_Quad;
	std::vector<IEntityRender*> m_EntityRenders;
	RenderCallback*				m_pRenderCallback;
	void*						m_RenderCallbackParams;
	int							m_iFlags;

	struct AABB
	{
		Legacy::Vec3 min;
		Legacy::Vec3 max;
	} m_Qubes;

	// I3DEngine interface
  public:
	virtual void						DrawLowDetail(const int& DrawFlags) override;
	virtual int							GetLoadedObjectCount() override;
	virtual float						GetWaterLevel(const Legacy::Vec3* pvPos, Legacy::Vec3* pvFlowDir) override;
	virtual float						GetWaterLevel(IEntityRender* pEntityRender, Legacy::Vec3* pvFlowDir) override;
	virtual void						SpawnParticles(const ParticleParams& SpawnParticleParams) override;
	virtual void						ResetParticlesAndDecals() override;
	virtual IParticleEmitter*			CreateParticleEmitter() override;
	virtual void						DeleteParticleEmitter(IParticleEmitter* pPartEmitter) override;
	virtual IParticleEffect*			CreateParticleEffect() override;
	virtual void						DeleteParticleEffect(IParticleEffect* pEffect) override;
	virtual IParticleEffect*			FindParticleEffect(const char* sEffectName) override;
	virtual void						CreateDecal(const CryEngineDecalInfo& Decal) override;
	virtual void						DeleteDecalsInRange(Legacy::Vec3 vBoxMin, Legacy::Vec3 vBoxMax, bool bDeleteBigTerrainDecals) override;
	virtual const void*					GetShoreGeometry(int& nPosStride, int& nVertCount, int nSectorX, int nSectorY) override;
	virtual void						DrawTerrainDetailTextureLayers() override;
	virtual void						DrawFarTrees() override;
	virtual void						DrawTerrainParticles(IShader* pShader) override;
	virtual ICryCharInstance*			MakeCharacter(const char* cid_file_name, unsigned int dwFlags) override;
	virtual bool						IsCharacterFile(const char* szFileName) override;
	virtual void						RemoveCharacter(ICryCharInstance* pCryCharInstance) override;
	virtual Legacy::Vec3				GetWorldColor(bool bScaled) override;
	virtual void						SetWorldColor(Legacy::Vec3 vColor) override;
	virtual void						SetWorldColorRatio(float fWorldColorRatio) override;
	virtual float						GetWorldColorRatio() override;
	virtual void						RecompileBeaches() override;
	virtual void						OnExplosion(Legacy::Vec3 vPos, Legacy::Vec3 vHitDir, float fRadius, int nTexID, bool bDeformTerrain) override;
	virtual void						AddWaterSplash(Legacy::Vec3 vPos, eSplashType eST, float fForce, int Id) override;
	virtual void						SetPhysMaterialEnumerator(IPhysMaterialEnumerator* pPhysMaterialEnumerator) override;
	virtual IPhysMaterialEnumerator*	GetPhysMaterialEnumerator() override;
	virtual void						SetBFCount(int nCount) override;
	virtual int							GetBFCount() override;
	virtual void						SetGrasshopperCount(int nCount) override;
	virtual int							GetGrasshopperCount() override;
	virtual void						SetGrasshopperCGF(int nSlot, IStatObj* pStatObj) override;
	virtual Legacy::Vec3				GetOutdoorAmbientColor() override;
	virtual Legacy::Vec3				GetSunColor() override;
	virtual IEntityRenderState*			MakeEntityRenderState() override;
	virtual void						FreeEntityRenderState(IEntityRender* pEntity) override;
	virtual void						MakeUnderWaterSmoothHMap(int nWaterUnitSize) override;
	virtual unsigned short*				GetUnderWaterSmoothHMap(int& nDimensions) override;
	virtual void						UpdateDetailObjects() override;
	virtual IEdgeConnectivityBuilder*	GetNewConnectivityBuilder() override;
	virtual IStencilShadowConnectivity* NewConnectivity() override;
	virtual IEdgeConnectivityBuilder*	GetNewStaticConnectivityBuilder() override;
	virtual IEdgeDetector*				GetEdgeDetector() override;
	virtual void						EnableHeatVision(bool bEnable) override;
	virtual void						ActivatePortal(const Legacy::Vec3& vPos, bool bActivate, IEntityRender* pEntity) override;
	virtual IWaterVolume*				CreateWaterVolume() override;
	virtual void						DeleteWaterVolume(IWaterVolume* pWaterVolume) override;
	virtual IWaterVolume*				FindWaterVolumeByName(const char* szName) override;
	virtual void						UpdateVisArea(IVisArea* pArea, const Legacy::Vec3* pPoints, int nCount, const char* szName, float fHeight, const Legacy::Vec3& vAmbientColor, bool bAfectedByOutLights, bool bSkyOnly, const Legacy::Vec3& vDynAmbientColor, float fViewDistRatio, bool bDoubleSide, bool bUseDeepness, bool bUseInIndoors) override;
	virtual bool						IsVisAreasConnected(IVisArea* pArea1, IVisArea* pArea2, int nMaxRecursion, bool bSkipDisabledPortals) override;
	virtual IEntityRender*				CreateEntityRender() override;
	virtual void						DeleteEntityRender(IEntityRender* pEntityRender) override;
	virtual void						DrawRain() override;
	virtual void						SetRainAmount(float fAmount) override;
	virtual void						SetWindForce(const Legacy::Vec3& vWindForce) override;
	virtual float						GetLightAmountForEntity(IEntityRender* pEntity, bool bOnlyVisibleLights) override;
	virtual float						GetAmbientLightAmountForEntity(IEntityRender* pEntity) override;
	virtual IVisArea*					GetVisAreaFromPos(const Legacy::Vec3& vPos) override;
	virtual void						EnableOceanRendering(bool bOcean, bool bShore) override;
	virtual IMatInfo*					CreateMatInfo() override;
	virtual void						DeleteMatInfo(IMatInfo* pMatInfo) override;
	virtual void						RenameMatInfo(IMatInfo* pMtl, const char* sNewName) override;
	virtual IMatInfo*					FindMaterial(const char* sMaterialName) override;
	virtual ILMSerializationManager*	CreateLMSerializationManager() override;
	virtual bool						IsPotentiallyVisible(IEntityRender* pEntityRender, float fAdditionRadius) override;
	virtual INT_PTR						AddStaticLightSource(const CDLight& LSource, IEntityRender* pCreator, ICryCharInstance* pCryCharInstance, const char* szBoneName) override;
	virtual bool						DeleteStaticLightSource(INT_PTR nLightId) override;
	virtual const list2<CDLight*>*		GetStaticLightSources() override;
	virtual void						RestoreTerrainFromDisk() override;
	virtual const char*					GetFilePath(const char* szFileName) override;
	virtual void						SetBlurMask(ITexPic* pMask) override;
	virtual void						CheckPhysicalized(const Legacy::Vec3& vBoxMin, const Legacy::Vec3& vBoxMax) override;
	virtual void						CheckMemoryHeap() override;
	virtual float						GetObjectsLODRatio() override;
	virtual float						GetObjectsViewDistRatio() override;
	virtual float						GetObjectsMinViewDist() override;
	virtual bool						SetMaterialFloat(char* szMatName, int nSubMatId, int nTexSlot, char* szParamName, float fValue) override;
	virtual void						CloseTerrainTextureFile() override;
	virtual void						DeleteEntityDecals(IEntityRender* pEntity) override;
	virtual void						OnLevelLoaded() override;
	virtual void						LockCGFResources() override;
	virtual void						UnlockCGFResources() override;
	virtual CMatMan*					GetMatMan() override;
};
