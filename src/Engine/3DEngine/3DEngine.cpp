#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/Camera.hpp>
#include <BlackBox/3DEngine/3DEngine.hpp>
#include <BlackBox/Renderer/ITechniqueManager.hpp>
#include <BlackBox/Renderer/ITechnique.hpp>
#include <BlackBox/Scene/IScene.hpp>

void C3DEngine::Enable(bool bEnable)
{
}

bool C3DEngine::Init()
{
	//m_pWorld = new World();
  return true;
}

void C3DEngine::SetLevelPath(const char* szFolderName)
{
}

bool C3DEngine::LoadLevel(const char* szFolderName, const char* szMissionName, bool bEditorMode)
{
  //IScene* scene;
	#if 0
  auto tm = gEnv->pRenderer->GetITechniqueManager();
  auto tech = tm->get("hdr");
  if (tech != nullptr)
  {
    tech->Init(m_pWorld->GetActiveScene(), nullptr);
    m_pWorld->GetActiveScene()->setTechnique(tech);
  }
	#endif
  return true;
}

void C3DEngine::Update()
{
}

void C3DEngine::SetCamera(const CCamera& cam, bool bToTheScreen)
{
}

void C3DEngine::Draw()
{
  int w;
  int h;
  if (GET_CVAR("r_aspect")->GetIVal())
  {
    w = gEnv->pRenderer->GetHeight();
    h = gEnv->pRenderer->GetWidth();
  }
  else
  {
    w = GET_CVAR("r_cam_w")->GetIVal();
    h = GET_CVAR("r_cam_h")->GetIVal();
  }

	#if 0
  auto r = ((float)w) / h;
	gEnv->pRenderer->SetCamera(*m_pWorld->GetActiveScene()->getCurrentCamera());
  m_pWorld->GetActiveScene()->getCurrentCamera()->Ratio = r > 1 ? r : (float)h / w;
  if (m_pWorld->GetActiveScene())
  {
    m_pWorld->GetActiveScene()->present(gEnv->pRenderer->GetWidth(), gEnv->pRenderer->GetHeight());
  }
	#endif

}

void C3DEngine::ShutDown(bool bEditorMode)
{
}

void C3DEngine::Release()
{
  delete this;
}

void C3DEngine::ActivateLight(const char* szName, bool bActivate)
{
}

IStatObj* C3DEngine::MakeObject(const char* szFileName, const char* szGeomName, EVertsSharing eVertsSharing, bool bLoadAdditinalInfo, bool bKeepInLocalSpace)
{
  return nullptr;
}

bool C3DEngine::ReleaseObject(IStatObj* pObject)
{
  return false;
}

IStatObj* C3DEngine::MakeObject()
{
  return nullptr;
}

void C3DEngine::RegisterEntity(IEntityRender* pEntity)
{
}

bool C3DEngine::UnRegisterEntity(IEntityRender* pEntity)
{
  return false;
}

bool C3DEngine::UnRegisterInAllSectors(IEntityRender* pEntity)
{
  return false;
}

void C3DEngine::SetRenderCallback(RenderCallback pFunc, void* pParams)
{
}

void C3DEngine::SetOutdoorAmbientColor(Vec3d vColor)
{
}

void C3DEngine::SetSkyBox(const char* szShaderName)
{
}

void C3DEngine::SetMaxViewDistance(float fMaxViewDistance)
{
}

float C3DEngine::GetMaxViewDistance()
{
  return 0.0f;
}

void C3DEngine::SetFogColor(const Vec3& vFogColor)
{
}

void C3DEngine::SetFogStart(const float fFogStart)
{
}

void C3DEngine::SetFogEnd(const float fFogEnd)
{
}

Vec3 C3DEngine::GetFogColor()
{
  return Vec3();
}

float C3DEngine::GetFogStart()
{
  return 0.0f;
}

float C3DEngine::GetFogEnd()
{
  return 0.0f;
}

float C3DEngine::GetTerrainElevation(float x, float y)
{
  return 0.0f;
}

float C3DEngine::GetTerrainZ(int x, int y)
{
  return 0.0f;
}

int C3DEngine::GetHeightMapUnitSize()
{
  return 0;
}

int C3DEngine::GetTerrainSize()
{
  return 0;
}

int C3DEngine::GetTerrainSectorSize()
{
  return 0;
}

float C3DEngine::GetMaxViewDist()
{
  return 0.0f;
}

bool C3DEngine::AddStaticObject(int nObjectID, const Vec3& vPos, const float fScale, unsigned char ucBright)
{
  return false;
}

bool C3DEngine::RemoveStaticObject(int nObjectID, const Vec3& vPos)
{
  return false;
}

bool C3DEngine::PhysicalizeStaticObject(void* pForeignData, int iForeignData, int iForeignFlags)
{
  return false;
}

void C3DEngine::RemoveAllStaticObjects()
{
}

void C3DEngine::SetTerrainSurfaceType(int x, int y, int nType)
{
}

int C3DEngine::GetTerrainSurfaceType(int x, int y)
{
  return 0;
}

void C3DEngine::SetTerainHightMapBlock(int x1, int y1, int nSizeX, int nSizeY, unsigned short* TerrainBlock, unsigned short nUpdateMask)
{
}

bool C3DEngine::IsTerainHightMapModifiedByGame()
{
  return false;
}

int C3DEngine::LockTerrainSectorTexture(int nSectorOriginX, int nSectorOriginY, int& nTexDim)
{
  return 0;
}

bool C3DEngine::SetStatInstGroup(int nGroupId, const IStatInstGroup& siGroup)
{
  return false;
}

bool C3DEngine::GetStatInstGroup(int nGroupId, IStatInstGroup& siGroup)
{
  return false;
}

int C3DEngine::GetTerrainTextureDim()
{
  return 0;
}

void C3DEngine::SetTerrainBurnedOut(int x, int y, bool bBurnedOut)
{
}

bool C3DEngine::IsTerrainBurnedOut(int x, int y)
{
  return false;
}

void C3DEngine::SetScreenShader(const char* szShaderName)
{
}

#if 0
void C3DEngine::SetPhysMaterialEnumerator(IPhysMaterialEnumerator* pPhysMaterialEnumerator)
{
}

IPhysMaterialEnumerator* C3DEngine::GetPhysMaterialEnumerator()
{
  return nullptr;
}
#endif

void C3DEngine::SetupDistanceFog()
{
}

void C3DEngine::LoadEnvironmentSettingsFromXML(const char* szMissionName, bool bEditorMode, const char* szMissionXMLString, bool bUpdateLightingOnVegetations)
{
}

void C3DEngine::LoadTerrainSurfacesFromXML(void* pDoc)
{
}

void C3DEngine::UpdateBeaches()
{
}

bool C3DEngine::IsPointInWater(Vec3 vPos)
{
  return false;
}

void C3DEngine::AddDynamicLightSource(const CDLight& LSource, IEntityRender* pEnt, int nEntityLightId, const Matrix44* pMatrix)
{
}

void C3DEngine::ApplyForceToEnvironment(Vec3 vPos, float fRadius, float fAmountOfForce)
{
}

Vec3 C3DEngine::GetSunPosition(bool bMoveUp)
{
  return Vec3();
}

unsigned int C3DEngine::GetLightMaskFromPosition(const Vec3& vPos, float fRadius)
{
  return 0;
}

Vec3 C3DEngine::GetAmbientColorFromPosition(const Vec3& vPos, float fRadius)
{
  return Vec3();
}

int C3DEngine::GetFogVolumeIdFromBBox(const Vec3& vBoxMin, const Vec3& vBoxMax)
{
  return 0;
}

bool C3DEngine::MakeSectorLightMap(int nSectorOriginX, int nSectorOriginY, unsigned char* pImage, int nImageSize)
{
  return false;
}

float C3DEngine::GetDistanceToSectorWithWater()
{
  return 0.0f;
}

void C3DEngine::SetSkyBoxAlpha(float fAlpha)
{
}

const char* C3DEngine::GetLevelFilePath(const char* szFileName)
{
  return nullptr;
}

void C3DEngine::DisplayInfo(float& fTextPosX, float& fTextPosY, float& fTextStepY)
{
}

IVisArea* C3DEngine::CreateVisArea()
{
  return nullptr;
}

void C3DEngine::DeleteVisArea(IVisArea* pVisArea)
{
}

void C3DEngine::SetScreenMask(ITexPic* pMask)
{
}

void C3DEngine::SetScreenFx(const char* pEffectName, int iActive)
{
}

void C3DEngine::SetScreenFxParam(const char* pEffectName, const char* pEffectParam, void* pValue)
{
}

int C3DEngine::GetScreenFx(const char* pEffectName)
{
  return 0;
}

int C3DEngine::GetScreenFxParam(const char* pEffectName, const char* pEffectParam, void*& pValue)
{
  return 0;
}

void C3DEngine::ResetScreenFx(void)
{
}

void C3DEngine::SetFlags(int flags)
{
}

int C3DEngine::GetFlags()
{
  return 0;
}


BB_3DENGINEENGINE_API I3DEngine* Create3DEngine(ISystem* pSystem, const char* szInterfaceVersion)
{
  return new C3DEngine(pSystem, szInterfaceVersion);
}
