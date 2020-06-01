#pragma once
#include <BlackBox/EntitySystem/IEntityRenderState.hpp>
#include <BlackBox/3DEngine/I3DEngine.hpp>
#include <BlackBox/3DEngine/SceneManager.hpp>
#include <BlackBox/World/World.hpp>
class C3DEngine : public I3DEngine
{
	typedef void(*RenderCallback)(void* pParams);
public:
  C3DEngine(ISystem* pSystem, const char* szInterfaceVersion)
    :
    m_pSystem(pSystem)
  {

  }
  ~C3DEngine()
  {

  }
  virtual void Enable(bool bEnable) override;
  virtual bool Init() override;
  virtual void SetLevelPath(const char* szFolderName) override;
  virtual bool LoadLevel(const char* szFolderName, const char* szMissionName, bool bEditorMode = false) override;
  virtual void Update() override;
  virtual void SetCamera(const CCamera& cam, bool bToTheScreen = true) override;
  virtual void Draw() override;
  virtual void ShutDown(bool bEditorMode = false) override;
  virtual void Release() override;
  virtual void ActivateLight(const char* szName, bool bActivate) override;
  virtual IStatObj* MakeObject(const char* szFileName, const char* szGeomName = 0, EVertsSharing eVertsSharing = evs_ShareAndSortForCache, bool bLoadAdditinalInfo = true, bool bKeepInLocalSpace = false) override;
  virtual bool ReleaseObject(IStatObj* pObject) override;
  virtual IStatObj* MakeObject() override;
  virtual void RegisterEntity(IEntityRender* pEntity) override;
  virtual bool UnRegisterEntity(IEntityRender* pEntity) override;
  virtual bool UnRegisterInAllSectors(IEntityRender* pEntity = NULL) override;
  virtual void SetRenderCallback(RenderCallback pFunc, void* pParams) override;
  virtual void SetOutdoorAmbientColor(Vec3d vColor) override;
  virtual void SetSkyBox(const char* szShaderName) override;
  virtual void SetMaxViewDistance(float fMaxViewDistance) override;
  virtual float GetMaxViewDistance() override;
  virtual void SetFogColor(const Vec3& vFogColor) override;
  virtual void SetFogStart(const float fFogStart) override;
  virtual void SetFogEnd(const float fFogEnd) override;
  virtual Vec3 GetFogColor() override;
  virtual float GetFogStart() override;
  virtual float GetFogEnd() override;
  virtual float GetTerrainElevation(float x, float y) override;
  virtual float GetTerrainZ(int x, int y) override;
  virtual int GetHeightMapUnitSize() override;
  virtual int GetTerrainSize() override;
  virtual int GetTerrainSectorSize() override;
  virtual float GetMaxViewDist() override;
  virtual bool AddStaticObject(int nObjectID, const Vec3& vPos, const float fScale, unsigned char ucBright = 255) override;
  virtual bool RemoveStaticObject(int nObjectID, const Vec3& vPos) override;
  virtual bool PhysicalizeStaticObject(void* pForeignData, int iForeignData, int iForeignFlags) override;
  virtual void RemoveAllStaticObjects() override;
  virtual void SetTerrainSurfaceType(int x, int y, int nType) override;
  virtual int GetTerrainSurfaceType(int x, int y) override;
  virtual void SetTerainHightMapBlock(int x1, int y1, int nSizeX, int nSizeY, unsigned short* TerrainBlock, unsigned short nUpdateMask = (((unsigned short)-1) & (~31))) override;
  virtual bool IsTerainHightMapModifiedByGame() override;
  virtual int LockTerrainSectorTexture(int nSectorOriginX, int nSectorOriginY, int& nTexDim) override;
  virtual bool SetStatInstGroup(int nGroupId, const IStatInstGroup& siGroup) override;
  virtual bool GetStatInstGroup(int nGroupId, IStatInstGroup& siGroup) override;
  virtual int GetTerrainTextureDim() override;
  virtual void SetTerrainBurnedOut(int x, int y, bool bBurnedOut) override;
  virtual bool IsTerrainBurnedOut(int x, int y) override;
  virtual void SetScreenShader(const char* szShaderName) override;
#if 0
  virtual void SetPhysMaterialEnumerator(IPhysMaterialEnumerator* pPhysMaterialEnumerator) override;
  virtual IPhysMaterialEnumerator* GetPhysMaterialEnumerator() override;
#endif
  virtual void SetupDistanceFog() override;
  virtual void LoadEnvironmentSettingsFromXML(const char* szMissionName, bool bEditorMode, const char* szMissionXMLString = 0, bool bUpdateLightingOnVegetations = true) override;
  virtual void LoadTerrainSurfacesFromXML(void* pDoc = NULL) override;
  virtual void UpdateBeaches() override;
  virtual bool IsPointInWater(Vec3 vPos) override;
  virtual void AddDynamicLightSource(const CDLight& LSource, IEntityRender* pEnt, int nEntityLightId = -1, const Matrix44* pMatrix = NULL) override;
  virtual void ApplyForceToEnvironment(Vec3 vPos, float fRadius, float fAmountOfForce) override;
  virtual Vec3 GetSunPosition(bool bMoveUp = true) override;
  virtual unsigned int GetLightMaskFromPosition(const Vec3& vPos, float fRadius = 1.f) override;
  virtual Vec3 GetAmbientColorFromPosition(const Vec3& vPos, float fRadius = 1.f) override;
  virtual int GetFogVolumeIdFromBBox(const Vec3& vBoxMin, const Vec3& vBoxMax) override;
  virtual bool MakeSectorLightMap(int nSectorOriginX, int nSectorOriginY, unsigned char* pImage, int nImageSize) override;
  virtual float GetDistanceToSectorWithWater() override;
  virtual void SetSkyBoxAlpha(float fAlpha) override;
  virtual const char* GetLevelFilePath(const char* szFileName) override;
  virtual void DisplayInfo(float& fTextPosX, float& fTextPosY, float& fTextStepY) override;
  virtual IVisArea* CreateVisArea() override;
  virtual void DeleteVisArea(IVisArea* pVisArea) override;
  virtual void SetScreenMask(ITexPic* pMask) override;
  virtual void SetScreenFx(const char* pEffectName, int iActive) override;
  virtual void SetScreenFxParam(const char* pEffectName, const char* pEffectParam, void* pValue) override;
  virtual int GetScreenFx(const char* pEffectName) override;
  virtual int GetScreenFxParam(const char* pEffectName, const char* pEffectParam, void*& pValue) override;
  virtual void ResetScreenFx(void) override;

private:
	ISystem*				m_pSystem;
	World*					m_pWorld;
	SceneManager		m_SceneManager;

	CCamera					m_Camera;

	bool						m_Enabled;
	string					m_LevelPath;

	void*						m_RenderCallbacksParams;

	CVertexBuffer*	m_TestScene;
	CVertexBuffer*	m_Quad;

	struct AABB
	{
		Vec3 min;
		Vec3 max;
	}m_Qubes;

	std::vector<IShaderProgram*> m_Programs;

  // Inherited via I3DEngine
  virtual void SetFlags(int flags) override;
  virtual int GetFlags() override;
};
