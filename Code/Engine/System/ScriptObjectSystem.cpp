#include "ScriptObjectSystem.hpp"
#include <BlackBox\3DEngine\I3DEngine.hpp>
#include <BlackBox\System\ITimer.hpp>
#include <BlackBox\System\File\ICryPak.hpp>

#include "System.hpp"
#include "HTTPDownloader.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_DECLARE_SCRIPTABLEEX(CScriptObjectSystem)

// modes of ScanDirectory function
#define SCANDIR_ALL 0
#define SCANDIR_FILES 1
#define SCANDIR_SUBDIRS 2

CScriptObjectSystem::CScriptObjectSystem(ISystem* pSystem, IScriptSystem* pSS)
  :
  m_pSystem(pSystem)
{
  m_pLog = m_pSystem->GetILog();
  m_pRenderer = m_pSystem->GetIRenderer();
  m_pConsole = m_pSystem->GetIConsole();
  InitializeTemplate(pSS);
#undef SCRIPT_REG_CLASSNAME
#define SCRIPT_REG_CLASSNAME CScriptObjectSystem
  SCRIPT_REG_TEMPLFUNC(Print, "string, float, float, float, float, float g, float b, float a");
  Init(pSS, pSystem);
}

CScriptObjectSystem::~CScriptObjectSystem()
{
}

void CScriptObjectSystem::InitializeTemplate(IScriptSystem* pSS)
{
  _ScriptableEx<CScriptObjectSystem>::InitializeTemplate(pSS);

#undef SCRIPT_REG_CLASSNAME
#define SCRIPT_REG_CLASSNAME CScriptObjectSystem

  // NOTE:
  // regex for change function signature to script registration macro
  // s/int \(\w\+\).*/SCRIPT_REG_FUNC(\1);/g
  SCRIPT_REG_FUNC(CreateDownload);
  SCRIPT_REG_FUNC(EnumDisplayFormats);
  SCRIPT_REG_FUNC(ScreenShot);
  SCRIPT_REG_FUNC(ClearConsole);
  SCRIPT_REG_FUNC(ShowConsole);
  SCRIPT_REG_FUNC(Log);

  SCRIPT_REG_FUNC(DrawImage);
  SCRIPT_REG_FUNC(LoadTexture);
  SCRIPT_REG_FUNC(LoadImage);
  SCRIPT_REG_FUNC(ScanDirectory);

  SCRIPT_REG_FUNC(LogToConsole);
  SCRIPT_REG_FUNC(ExecuteCommand);
  SCRIPT_REG_FUNC(Warning);
  SCRIPT_REG_FUNC(Error);
  SCRIPT_REG_FUNC(IsEditor);
  SCRIPT_REG_FUNC(LogAlways);

  
  SCRIPT_REG_FUNC(GetCurrAsyncTime);
  SCRIPT_REG_FUNC(GetFrameTime);

  SCRIPT_REG_FUNC(IsDevModeEnable);

  	gEnv->pScriptSystem->SetGlobalValue("SCANDIR_ALL", SCANDIR_ALL);
	gEnv->pScriptSystem->SetGlobalValue("SCANDIR_FILES", SCANDIR_FILES);
	gEnv->pScriptSystem->SetGlobalValue("SCANDIR_SUBDIRS", SCANDIR_SUBDIRS);
}

void CScriptObjectSystem::Init(IScriptSystem* pScriptSystem, ISystem* pSystem)
{
  m_pSystem = pSystem;
  InitGlobal(pScriptSystem, "System", this);
  if (gEnv->pRenderer)
  {
	  m_pFont = gEnv->pRenderer->GetIFont();
	  m_pFont->Init("VeraMono.ttf", 14,14);
  }

}

#include <lua.hpp>

/////////////////////////////////////////////////////////////////////////////////
/*! Creates a download object
    @return download object just created
 */
int CScriptObjectSystem::CreateDownload(IFunctionHandler* pH)
{
	auto download = ((CSystem*)(gEnv->pSystem))->m_pDownloadManager->CreateDownload();
	
	return pH->EndFunction(download->GetScriptObject());
}

int CScriptObjectSystem::EnumDisplayFormats(IFunctionHandler* pH)
{
  SCRIPT_CHECK_PARAMETERS(0);
	if (gEnv->IsDedicated())
  {
    return pH->EndFunction();
  }
  CryLog("Enumerating display settings...");
  SmartScriptObject pDispArray(m_pSS);
  SDispFormat* Formats = NULL;
  unsigned int i;
  unsigned int numFormats = m_pRenderer->EnumDisplayFormats(NULL);
  if (numFormats)
  {
    Formats = new SDispFormat[numFormats];
    m_pRenderer->EnumDisplayFormats(Formats);
  }

  for (i = 0; i < numFormats; i++)
  {
    SDispFormat* pForm = &Formats[i];
    SmartScriptObject pDisp(m_pSS);
    pDisp->SetValue("width", pForm->m_Width);
    pDisp->SetValue("height", pForm->m_Height);
    pDisp->SetValue("bpp", pForm->m_BPP);
    pDispArray->SetAt(i + 1, pDisp);
  }

  if (numFormats == 0)        // renderer is not doing his job
  {
    {
      SmartScriptObject pDisp(m_pSS);
      pDisp->SetValue("width", 640);
      pDisp->SetValue("height", 480);
      pDisp->SetValue("bpp", 32);
      pDispArray->SetAt(1, pDisp);
    }
    {
      SmartScriptObject pDisp(m_pSS);
      pDisp->SetValue("width", 800);
      pDisp->SetValue("height", 600);
      pDisp->SetValue("bpp", 32);
      pDispArray->SetAt(2, pDisp);
    }
    {
      SmartScriptObject pDisp(m_pSS);
      pDisp->SetValue("width", 1024);
      pDisp->SetValue("height", 768);
      pDisp->SetValue("bpp", 32);
      pDispArray->SetAt(3, pDisp);
    }
  }

  if (Formats)
    delete[] Formats;
  return pH->EndFunction(pDispArray);
}

int CScriptObjectSystem::ScreenShot(IFunctionHandler* pH)
{
  SCRIPT_CHECK_PARAMETERS(1);
  const char* filename;

  pH->GetParam(1, filename);
  m_pRenderer->ScreenShot(filename);
  return pH->EndFunction();
}

int CScriptObjectSystem::ClearConsole(IFunctionHandler* pH)
{
  m_pConsole->Clear();
  return pH->EndFunction();
}

int CScriptObjectSystem::ShowConsole(IFunctionHandler* pH)
{
  SCRIPT_CHECK_PARAMETERS(1);
  int show;

  pH->GetParam(1, show);
  m_pConsole->ShowConsole(show);
  return pH->EndFunction();
}

int CScriptObjectSystem::DrawImage(IFunctionHandler* pH)
{
  SCRIPT_CHECK_PARAMETERS(6);
	int id;
	float xpos;
	float ypos;
	float w;
	float h;
	int blending_mode;
	pH->GetParams(id, xpos, ypos, w, h, blending_mode);
	gEnv->pRenderer->DrawImage(xpos, ypos, w, h, id, 0, 0, 1, 1, 0, 1, 0, 1);
	return pH->EndFunction();
}

int CScriptObjectSystem::LoadTexture(IFunctionHandler* pH)
{
	const char* name;
	if (gEnv->pRenderer)
	{
		if (pH->GetParams(name))
		{
#if 0
            CryLog("User ask load %s texture", name);
#endif
			int		  t			= gEnv->pRenderer->LoadTexture(name, 0, 0);
			USER_DATA pUserData = m_pScriptSystem->CreateUserData((int)t, USER_DATA_TEXTURE);
			return pH->EndFunction(pUserData);
		}
	}

	return pH->EndFunctionNull();
}

int CScriptObjectSystem::Log(IFunctionHandler* pH)
{
	SCRIPT_CHECK_PARAMETERS(1);
	const char* string;

	pH->GetParam(1, string);
	m_pSystem->Log(string);
	//m_pConsole->PrintLine(string);
	return pH->EndFunction();
}

int CScriptObjectSystem::Print(const char* text, float x, float y, float scale, float r, float g, float b, float a)
{
	float color[] = {r, g, b, a};
	m_pFont->RenderText(text, x, y, scale, color);
	m_pFont->Submit();
	return 0;
}
int CScriptObjectSystem::LoadImage(IFunctionHandler* pH)
{
	return LoadTexture(pH);
}

/////////////////////////////////////////////////////////////////////////////////
inline bool Filter(struct __finddata64_t& fd, int nScanMode)
{
	if (!strcmp(fd.name, ".") || !strcmp(fd.name, ".."))
		return false;

	switch (nScanMode)
	{
	case SCANDIR_ALL:
		return true;
	case SCANDIR_SUBDIRS:
		return 0 != (fd.attrib & _A_SUBDIR);
	case SCANDIR_FILES:
		return 0 == (fd.attrib & _A_SUBDIR);
	default:
		return false;
	}
}

/////////////////////////////////////////////////////////////////////////////////
inline bool Filter(struct _finddata_t& fd, int nScanMode)
{
	if (!strcmp(fd.name, ".") || !strcmp(fd.name, ".."))
		return false;

	switch (nScanMode)
	{
	case SCANDIR_ALL:
		return true;
	case SCANDIR_SUBDIRS:
		return 0 != (fd.attrib & _A_SUBDIR);
	case SCANDIR_FILES:
		return 0 == (fd.attrib & _A_SUBDIR);
	default:
		return false;
	}
}

int CScriptObjectSystem::ScanDirectory(IFunctionHandler* pH)
{
	if (pH->GetParamCount() < 1)
		return pH->EndFunction();

	SmartScriptObject pObj(m_pSS);
	int				  k = 1;

	const char* pszFolderName;
	if (!pH->GetParam(1, pszFolderName))
		return pH->EndFunction(*pObj);

	int nScanMode = SCANDIR_SUBDIRS;

	if (pH->GetParamCount() > 1)
		pH->GetParam(2, nScanMode);

	{
		_finddata_t c_file;
		intptr_t	hFile;

		if ((hFile = gEnv->pCryPak->FindFirst((string(pszFolderName) + "\\*.*").c_str(), &c_file)) == -1L)
		{
			return pH->EndFunction(*pObj);
		}
		else
		{
			do
			{
				if (Filter(c_file, nScanMode))
				{
					pObj->SetAt(k, c_file.name);
					k++;
				}
			} while (gEnv->pCryPak->FindNext(hFile, &c_file) == 0);

			gEnv->pCryPak->FindClose(hFile);
		}
	}

	return pH->EndFunction(*pObj);
}

/////////////////////////////////////////////////////////////////////////////////
int CScriptObjectSystem::ExecuteCommand(IFunctionHandler* pH)
{
	SCRIPT_CHECK_PARAMETERS(1);
	const char* szCmd;

	if (pH->GetParam(1, szCmd))
	{
		gEnv->pConsole->ExecuteString(szCmd);
	}

	return pH->EndFunction();
}

/////////////////////////////////////////////////////////////////////////////////
/*! Write a string to the console
    @param String to write
    @see CScriptObjectSystem::Log
 */
int CScriptObjectSystem::LogToConsole(IFunctionHandler* pH)
{
	SCRIPT_CHECK_PARAMETERS(1);
	#if 0
	LogString(pH, true);
	#endif

	return pH->EndFunction();
}

/////////////////////////////////////////////////////////////////////////////////
/*! log even with log verbosity 0 - without <LUA>
   @param String to write
 */
int CScriptObjectSystem::LogAlways(IFunctionHandler* pH)
{
	SCRIPT_CHECK_PARAMETERS(1);
	const char* sParam = NULL;

	pH->GetParam(1, sParam);

	if (sParam)
		CryLogAlways("%s", sParam);

	return pH->EndFunction();
}

/////////////////////////////////////////////////////////////////////////////////
int CScriptObjectSystem::Warning(IFunctionHandler* pH)
{
	SCRIPT_CHECK_PARAMETERS(1);
	const char* sParam = "";
	if (pH->GetParam(1, sParam))
	{
		m_pSystem->Warning(VALIDATOR_MODULE_SCRIPTSYSTEM, VALIDATOR_WARNING, 0, NULL, "%s", sParam);
	}

	return pH->EndFunction();
}

/////////////////////////////////////////////////////////////////////////////////
int CScriptObjectSystem::Error(IFunctionHandler* pH)
{
	SCRIPT_CHECK_PARAMETERS(1);
	const char* sParam = "";
	if (pH->GetParam(1, sParam))
	{
		gEnv->pSystem->Warning(VALIDATOR_MODULE_SCRIPTSYSTEM, VALIDATOR_ERROR, 0, NULL, "%s", sParam);
	}

	return pH->EndFunction();
}

/////////////////////////////////////////////////////////////////////////////////
int CScriptObjectSystem::IsEditor(IFunctionHandler* pH)
{
	return pH->EndFunction(gEnv->IsEditor());
}

/////////////////////////////////////////////////////////////////////////////////
int CScriptObjectSystem::GetCurrAsyncTime(IFunctionHandler* pH)
{
	SCRIPT_CHECK_PARAMETERS(0);
	float fTime = gEnv->pTimer->GetAsyncCurTime();
	return pH->EndFunction(fTime);
}

/////////////////////////////////////////////////////////////////////////////////
int CScriptObjectSystem::GetFrameTime(IFunctionHandler* pH)
{
	SCRIPT_CHECK_PARAMETERS(0);
	float fTime = gEnv->pTimer->GetFrameTime();
	return pH->EndFunction(fTime);
}
#if 0
/////////////////////////////////////////////////////////////////////////////////
int CScriptObjectSystem::GetLocalOSTime(IFunctionHandler* pH)
{
	SCRIPT_CHECK_PARAMETERS(0);
	//! Get time.
#if CRY_PLATFORM_LINUX || CRY_PLATFORM_ANDROID || CRY_PLATFORM_APPLE || CRY_PLATFORM_ORBIS
	time_t	   long_time = time(NULL);
	struct tm* newtime	 = localtime(&long_time); /* Convert to local time. */
#else
	__time64_t long_time;
	_time64(&long_time);						   /* Get time as long integer. */
	struct tm* newtime = _localtime64(&long_time); /* Convert to local time. */
#endif

	if (newtime)
	{
		m_pScriptTimeTable->BeginSetGetChain();
		m_pScriptTimeTable->SetValueChain("sec", newtime->tm_sec);
		m_pScriptTimeTable->SetValueChain("min", newtime->tm_min);
		m_pScriptTimeTable->SetValueChain("hour", newtime->tm_hour);
		m_pScriptTimeTable->SetValueChain("isdst", newtime->tm_isdst);
		m_pScriptTimeTable->SetValueChain("mday", newtime->tm_mday);
		m_pScriptTimeTable->SetValueChain("wday", newtime->tm_wday);
		m_pScriptTimeTable->SetValueChain("mon", newtime->tm_mon);
		m_pScriptTimeTable->SetValueChain("yday", newtime->tm_yday);
		m_pScriptTimeTable->SetValueChain("year", newtime->tm_year);
		m_pScriptTimeTable->EndSetGetChain();
	}
	return pH->EndFunction(m_pScriptTimeTable);
}

/////////////////////////////////////////////////////////////////////////////////
int CScriptObjectSystem::GetUserName(IFunctionHandler* pH)
{
	SCRIPT_CHECK_PARAMETERS(0);
	//! Get user name on this machine.
	return pH->EndFunction(GetISystem()->GetUserName());
}

/////////////////////////////////////////////////////////////////////////////////
int CScriptObjectSystem::ShowConsole(IFunctionHandler* pH)
{
	SCRIPT_CHECK_PARAMETERS(1);
	int nParam = 0;
	pH->GetParam(1, nParam);
	m_pConsole->ShowConsole(nParam != 0);
	return pH->EndFunction();
}

/////////////////////////////////////////////////////////////////////////////////
int CScriptObjectSystem::CheckHeapValid(IFunctionHandler* pH)
{
	const char* name = "<noname>";
	if (pH->GetParamCount() > 0 && pH->GetParamType(1) == svtString)
		pH->GetParam(1, name);

	if (!IsHeapValid())
	{
		CryLogAlways("IsHeapValid failed: %s", name);
		assert(IsHeapValid());
	}

	return pH->EndFunction();
}

/////////////////////////////////////////////////////////////////////////////////
int CScriptObjectSystem::GetConfigSpec(IFunctionHandler* pH)
{
	static ICVar* e_obj_quality(gEnv->pConsole->GetCVar("e_ObjQuality"));
	int			  obj_quality = CONFIG_VERYHIGH_SPEC;
	if (e_obj_quality)
		obj_quality = e_obj_quality->GetIVal();

	return pH->EndFunction(obj_quality);
}

/////////////////////////////////////////////////////////////////////////////////
int CScriptObjectSystem::IsMultiplayer(IFunctionHandler* pH)
{
	return pH->EndFunction(gEnv->bMultiplayer);
}

/////////////////////////////////////////////////////////////////////////////////
/*!Get an entity by id
   @param nID the entity id
 */
int CScriptObjectSystem::GetEntity(IFunctionHandler* pH)
{
	//support also number type
	EntityId eID(0);
	if (pH->GetParamType(1) == svtNumber)
	{
		pH->GetParam(1, eID);
	}
	else
	{
		ScriptHandle sh;
		pH->GetParam(1, sh);
		eID = (EntityId)sh.n;
	}

	IEntity* pEntity = gEnv->pEntitySystem->GetEntity(eID);

	if (pEntity)
	{
		IScriptTable* pObject = pEntity->GetScriptTable();

		if (pObject)
		{
			return pH->EndFunction(pObject);
		}
	}

	return pH->EndFunction();
}

/////////////////////////////////////////////////////////////////////////////////
int CScriptObjectSystem::GetEntityClass(IFunctionHandler* pH)
{
	EntityId eID(0);
	if (pH->GetParamType(1) == svtNumber)
	{
		pH->GetParam(1, eID);
	}
	else
	{
		ScriptHandle sh;
		pH->GetParam(1, sh);
		eID = (EntityId)sh.n;
	}

	IEntity* pEntity = gEnv->pEntitySystem->GetEntity(eID);

	if (pEntity)
		return pH->EndFunction(pEntity->GetClass()->GetName());

	return pH->EndFunction();
}

/////////////////////////////////////////////////////////////////////////////////
/*!return a all entities currently present in the level
   @return a table filled with all entities currently present in the level
 */
int CScriptObjectSystem::GetEntities(IFunctionHandler* pH)
{
	Vec3  center(0, 0, 0);
	float radius(0);

	if (pH->GetParamCount() > 1)
	{
		pH->GetParam(1, center);
		pH->GetParam(2, radius);
	}

	SmartScriptTable pObj(m_pSS);
	int				 k = 0;

	IEntityItPtr pIIt	 = gEnv->pEntitySystem->GetEntityIterator();
	IEntity*	 pEntity = NULL;

	while (pEntity = pIIt->Next())
	{
		if (radius)
			if ((pEntity->GetWorldPos() - center).len2() > radius * radius)
				continue;

		if (pEntity->GetScriptTable())
		{
			/*ScriptHandle handle;
			   handle.n = pEntity->GetId();*/

			pObj->SetAt(k, pEntity->GetScriptTable());
			k++;
		}
	}
	return pH->EndFunction(*pObj);
}

/////////////////////////////////////////////////////////////////////////////////
/*!return a all entities for a specified entity class
   @return a table filled with all entities of a specified entity class
 */
int CScriptObjectSystem::GetEntitiesByClass(IFunctionHandler* pH, const char* EntityClass)
{
	if (EntityClass == NULL || EntityClass[0] == '\0')
		return pH->EndFunction();

	IEntityClass* pClass = gEnv->pEntitySystem->GetClassRegistry()->FindClass(EntityClass);

	if (!pClass)
	{
		return pH->EndFunction();
	}

	SmartScriptTable pObj(m_pSS);
	IEntityItPtr	 pIIt	 = gEnv->pEntitySystem->GetEntityIterator();
	IEntity*		 pEntity = 0;
	int				 k		 = 1;

	pIIt->MoveFirst();

	while (pEntity = pIIt->Next())
	{
		if (pEntity->GetClass() == pClass)
		{
			if (pEntity->GetScriptTable())
			{
				pObj->SetAt(k++, pEntity->GetScriptTable());
			}
		}
	}

	return pH->EndFunction(*pObj);
}

/////////////////////////////////////////////////////////////////////////////////
/*!return a all entities for a specified entity class
   @return a table filled with all entities of a specified entity class in a radius
 */
int CScriptObjectSystem::GetEntitiesInSphere(IFunctionHandler* pH, Vec3 center, float radius)
{
	SmartScriptTable pObj(m_pSS);
	IEntityItPtr	 pIIt	 = gEnv->pEntitySystem->GetEntityIterator();
	IEntity*		 pEntity = 0;
	int				 k		 = 1;

	pIIt->MoveFirst();

	while (pEntity = pIIt->Next())
	{
		if ((pEntity->GetWorldPos() - center).len2() <= radius * radius)
		{
			if (pEntity->GetScriptTable())
			{
				pObj->SetAt(k++, pEntity->GetScriptTable());
			}
		}
	}

	return pH->EndFunction(*pObj);
}

/////////////////////////////////////////////////////////////////////////////////
/*!return a all entities for a specified entity class
   @return a table filled with all entities of a specified entity class in a radius
 */
int CScriptObjectSystem::GetEntitiesInSphereByClass(IFunctionHandler* pH, Vec3 center, float radius, const char* EntityClass)
{
	if (EntityClass == NULL || EntityClass[0] == '\0')
		return pH->EndFunction();

	IEntityClass* pClass = gEnv->pEntitySystem->GetClassRegistry()->FindClass(EntityClass);

	if (!pClass)
	{
		return pH->EndFunction();
	}

	SmartScriptTable pObj(m_pSS);
	IEntityItPtr	 pIIt	 = gEnv->pEntitySystem->GetEntityIterator();
	IEntity*		 pEntity = 0;
	int				 k		 = 1;

	pIIt->MoveFirst();

	while (pEntity = pIIt->Next())
	{
		if ((pEntity->GetClass() == pClass) && ((pEntity->GetWorldPos() - center).len2() <= radius * radius))
		{
			if (pEntity->GetScriptTable())
			{
				pObj->SetAt(k++, pEntity->GetScriptTable());
			}
		}
	}

	return pH->EndFunction(*pObj);
}

/////////////////////////////////////////////////////////////////////////////////
inline bool Filter(struct __finddata64_t& fd, int nScanMode)
{
	if (!strcmp(fd.name, ".") || !strcmp(fd.name, ".."))
		return false;

	switch (nScanMode)
	{
	case SCANDIR_ALL:
		return true;
	case SCANDIR_SUBDIRS:
		return 0 != (fd.attrib & _A_SUBDIR);
	case SCANDIR_FILES:
		return 0 == (fd.attrib & _A_SUBDIR);
	default:
		return false;
	}
}

/////////////////////////////////////////////////////////////////////////////////
inline bool Filter(struct _finddata_t& fd, int nScanMode)
{
	if (!strcmp(fd.name, ".") || !strcmp(fd.name, ".."))
		return false;

	switch (nScanMode)
	{
	case SCANDIR_ALL:
		return true;
	case SCANDIR_SUBDIRS:
		return 0 != (fd.attrib & _A_SUBDIR);
	case SCANDIR_FILES:
		return 0 == (fd.attrib & _A_SUBDIR);
	default:
		return false;
	}
}

/////////////////////////////////////////////////////////////////////////////////
int CScriptObjectSystem::ScanDirectory(IFunctionHandler* pH)
{
	if (pH->GetParamCount() < 1)
		return pH->EndFunction();

	SmartScriptTable pObj(m_pSS);
	int				 k = 1;

	const char* pszFolderName;
	if (!pH->GetParam(1, pszFolderName))
		return pH->EndFunction(*pObj);

	int nScanMode = SCANDIR_SUBDIRS;

	if (pH->GetParamCount() > 1)
		pH->GetParam(2, nScanMode);

	{
		_finddata_t c_file;
		intptr_t	hFile;

		if ((hFile = gEnv->pCryPak->FindFirst((string(pszFolderName) + "\\*.*").c_str(), &c_file)) == -1L)
		{
			return pH->EndFunction(*pObj);
		}
		else
		{
			do
			{
				if (Filter(c_file, nScanMode))
				{
					pObj->SetAt(k, c_file.name);
					k++;
				}
			} while (gEnv->pCryPak->FindNext(hFile, &c_file) == 0);

			gEnv->pCryPak->FindClose(hFile);
		}
	}

	return pH->EndFunction(*pObj);
}

/////////////////////////////////////////////////////////////////////////////////
int CScriptObjectSystem::DrawLabel(IFunctionHandler* pH)
{
	SCRIPT_CHECK_PARAMETERS(7);
	Vec3		vPos(0, 0, 0);
	float		fSize;
	const char* text;
	float		r(1);
	float		g(1);
	float		b(1);
	float		alpha(1);

	if (!pH->GetParams(vPos, fSize, text, r, g, b, alpha))
		return pH->EndFunction();

	if (m_pRenderer)
	{
		float color[] = {r, g, b, alpha};

		IRenderAuxText::DrawLabelEx(vPos, fSize, color, true, true, text);
		//IRenderAuxText::DrawLabel(vPos, fSize, text);
	}

	return pH->EndFunction();
}

/////////////////////////////////////////////////////////////////////////////////
/*!return all entities contained in a certain radius
    @param oVec center of the sphere
    @param fRadius length of the radius
    @return a table filled with all entities contained in the specified radius
 */
int CScriptObjectSystem::GetPhysicalEntitiesInBox(IFunctionHandler* pH, Vec3 center, float radius)
{
	IEntitySystem* pEntitySystem = gEnv->pEntitySystem;

	SEntityProximityQuery query;
	query.box.min = center - Vec3(radius, radius, radius);
	query.box.max = center + Vec3(radius, radius, radius);
	pEntitySystem->QueryProximity(query);

	int n = query.nCount;
	if (n > 0)
	{
		SmartScriptTable tbl(m_pSS);

		int k = 0;
		for (int i = 0; i < n; i++)
		{
			IEntity* pEntity = query.pEntities[i];
			if (pEntity)
			{
				// The physics can return multiple records per one entity, filter out entities of same id.
				if (!pEntity->GetPhysics())
					continue;
				IScriptTable* pEntityTable = pEntity->GetScriptTable();
				if (pEntityTable)
				{
					tbl->SetAt(++k, pEntityTable);
				}
			}
		}
		if (k)
			return pH->EndFunction(tbl);
	}

	return pH->EndFunction();
}

/////////////////////////////////////////////////////////////////////////////////
int CScriptObjectSystem::GetPhysicalEntitiesInBoxByClass(IFunctionHandler* pH, Vec3 center, float radius, const char* className)
{
	IEntitySystem* pEntitySystem = gEnv->pEntitySystem;
	IEntityClass*  pClass		 = pEntitySystem->GetClassRegistry()->FindClass(className);
	if (!pClass)
	{
		return pH->EndFunction();
	}

	SEntityProximityQuery query;
	query.box.min	   = center - Vec3(radius, radius, radius);
	query.box.max	   = center + Vec3(radius, radius, radius);
	query.pEntityClass = pClass;
	pEntitySystem->QueryProximity(query);

	int n = query.nCount;
	if (n > 0)
	{
		SmartScriptTable tbl(m_pSS);

		int k = 0;
		for (int i = 0; i < n; i++)
		{
			IEntity* pEntity = query.pEntities[i];
			if (pEntity)
			{
				// The physics can return multiple records per one entity, filter out entities of same id.
				if (!pEntity->GetPhysics())
					continue;
				IScriptTable* pEntityTable = pEntity->GetScriptTable();
				if (pEntityTable)
				{
					tbl->SetAt(++k, pEntityTable);
				}
			}
		}
		if (k)
			return pH->EndFunction(tbl);
	}
	return pH->EndFunction();
}

/////////////////////////////////////////////////////////////////////////////////
int CScriptObjectSystem::GetNearestEntityByClass(IFunctionHandler* pH, Vec3 center, float radius, const char* className)
{
	IEntitySystem* pEntitySystem = gEnv->pEntitySystem;
	IEntityClass*  pClass		 = pEntitySystem->GetClassRegistry()->FindClass(className);
	if (!pClass)
		return pH->EndFunction();

	ScriptHandle ignore[2];
	ignore[0].n = ignore[1].n = 0;

	if (pH->GetParamCount() > 3)
		pH->GetParam(4, ignore[0]);
	if (pH->GetParamCount() > 4)
		pH->GetParam(5, ignore[1]);

	SEntityProximityQuery query;
	query.box.min	   = center - Vec3(radius, radius, radius);
	query.box.max	   = center + Vec3(radius, radius, radius);
	query.pEntityClass = pClass;
	pEntitySystem->QueryProximity(query);

	int	  closest	   = -1;
	float closestdist2 = 1e+8f;

	int n = query.nCount;
	if (n > 0)
	{
		for (int i = 0; i < n; i++)
		{
			IEntity* pEntity = query.pEntities[i];
			if (pEntity && pEntity->GetId() != ignore[0].n && pEntity->GetId() != ignore[1].n)
			{
				float dist2 = (pEntity->GetWorldPos() - center).len2();
				if (dist2 < closestdist2)
				{
					closest		 = i;
					closestdist2 = dist2;
				}
			}
		}

		if (closest > -1)
			return pH->EndFunction(query.pEntities[closest]->GetScriptTable());
	}

	return pH->EndFunction();
}

/////////////////////////////////////////////////////////////////////////////////
int CScriptObjectSystem::GetEntityByName(IFunctionHandler* pH, const char* sEntityName)
{
	IEntity* pEntity = gEnv->pEntitySystem->FindEntityByName(sEntityName);
	if (pEntity)
	{
		IScriptTable* pObject = pEntity->GetScriptTable();
		return pH->EndFunction(pObject);
	}
	return pH->EndFunction();
}

/////////////////////////////////////////////////////////////////////////////////
int CScriptObjectSystem::GetEntityIdByName(IFunctionHandler* pH, const char* sEntityName)
{
	IEntity* pEntity = gEnv->pEntitySystem->FindEntityByName(sEntityName);
	if (pEntity)
	{
		ScriptHandle handle;
		handle.n = pEntity->GetId();
		return pH->EndFunction(handle);
	}
	return pH->EndFunction();
}

/////////////////////////////////////////////////////////////////////////////////
/*!create a terrain deformation at the given point
   this function is called when a projectile or a grenade explode
   @param oVec explosion position
   @param fSize explosion radius
 */
int CScriptObjectSystem::DeformTerrainInternal(IFunctionHandler* pH, bool nameIsMaterial)
{
	if (pH->GetParamCount() < 3)
		return pH->EndFunction();

	Vec3		vPos;
	float		fSize;
	const char* name = 0;

	pH->GetParams(vPos, fSize, name);

	bool bDeform = true;

	if (pH->GetParamCount() > 3)
		pH->GetParam(4, bDeform);

	m_p3DEngine->OnExplosion(vPos, fSize, bDeform);

	return pH->EndFunction();
}

/////////////////////////////////////////////////////////////////////////////////
int CScriptObjectSystem::DeformTerrain(IFunctionHandler* pH)
{
	return DeformTerrainInternal(pH, false);
}

/////////////////////////////////////////////////////////////////////////////////
int CScriptObjectSystem::DeformTerrainUsingMat(IFunctionHandler* pH)
{
	return DeformTerrainInternal(pH, true);
}

/////////////////////////////////////////////////////////////////////////////////
int CScriptObjectSystem::ScreenToTexture(IFunctionHandler* pH)
{
	SCRIPT_CHECK_PARAMETERS(0);
	//m_pRenderer->ScreenToTexture(0);
	return pH->EndFunction();
}

/////////////////////////////////////////////////////////////////////////////////
int CScriptObjectSystem::DrawLine(IFunctionHandler* pH)
{
	SCRIPT_CHECK_PARAMETERS(6);
	IRenderAuxGeom* pRenderAuxGeom(gEnv->pRenderer->GetIRenderAuxGeom());

	Vec3 p1(0, 0, 0);
	Vec3 p2(0, 0, 0);
	pH->GetParam(1, p1);
	pH->GetParam(2, p2);

	float r, g, b, a;
	pH->GetParam(3, r);
	pH->GetParam(4, g);
	pH->GetParam(5, b);
	pH->GetParam(6, a);
	ColorB col((unsigned char)(r * 255.0f), (unsigned char)(g * 255.0f),
			   (unsigned char)(b * 255.0f), (unsigned char)(a * 255.0f));

	pRenderAuxGeom->SetRenderFlags(e_Def3DPublicRenderflags);
	pRenderAuxGeom->DrawLine(p1, col, p2, col);

	return pH->EndFunction();
}

/////////////////////////////////////////////////////////////////////////////////
int CScriptObjectSystem::Draw2DLine(IFunctionHandler* pH)
{
	SCRIPT_CHECK_PARAMETERS(8);
	IRenderAuxGeom* pRenderAuxGeom(gEnv->pRenderer->GetIRenderAuxGeom());

	Vec3 p1(0, 0, 0);
	Vec3 p2(0, 0, 0);
	pH->GetParam(1, p1.x);
	pH->GetParam(2, p1.y);
	pH->GetParam(3, p2.x);
	pH->GetParam(4, p2.y);

	// do normalization as exiting scripts assume a virtual window size of 800x600
	// however the auxiliary geometry rendering system uses normalized device coords for 2d primitive rendering
	const float c_Normalize2Dx(1.0f / 800.0f);
	const float c_Normalize2Dy(1.0f / 600.0f);
	p1.x *= c_Normalize2Dx;
	p1.y *= c_Normalize2Dy;
	p2.x *= c_Normalize2Dx;
	p2.y *= c_Normalize2Dy;

	float r, g, b, a;
	pH->GetParam(5, r);
	pH->GetParam(6, g);
	pH->GetParam(7, b);
	pH->GetParam(8, a);
	ColorB col((unsigned char)(r * 255.0f), (unsigned char)(g * 255.0f),
			   (unsigned char)(b * 255.0f), (unsigned char)(a * 255.0f));

	SAuxGeomRenderFlags renderFlags(e_Def2DPublicRenderflags);

	if (a < 1.0f)
	{
		renderFlags.SetAlphaBlendMode(e_AlphaBlended);
	}

	pRenderAuxGeom->SetRenderFlags(renderFlags);
	pRenderAuxGeom->DrawLine(p1, col, p2, col);

	return pH->EndFunction();
}

/////////////////////////////////////////////////////////////////////////////////
int CScriptObjectSystem::DrawText(IFunctionHandler* pH)
{
	SCRIPT_CHECK_PARAMETERS(9);
	ICryFont* pCryFont = gEnv->pCryFont;

	if (!pCryFont)
	{
		return pH->EndFunction();
	}

	float		x		 = 0;
	float		y		 = 0;
	const char* text	 = "";
	const char* fontName = "default";
	float		size	 = 16;
	float		r		 = 1;
	float		g		 = 1;
	float		b		 = 1;
	float		a		 = 1;

	pH->GetParam(1, x);
	pH->GetParam(2, y);
	pH->GetParam(3, text);
	pH->GetParam(4, fontName);
	pH->GetParam(5, size);
	pH->GetParam(6, r);
	pH->GetParam(7, g);
	pH->GetParam(8, b);
	pH->GetParam(9, a);

	IFFont* pFont = pCryFont->GetFont(fontName);

	if (!pFont)
	{
		return pH->EndFunction();
	}

	STextDrawContext ctx;
	ctx.SetColor(ColorF(r, g, b, a));
	ctx.SetSize(Vec2(size, size));
	ctx.SetProportional(true);
	ctx.SetSizeIn800x600(true);
	pFont->DrawString(x, y, text, true, ctx);

	return pH->EndFunction();
}

/////////////////////////////////////////////////////////////////////////////////
int CScriptObjectSystem::DrawSphere(IFunctionHandler* pH, float x, float y, float z, float radius, int r, int g, int b, int a)
{
	IRenderAuxGeom* pRenderAuxGeom = gEnv->pRenderer->GetIRenderAuxGeom();
	if (pRenderAuxGeom)
	{
		SAuxGeomRenderFlags oldFlags = pRenderAuxGeom->GetRenderFlags();
		SAuxGeomRenderFlags newFlags = oldFlags;

		newFlags.SetCullMode(e_CullModeNone);
		newFlags.SetFillMode(e_FillModeWireframe);
		newFlags.SetAlphaBlendMode(e_AlphaBlended);
		pRenderAuxGeom->SetRenderFlags(newFlags);

		pRenderAuxGeom->DrawSphere(Vec3(x, y, z), radius, ColorB(r, g, b, a), false);

		pRenderAuxGeom->SetRenderFlags(oldFlags);
	}
	return pH->EndFunction();
}

/////////////////////////////////////////////////////////////////////////////////
int CScriptObjectSystem::DrawAABB(IFunctionHandler* pH, float x, float y, float z, float x2, float y2, float z2, int r, int g, int b, int a)
{
	IRenderAuxGeom* pRenderAuxGeom = gEnv->pRenderer->GetIRenderAuxGeom();
	if (pRenderAuxGeom)
	{
		SAuxGeomRenderFlags oldFlags = pRenderAuxGeom->GetRenderFlags();
		SAuxGeomRenderFlags newFlags = oldFlags;

		newFlags.SetCullMode(e_CullModeNone);
		newFlags.SetAlphaBlendMode(e_AlphaBlended);
		pRenderAuxGeom->SetRenderFlags(newFlags);

		AABB bbox(Vec3(x, y, z), Vec3(x2, y2, z2));
		pRenderAuxGeom->DrawAABB(bbox, true, ColorB(r, g, b, a), eBBD_Faceted);

		pRenderAuxGeom->SetRenderFlags(oldFlags);
	}
	return pH->EndFunction();
}

/////////////////////////////////////////////////////////////////////////////////
int CScriptObjectSystem::DrawOBB(IFunctionHandler* pH, float x, float y, float z, float w, float h, float d, float rx, float ry, float rz)
{
	IRenderAuxGeom* pRenderAuxGeom = gEnv->pRenderer->GetIRenderAuxGeom();
	if (pRenderAuxGeom)
	{
		SAuxGeomRenderFlags oldFlags = pRenderAuxGeom->GetRenderFlags();
		SAuxGeomRenderFlags newFlags = oldFlags;

		newFlags.SetCullMode(e_CullModeNone);
		newFlags.SetAlphaBlendMode(e_AlphaBlended);
		pRenderAuxGeom->SetRenderFlags(newFlags);

		AABB bbox(Vec3(-w * 0.5f, -h * 0.5f, -d * 0.5f), Vec3(w * 0.5f, h * 0.5f, d * 0.5f));
		OBB	 obb = OBB::CreateOBBfromAABB(Matrix33::CreateRotationXYZ(Ang3(rx, ry, rz)), bbox);
		pRenderAuxGeom->DrawOBB(obb, Vec3(x, y, z), true, ColorB(255, 128, 128, 128), eBBD_Faceted);

		pRenderAuxGeom->SetRenderFlags(oldFlags);
	}
	return pH->EndFunction();
}

/////////////////////////////////////////////////////////////////////////////////
// <<NOTE>> check 3dScreenEffects for a list of effects names and respective parameters

/////////////////////////////////////////////////////////////////////////////////
int CScriptObjectSystem::SetPostProcessFxParam(IFunctionHandler* pH)
{
	SCRIPT_CHECK_PARAMETERS(2);
	const char* pszEffectParam = 0;
	pH->GetParam(1, pszEffectParam);

	ScriptVarType type = pH->GetParamType(2);
	switch (type)
	{
	case svtNumber:
	{
		float fValue = -1;
		pH->GetParam(2, fValue);
		gEnv->p3DEngine->SetPostEffectParam(pszEffectParam, fValue);

		break;
	}
	case svtObject:
	{
		Vec3 pValue = Vec3(0, 0, 0);
		pH->GetParam(2, pValue);
		gEnv->p3DEngine->SetPostEffectParamVec4(pszEffectParam, Vec4(pValue, 1));

		break;
	}
	case svtString:
	{
		const char* pszValue = 0;
		pH->GetParam(2, pszValue);
		gEnv->p3DEngine->SetPostEffectParamString(pszEffectParam, pszValue);

		break;
	}
	}

	return pH->EndFunction();
}
#endif
/////////////////////////////////////////////////////////////////////////////////
int CScriptObjectSystem::IsDevModeEnable(IFunctionHandler* pH)
{
	SCRIPT_CHECK_PARAMETERS(0);
	// check if we're running in devmode (cheat mode)
	// to check if we are allowed to enable certain scripts
	// function facilities (god mode, fly mode etc.)
	bool bDevMode = m_pSystem->IsDevMode();

	return pH->EndFunction(bDevMode);
}
