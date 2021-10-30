#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/Camera.hpp>
#include <BlackBox/3DEngine/3DEngine.hpp>
#include <BlackBox/3DEngine/ObjLoader.hpp>
#include <BlackBox/Renderer/IRenderAuxGeom.hpp>
#include <BlackBox/Renderer/ITechnique.hpp>
#include <BlackBox/Scene/IScene.hpp>
#include <BlackBox/EntitySystem/IEntitySystem.hpp>

#include <BlackBox/EntitySystem/EntityDesc.hpp>
#include <BlackBox\Renderer\IFont.hpp>
#include <BlackBox\System\ITimer.hpp>
#include <BlackBox\Utils\Text.hpp>

void loadModel(string path);
C3DEngine::C3DEngine(ISystem *pSystem, const char *szInterfaceVersion)
    :
      m_pSystem(pSystem)
{

}

C3DEngine::~C3DEngine()
{

}

void C3DEngine::Enable(bool bEnable)
{
	m_Enabled = bEnable;
}

bool C3DEngine::Init()
{
    //MakeObject("res/geom/cnek.obj", nullptr);
    //MakeObject("res/geom/doggy.fbx", nullptr);
	return true;
}

void C3DEngine::SetLevelPath(const char* szFolderName)
{
	m_LevelPath = szFolderName;
}

void loadModel(string path)
{

} 

bool C3DEngine::LoadLevel(const char* szFolderName, const char* szMissionName, bool bEditorMode)
{
	if (gEnv->pRenderer)
	{
		//gEnv->pRenderer->GetIRenderAuxGeom()->DrawAABB(m_Qubes.min, m_Qubes.max);
	}

	return true;
}

void C3DEngine::Update()
{
}

void C3DEngine::SetCamera(const CCamera& cam, bool bToTheScreen)
{
	m_Camera = cam;
}
static float s_time = 0;
void C3DEngine::Draw()
{
	if (!m_Enabled)
		return;
	gEnv->pRenderer->SetCamera(m_Camera);

	static bool costil = true;

	if (costil)
	{
		auto obj_3d = MakeObject("Data/minecraft/Grass_Block.obj", nullptr);
		//auto obj_3d = MakeObject("Data/geom/pengium2.obj", nullptr);

		#if 0
		for (int z = 0; z < 10; z++)
		{
			for (int x = 0; x < 10; x++)
			{
				auto		  entity = gEnv->p3DEngine->MakeEntity(1, 0);
				CEntityObject obj;
				obj.scale  = glm::vec3(1);
				obj.object = obj_3d;
				entity->SetEntityObject(0, obj);
				entity->SetPos({x, 0, z});
				entity->SetScale(1);
			}
		
		}
		#endif

		costil = false;
	
	}

	for (IEntity* obj : m_Entities)
	{
		if (obj->IsGarbage() || obj->GetIStatObj() == nullptr)
			continue;
		SRendParams rp;
		glm::vec3 pos = obj->GetPos();
		glm::vec3 rotation = obj->GetAngles();
		float scale = obj->GetScale();
		auto		transform = glm::mat4(1);
		//glm::mat4 transform = glm::rotate(glm::mat4(1), );
		transform = glm::translate(transform, pos);
		transform = glm::scale(transform, glm::vec3(scale));
		IStatObj* stat = obj->GetIStatObj(0);
		rp.pMatrix = &transform;
		rp.texture = stat->GetTexture();
		stat->Render(rp, {});
	}

	#if 0
	if (m_pRenderCallback)
		m_pRenderCallback(m_RenderCallbackParams);
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
	CStatObj* obj{};
	if (obj = CStatObj::Load(szFileName, szGeomName); obj)
	{
		obj->SetBBoxMin(obj->m_IndexedMesh.m_vBoxMin);
		obj->SetBBoxMax(obj->m_IndexedMesh.m_vBoxMax);
		//m_Objects.push_back(obj);
	}
	return static_cast<IStatObj*>(obj);
}

IEntity* C3DEngine::MakeEntity()
{
	CEntityDesc desc(0, 0);
	auto entity = gEnv->pEntitySystem->SpawnEntity(desc);
	m_Entities.push_back(entity);
	return entity;
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

ICryCharInstance *C3DEngine::MakeCharacter(const char *cid_file_name, unsigned int dwFlags)
{

	return {};
}

bool C3DEngine::IsCharacterFile(const char *szFileName)
{

	return {};
}

void C3DEngine::RemoveCharacter(ICryCharInstance *pCryCharInstance)
{

}

Legacy::Vec3 C3DEngine::GetWorldColor(bool bScaled)
{

	return {};
}

void C3DEngine::SetWorldColor(Legacy::Vec3 vColor)
{

}

void C3DEngine::SetWorldColorRatio(float fWorldColorRatio)
{

}

float C3DEngine::GetWorldColorRatio()
{

	return {};
}

void C3DEngine::RecompileBeaches()
{

}

void C3DEngine::OnExplosion(Legacy::Vec3 vPos, Legacy::Vec3 vHitDir, float fRadius, int nTexID, bool bDeformTerrain)
{

}

void C3DEngine::AddWaterSplash(Legacy::Vec3 vPos, eSplashType eST, float fForce, int Id)
{

}

void C3DEngine::SetPhysMaterialEnumerator(IPhysMaterialEnumerator *pPhysMaterialEnumerator)
{

}

IPhysMaterialEnumerator *C3DEngine::GetPhysMaterialEnumerator()
{

	return {};
}

void C3DEngine::SetBFCount(int nCount)
{

}

int C3DEngine::GetBFCount()
{
	return {};

}

void C3DEngine::SetGrasshopperCount(int nCount)
{

}

int C3DEngine::GetGrasshopperCount()
{

	return {};
}

void C3DEngine::SetGrasshopperCGF(int nSlot, IStatObj *pStatObj)
{

}

Legacy::Vec3 C3DEngine::GetOutdoorAmbientColor()
{
	return {};
}

Legacy::Vec3 C3DEngine::GetSunColor()
{
	return {};

}

IEntityRenderState *C3DEngine::MakeEntityRenderState()
{
	return {};

}

void C3DEngine::FreeEntityRenderState(IEntityRender *pEntity)
{

}

void C3DEngine::MakeUnderWaterSmoothHMap(int nWaterUnitSize)
{

}

unsigned short *C3DEngine::GetUnderWaterSmoothHMap(int &nDimensions)
{

	return {};
}

void C3DEngine::UpdateDetailObjects()
{

}

IEdgeConnectivityBuilder *C3DEngine::GetNewConnectivityBuilder()
{

	return {};
}

IStencilShadowConnectivity *C3DEngine::NewConnectivity()
{

	return {};
}

IEdgeConnectivityBuilder *C3DEngine::GetNewStaticConnectivityBuilder()
{

	return {};
}

IEdgeDetector *C3DEngine::GetEdgeDetector()
{
	return {};

}

void C3DEngine::EnableHeatVision(bool bEnable)
{

}

void C3DEngine::ActivatePortal(const Legacy::Vec3 &vPos, bool bActivate, IEntityRender *pEntity)
{

}

IWaterVolume *C3DEngine::CreateWaterVolume()
{

	return {};
}

void C3DEngine::DeleteWaterVolume(IWaterVolume *pWaterVolume)
{

}

IWaterVolume *C3DEngine::FindWaterVolumeByName(const char *szName)
{
	return {};
}

void C3DEngine::UpdateVisArea(IVisArea *pArea, const Legacy::Vec3 *pPoints, int nCount, const char *szName, float fHeight, const Legacy::Vec3 &vAmbientColor, bool bAfectedByOutLights, bool bSkyOnly, const Legacy::Vec3 &vDynAmbientColor, float fViewDistRatio, bool bDoubleSide, bool bUseDeepness, bool bUseInIndoors)
{

}

bool C3DEngine::IsVisAreasConnected(IVisArea *pArea1, IVisArea *pArea2, int nMaxRecursion, bool bSkipDisabledPortals)
{
	return {};

}

IEntityRender *C3DEngine::CreateEntityRender()
{
	return {};
	return {};

}

void C3DEngine::DeleteEntityRender(IEntityRender *pEntityRender)
{

}

void C3DEngine::DrawRain()
{

}

void C3DEngine::SetRainAmount(float fAmount)
{

}

void C3DEngine::SetWindForce(const Legacy::Vec3 &vWindForce)
{

}

float C3DEngine::GetLightAmountForEntity(IEntityRender *pEntity, bool bOnlyVisibleLights)
{
	return {};

}

float C3DEngine::GetAmbientLightAmountForEntity(IEntityRender *pEntity)
{
	return {};

}

IVisArea *C3DEngine::GetVisAreaFromPos(const Legacy::Vec3 &vPos)
{
	return {};

}

void C3DEngine::EnableOceanRendering(bool bOcean, bool bShore)
{

}

IMatInfo *C3DEngine::CreateMatInfo()
{

	return {};
}

void C3DEngine::DeleteMatInfo(IMatInfo *pMatInfo)
{

}

void C3DEngine::RenameMatInfo(IMatInfo *pMtl, const char *sNewName)
{

}

IMatInfo *C3DEngine::FindMaterial(const char *sMaterialName)
{
	return {};

}

ILMSerializationManager *C3DEngine::CreateLMSerializationManager()
{

	return {};
}

bool C3DEngine::IsPotentiallyVisible(IEntityRender *pEntityRender, float fAdditionRadius)
{
	return {};

}

INT_PTR C3DEngine::AddStaticLightSource(const CDLight &LSource, IEntityRender *pCreator, ICryCharInstance *pCryCharInstance, const char *szBoneName)
{
	return {};

}

bool C3DEngine::DeleteStaticLightSource(INT_PTR nLightId)
{

	return {};
}

const list2<CDLight *> *C3DEngine::GetStaticLightSources()
{
	return {};

}

void C3DEngine::RestoreTerrainFromDisk()
{

}

const char *C3DEngine::GetFilePath(const char *szFileName)
{
	return {};

}

void C3DEngine::SetBlurMask(ITexPic *pMask)
{

}

void C3DEngine::CheckPhysicalized(const Legacy::Vec3 &vBoxMin, const Legacy::Vec3 &vBoxMax)
{

}

void C3DEngine::CheckMemoryHeap()
{

}

float C3DEngine::GetObjectsLODRatio()
{

	return {};
}

float C3DEngine::GetObjectsViewDistRatio()
{

	return {};
}

float C3DEngine::GetObjectsMinViewDist()
{

	return {};
}

bool C3DEngine::SetMaterialFloat(char *szMatName, int nSubMatId, int nTexSlot, char *szParamName, float fValue)
{
	return {};

}

void C3DEngine::CloseTerrainTextureFile()
{

}

void C3DEngine::DeleteEntityDecals(IEntityRender *pEntity)
{

}

void C3DEngine::OnLevelLoaded()
{

}

void C3DEngine::LockCGFResources()
{

}

void C3DEngine::UnlockCGFResources()
{

}

CMatMan *C3DEngine::GetMatMan()
{
	return {};

}

void C3DEngine::SetOutdoorAmbientColor(Legacy::Vec3 vColor)
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

void C3DEngine::SetFogColor(const Legacy::Vec3& vFogColor)
{
}

void C3DEngine::SetFogStart(const float fFogStart)
{
}

void C3DEngine::SetFogEnd(const float fFogEnd)
{
}

Legacy::Vec3 C3DEngine::GetFogColor()
{
  return Legacy::Vec3();
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

bool C3DEngine::AddStaticObject(int nObjectID, const Legacy::Vec3& vPos, const float fScale, unsigned char ucBright)
{
  return false;
}

bool C3DEngine::RemoveStaticObject(int nObjectID, const Legacy::Vec3& vPos)
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

bool C3DEngine::IsPointInWater(Legacy::Vec3 vPos)
{
  return false;
}

void C3DEngine::AddDynamicLightSource(const CDLight& LSource, IEntityRender* pEnt, int nEntityLightId, const Legacy::Matrix44* pMatrix)
{
}

void C3DEngine::ApplyForceToEnvironment(Legacy::Vec3 vPos, float fRadius, float fAmountOfForce)
{
}

Legacy::Vec3 C3DEngine::GetSunPosition(bool bMoveUp)
{
  return Legacy::Vec3();
}

unsigned int C3DEngine::GetLightMaskFromPosition(const Legacy::Vec3& vPos, float fRadius)
{
  return 0;
}

Legacy::Vec3 C3DEngine::GetAmbientColorFromPosition(const Legacy::Vec3& vPos, float fRadius)
{
  return Legacy::Vec3();
}

int C3DEngine::GetFogVolumeIdFromBBox(const Legacy::Vec3& vBoxMin, const Legacy::Vec3& vBoxMax)
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
	float px = 20;
	float py = fTextPosY;
	auto  dy = fTextStepY;
	dy = 15;

	static IFont* pFont{};
	if (!pFont)
	{
		pFont = gEnv->pRenderer->GetIFont();
		pFont->Init("VeraMono.ttf", 16, 16);
	}
	auto PRINT = [=,&py](float y, char* szFormat, ...)
	{
		char temp[256];
		va_list arglist;
		va_start(arglist, szFormat);
		vsprintf(temp, szFormat, arglist);
		va_end(arglist);

		PrintRightAlignedText(y, temp, pFont);
		py += dy;
	};

	auto p = m_Camera.transform.position;
	auto r = m_Camera.transform.rotation;
	PRINT(py, "CamPos = %.2f %.2f %.2f Angl = %3.2f %3.2f", p.x, p.y, p.z, r.x, r.y);
	PRINT(py, "Ver = 1.0.0.0");
	PRINT(py, "Polygons 0,000");
	PRINT(py, "...");
	PRINT(py, "FPS %.2f ( 60.. 50) / 60", 1.f / gEnv->pTimer->GetRealFrameTime());
	PRINT(py, "ViewDist = 1024/0.0");
	PRINT(py, "Render path = ...");
	if (gEnv->pSystem->IsDevMode())
		PRINT(py, gEnv->IsEditor() ? "DevMode (Editor)" : "DevMode");

	#undef PRINT
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
	m_iFlags = flags;
}

int C3DEngine::GetFlags()
{
  return m_iFlags;
}

void C3DEngine::ClearRenderResources(bool bEditor)
{
    //NOT_IMPLEMENTED;
    CryError("Ffunction not implemented: %s", __FUNCTION__);
}

void C3DEngine::DrawLowDetail(const int &DrawFlags)
{

	return;
}

int C3DEngine::GetLoadedObjectCount()
{

	return {};
}

float C3DEngine::GetWaterLevel(const Legacy::Vec3 *pvPos, Legacy::Vec3 *pvFlowDir)
{

	return {};
}

float C3DEngine::GetWaterLevel(IEntityRender *pEntityRender, Legacy::Vec3 *pvFlowDir)
{

	return {};
}

void C3DEngine::SpawnParticles(const ParticleParams &SpawnParticleParams)
{

}

void C3DEngine::ResetParticlesAndDecals()
{

}

IParticleEmitter *C3DEngine::CreateParticleEmitter()
{

	return {};
}

void C3DEngine::DeleteParticleEmitter(IParticleEmitter *pPartEmitter)
{

}

IParticleEffect *C3DEngine::CreateParticleEffect()
{

	return {};
}

void C3DEngine::DeleteParticleEffect(IParticleEffect *pEffect)
{

}

IParticleEffect *C3DEngine::FindParticleEffect(const char *sEffectName)
{

	return {};
}

void C3DEngine::CreateDecal(const CryEngineDecalInfo &Decal)
{

}

void C3DEngine::DeleteDecalsInRange(Legacy::Vec3 vBoxMin, Legacy::Vec3 vBoxMax, bool bDeleteBigTerrainDecals)
{

}

const void *C3DEngine::GetShoreGeometry(int &nPosStride, int &nVertCount, int nSectorX, int nSectorY)
{

	return {};
}

void C3DEngine::DrawTerrainDetailTextureLayers()
{

}

void C3DEngine::DrawFarTrees()
{

}

void C3DEngine::DrawTerrainParticles(IShader *pShader)
{

}

void C3DEngine::GetMemoryUsage(class ICrySizer* pSizer) const
{
    pSizer->AddObject(this, sizeof(*this));
	#if 0
    pSizer->AddObject(m_Objects);
	#endif
}


BB_3DENGINEENGINE_API I3DEngine* Create3DEngine(ISystem* pSystem, const char* szInterfaceVersion)
{
  return new C3DEngine(pSystem, szInterfaceVersion);
}
