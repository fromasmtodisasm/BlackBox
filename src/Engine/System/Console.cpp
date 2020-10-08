#include <BlackBox/System/System.hpp>
#include <BlackBox/Core/Utils.hpp>
#include <BlackBox/Input/IHardwareMouse.hpp>
#include <BlackBox/Input/IInput.hpp>
#include <BlackBox/Renderer/IFont.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/Texture.hpp>
#include <BlackBox/Renderer/TextureManager.hpp>
#include <BlackBox/ScriptSystem/IScriptSystem.hpp>
#include <BlackBox/System/Console.hpp>
#include <BlackBox/System/IClipBoard.hpp>
#include <BlackBox/System/ILog.hpp>

#include "NullImplementation/NullFont.hpp"
#define CRY_ASSERT ASSERT

#define BACKGROUND_SERVER_CHAR '/'
#define CONST_TEMP_STRING(s) s
//#define DEFENCE_CVAR_HASH_LOGGING

namespace ECVarTypeHelper
{
	template<typename T>
	constexpr const char* GetNameForT();
	template<>
	constexpr const char* GetNameForT<int>()
	{
		return "int";
	}
	template<>
	constexpr const char* GetNameForT<float>()
	{
		return "float";
	}
	template<>
	constexpr const char* GetNameForT<const char*>()
	{
		return "string";
	}

	inline const char* GetNameForECVar(ECVarType type)
	{
		switch (type)
		{
		case ECVarType::Invalid:
			return "?";
		case ECVarType::Int:
			return "int";
		case ECVarType::Float:
			return "float";
		case ECVarType::String:
			return "string";
		default:
			CRY_ASSERT(false);
			return "?";
		}
	}
} // namespace ECVarTypeHelper

static inline void AssertName(const char* szName)
{
#ifdef _DEBUG
	CRY_ASSERT(szName);

	// test for good console variable / command name
	const char* p	= szName;
	bool bFirstChar = true;

	while (*p)
	{
		CRY_ASSERT((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || (*p >= '0' && *p <= '9' && !bFirstChar) || *p == '_' || *p == '.');

		++p;
		bFirstChar = false;
	}
#endif
}

// user defined comparison - for nicer printout
inline int GetCharPrio(char x)
{
	if (x >= 'a' && x <= 'z')
		x += 'A' - 'a'; // make upper case

	if (x == '_')
		return 300;
	else
		return x;
}

// case sensitive
inline bool less_CVar(const char* left, const char* right)
{
	while (true)
	{
		const uint32 l = GetCharPrio(*left), r = GetCharPrio(*right);

		if (l < r)
			return true;
		if (l > r)
			return false;

		if (*left == 0 || *right == 0)
			break;

		++left;
		++right;
	}

	return false;
}

void Command_SetWaitSeconds(IConsoleCmdArgs* pCmd)
{
#if WAIT_SECONDS
	if (pCmd->GetArgCount() > 1)
	{
		CXConsole* pConsole = static_cast<CXConsole*>(gEnv->pConsole);
		if (pConsole->m_waitSeconds.GetValue() != 0)
			CryWarning(EValidatorModule::VALIDATOR_MODULE_SYSTEM, EValidatorSeverity::VALIDATOR_WARNING, "You are overwriting the current wait seconds!");
		pConsole->m_waitSeconds.SetSeconds(atof(pCmd->GetArg(1)));
		pConsole->m_waitSeconds += gEnv->pTimer->GetFrameStartTime();
	}
#endif
}

void Command_SetWaitFrames(IConsoleCmdArgs* pCmd)
{
	if (pCmd->GetArgCount() > 1)
	{
		CXConsole* pConsole = static_cast<CXConsole*>(gEnv->pConsole);
		if (pConsole->m_waitFrames != 0)
			CryWarning(EValidatorModule::VALIDATOR_MODULE_SYSTEM, EValidatorSeverity::VALIDATOR_WARNING, "You are overwriting the current wait frames!");
		pConsole->m_waitFrames = std::max(0, atoi(pCmd->GetArg(1)));
	}
}

#pragma region CVars
char* CCVar::GetString()
{
	static char sResult[256];
	char* result;
	if (type == CVAR_INT)
	{
		sprintf(sResult, "%d", value.i);
		result = sResult;
	}
	else if (type == CVAR_FLOAT)
	{
		sprintf(sResult, "%f", value.f);
		result = sResult;
	}
	else
	{
		result = value.s;
	}
	return result;
}

void CCVar::Set(const char* s)
{
	if (value.s != nullptr && type == CVAR_STRING)
		delete[] value.s;
	value.s = strdup(s);
	type	= CVAR_STRING;
}

void CCVar::ForceSet(const char* s)
{
	if (value.s != nullptr)
		delete[] value.s;
	type	= CVAR_STRING;
	value.s = const_cast<char*>(s);
}

void CCVar::Set(float f)
{
	if (type == CVAR_STRING)
	{
		delete[] value.s;
	}
	type	= CVAR_FLOAT;
	value.f = f;
}

void CCVar::Set(int i)
{
	if (type == CVAR_STRING)
	{
		delete[] value.s;
	}
	type	= CVAR_INT;
	value.i = i;
}

void CCVar::Refresh()
{
	value.i = 0;
}

int CCVar::GetType()
{
	return type;
}

const char* CCVar::GetHelp()
{
	return help;
}

int CCVar::GetIVal()
{
	if (type == CVAR_FLOAT)
		value.i = static_cast<int>(value.f);
	else if (type == CVAR_STRING)
		value.i = static_cast<int>(std::atoi(value.s));
	type = CVAR_INT;
	return value.i;
}

float CCVar::GetFVal()
{
	if (type == CVAR_INT)
		value.f = static_cast<float>(value.i);
	else if (type == CVAR_STRING)
		value.f = static_cast<float>(std::atof(value.s));
	type = CVAR_FLOAT;
	return value.f;
}

int CCVarRef::GetIVal()
{
	if (type == CVAR_FLOAT)
		*value.i = static_cast<int>(*value.f);
	else if (type == CVAR_STRING)
		*value.i = static_cast<int>(std::atoi(*value.s));
	type = CVAR_INT;
	return *value.i;
}

float CCVarRef::GetFVal()
{
	if (type == CVAR_INT)
		*value.f = static_cast<float>(*value.i);
	else if (type == CVAR_STRING)
		*value.f = static_cast<float>(std::atof(*value.s));
	type = CVAR_FLOAT;
	return *value.f;
}

char* CCVarRef::GetString()
{
	static char sResult[256];
	char* result;
	if (type == CVAR_INT)
	{
		sprintf(sResult, "%d", *value.i);
		result = sResult;
	}
	else if (type == CVAR_FLOAT)
	{
		sprintf(sResult, "%f", *value.f);
		result = sResult;
	}
	else
	{
		result = *value.s;
	}
	return result;
}

void CCVarRef::Set(const char* s)
{
#if 0
  if (*value.s != nullptr && type == CVAR_STRING)
    delete[] *value.s;
  *value.s = strdup(s);
  type = CVAR_STRING;
#endif
	if (type == CVAR_INT)
	{
		Set(atoi(s));
	}
	else if (type == CVAR_FLOAT)
	{
		Set((float)atof(s));
	}
	else
	{
		ForceSet(s);
	}
}

void CCVarRef::ForceSet(const char* s)
{
	if (value.s != nullptr)
		delete[] * value.s;
	type	 = CVAR_STRING;
	*value.s = const_cast<char*>(s);
}

void CCVarRef::Set(float f)
{
	if (type == CVAR_STRING)
	{
		delete[] * value.s;
	}
	type	 = CVAR_FLOAT;
	*value.f = f;
}

void CCVarRef::Set(int i)
{
	if (type == CVAR_STRING)
	{
		delete[] * value.s;
	}
	type	 = CVAR_INT;
	*value.i = i;
}

void CCVarRef::Refresh()
{
	*value.i = 0;
}

int CCVarRef::GetType()
{
	return type;
}

const char* CCVarRef::GetHelp()
{
	return help;
}

CommandLine ConsolePrompt::get()
{
	//auto time					  = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::string time_str		  = "0"; //std::ctime(&time);
	time_str[time_str.size() - 1] = 0;
	return {
		Text(user + "@" + pc, glm::vec3(0.0, 1.0, 0.0), 1.0),
		Text(" " + env, glm::vec3(1.0, 0.0, 1.0), 1.0),
		Text(" " + cd, glm::vec3(1.0, 1.0, 0.0), 1.0),
		Text(std::string(" " + time_str), color, 1.0) //,
													  //Text(" FPS: " + std::to_string(GetISystem()->GetIGame()->getFPS()) + "\n", glm::vec3(1.0, 0.3, 0.5), 1.0),
	};
}

void CNullCVar::Release()
{
}

int CNullCVar::GetIVal()
{
	return 0;
}

float CNullCVar::GetFVal()
{
	return 0.0f;
}

char* CNullCVar::GetString()
{
	return nullptr;
}

void CNullCVar::Set(const char* s)
{
}

void CNullCVar::ForceSet(const char* s)
{
}

void CNullCVar::Set(float f)
{
}

void CNullCVar::Set(int i)
{
}

void CNullCVar::Refresh()
{
}

void CNullCVar::ClearFlags(int flags)
{
}

int CNullCVar::GetFlags()
{
	return 0;
}

int CNullCVar::SetFlags(int flags)
{
	return 0;
}

int CNullCVar::GetType()
{
	return 0;
}

const char* CNullCVar::GetName()
{
	return nullptr;
}

const char* CNullCVar::GetHelp()
{
	return nullptr;
}
#pragma endregion

CXConsole::CXConsole(CSystem& system)
	: //m_managedConsoleCommandListeners(1)
	  /*,*/ m_system(system)
{
	m_fRepeatTimer			= 0;
	m_pSysDeactivateConsole = 0;
	m_pFont					= NULL;
	m_pRenderer				= NULL;
	m_pNetwork				= NULL; // EvenBalance - M. Quinn
	m_pInput				= NULL;
	m_pImage				= NULL;
	m_nCursorPos			= 0;
	m_nScrollPos			= 0;
	m_nScrollMax			= 300;
	m_nTempScrollMax		= m_nScrollMax;
	m_nScrollLine			= 0;
	m_nHistoryPos			= -1;
	m_nTabCount				= 0;
	m_bConsoleActive		= false;
	m_bActivationKeyEnable	= true;
	m_bIsProcessingGroup	= false;
	m_sdScrollDir			= sdNONE;
	m_bDrawCursor			= true;
	m_fCursorBlinkTimer		= 0;

	m_nCheatHashRangeFirst = 0;
	m_nCheatHashRangeLast  = 0;
	m_bCheatHashDirty	   = false;
	m_nCheatHash		   = 0;

	m_bStaticBackground = false;
	m_nProgress			= 0;
	m_nProgressRange	= 0;
	m_nLoadingBackTexID = 0;
	m_nWhiteTexID		= 0;

	m_deferredExecution = false;
	m_waitFrames		= 0;
#if WAIT_SECONDS
	m_waitSeconds = 0.0f;
#endif
	m_blockCounter = 0;

	m_currentLoadConfigType = eLoadConfigDefault;
	m_readOnly				= false;

#if 0
	CNotificationNetworkConsole::Initialize();
#endif
}

CXConsole::~CXConsole()
{
	if (gEnv->pSystem)
		gEnv->pSystem->GetIRemoteConsole()->UnregisterListener(this);

#if 0
	CNotificationNetworkConsole::Shutdown();
#endif

	for (auto& nameValuePair : m_configVars)
	{
		if (nameValuePair.first == "sys_cvar_overrides_path") // from system.cfg, but not handled as a CVar
			continue;
		CryWarning(EValidatorModule::VALIDATOR_MODULE_SYSTEM, EValidatorSeverity::VALIDATOR_WARNING,
				   "The CVar '%s' was found in a config file but was never registered.", nameValuePair.first.c_str());
	}

	for (auto& nameArgumentsPair : m_configCommands)
	{
		CryWarning(EValidatorModule::VALIDATOR_MODULE_SYSTEM, EValidatorSeverity::VALIDATOR_WARNING,
				   "The command '%s %s' was found in a config file but was never registered.", nameArgumentsPair.first.c_str(), nameArgumentsPair.second.c_str());
	}

	while (!m_mapVariables.empty())
		UnregisterVariableImpl(m_mapVariables.begin());
	while (!m_mapCommands.empty())
	{
		string command = m_mapCommands.begin()->first;
		RemoveCommand(command.c_str());
	}
}

bool CXConsole::Init(ISystem* pSystem)
{
	return false;
}

void CXConsole::Release()
{
	delete this;
}

ICVar* CXConsole::CreateVariable(const char* sName, const char* sValue, int nFlags, const char* help /* = ""*/)
{
	AssertName(sName);

	ICVar* pCVar = stl::find_in_map(m_mapVariables, sName, NULL);
	if (pCVar)
	{
		gEnv->pLog->LogError("[CVARS]: [DUPLICATE] CXConsole::RegisterString(const char*): variable [%s] is already registered", pCVar->GetName());
#if LOG_CVAR_INFRACTIONS_CALLSTACK
		gEnv->pSystem->debug_LogCallStack();
#endif // LOG_CVAR_INFRACTIONS_CALLSTACK
		return pCVar;
	}

	const string name(sName);
	pCVar = new CCVar(this, name.data(), sValue, nFlags, help /*, true*/);
	RegisterVar(name, pCVar /*, pChangeFunc*/);
	return pCVar;
}

ICVar* CXConsole::CreateVariable(const char* sName, int iValue, int nFlags, const char* help /* = ""*/)
{
	AssertName(sName);

	ICVar* pCVar = stl::find_in_map(m_mapVariables, sName, NULL);
	if (pCVar)
	{
		gEnv->pLog->LogError("[CVARS]: [DUPLICATE] CXConsole::RegisterInt(): variable [%s] is already registered", pCVar->GetName());
#if LOG_CVAR_INFRACTIONS_CALLSTACK
		gEnv->pSystem->debug_LogCallStack();
#endif // LOG_CVAR_INFRACTIONS_CALLSTACK
		return pCVar;
	}

	const string name(sName);
	pCVar = new CCVar(this, name.data(), iValue, nFlags, help /*, true*/);
	RegisterVar(name, pCVar /*, pChangeFunc*/);
	return pCVar;
}

ICVar* CXConsole::CreateVariable(const char* sName, float fValue, int nFlags, const char* help /* = ""*/)
{
	AssertName(sName);

	ICVar* pCVar = stl::find_in_map(m_mapVariables, sName, nullptr_t(NULL));
	if (pCVar)
	{
		gEnv->pLog->LogError("[CVARS]: [DUPLICATE] CXConsole::RegisterFloat(): variable [%s] is already registered", pCVar->GetName());
#if LOG_CVAR_INFRACTIONS_CALLSTACK
		gEnv->pSystem->debug_LogCallStack();
#endif // LOG_CVAR_INFRACTIONS_CALLSTACK
		return pCVar;
	}

	const string name(sName);
	pCVar = new CCVar(this, name.data(), fValue, nFlags, help /*, true*/);
	RegisterVar(name, pCVar /*, pChangeFunc*/);
	return pCVar;
}

void CXConsole::UnregisterVariable(const char* sVarName, bool bDelete /* = false*/)
{
}

void CXConsole::AddCheckedCVar(ConsoleVariablesVector& vector, const ConsoleVariablesVector::value_type& value)
{
	ConsoleVariablesVector::iterator it = std::lower_bound(vector.begin(), vector.end(), value, CVarNameLess);

	if ((it == vector.end()) || strcmp(it->first, value.first))
		vector.insert(it, value);
}

void CXConsole::RemoveCheckedCVar(ConsoleVariablesVector& vector, const ConsoleVariablesVector::value_type& value)
{
	auto it = std::lower_bound(vector.begin(), vector.end(), value, CVarNameLess);
	if ((it != vector.end()) && !strcmp(it->first, value.first))
		vector.erase(it);
}

bool CXConsole::CVarNameLess(const std::pair<const char*, ICVar*>& lhs, const std::pair<const char*, ICVar*>& rhs)
{
	return strcmp(lhs.first, rhs.first) < 0;
}

void CXConsole::UnregisterVariableImpl(const ConsoleVariablesMap::iterator& iter)
{
	//UnRegisterAutoComplete(iter->first);

	ICVar* pCVar	  = iter->second;
	const int32 flags = pCVar->GetFlags();
	if (flags & VF_CHEAT_ALWAYS_CHECK)
		RemoveCheckedCVar(m_alwaysCheckedVariables, std::make_pair(iter->first.data(), iter->second));
	else if ((flags & (VF_CHEAT | VF_CHEAT_NOCHECK)) == VF_CHEAT)
		RemoveCheckedCVar(m_randomCheckedVariables, std::make_pair(iter->first.data(), iter->second));

	m_mapVariables.erase(iter);

	for (auto& it : m_consoleVarSinks)
		it->OnVarUnregister(pCVar);

	if (pCVar->IsOwnedByConsole())
		delete pCVar;
}

void CXConsole::RegisterVar(const string& name, ICVar* pCVar, ConsoleVarFunc pChangeFunc)
{
	const bool isConst		= pCVar->GetFlags() & VF_CONST_CVAR != 0;
	const bool isCheat		= ((pCVar->GetFlags() & (VF_CHEAT | VF_CHEAT_NOCHECK | VF_CHEAT_ALWAYS_CHECK)) != 0);
	const bool isReadOnly	= ((pCVar->GetFlags() & VF_READONLY) != 0);
	const bool isDeprecated = ((pCVar->GetFlags() & VF_DEPRECATED) != 0);

	auto it = m_configVars.find(pCVar->GetName());
	if (it != m_configVars.end())
	{
		SConfigVar& var				  = it->second;
		bool allowChange			  = true;
		const bool wasProcessingGroup = GetIsProcessingGroup();
		SetProcessingGroup(var.m_partOfGroup);

		if (
#if CVAR_GROUPS_ARE_PRIVILEGED
			!m_bIsProcessingGroup &&
#endif // !CVAR_GROUPS_ARE_PRIVILEGED
			(isConst || isCheat || isReadOnly || isDeprecated))
		{
			allowChange = !isDeprecated && ((gEnv->pSystem->IsDevMode()) || (gEnv->IsEditor()));
			if (pCVar->GetString() != var.m_value && !allowChange)
			{
#if LOG_CVAR_INFRACTIONS
				LogChangeMessage(pCVar->GetName(), isConst, isCheat,
								 isReadOnly, isDeprecated, pCVar->GetString(), var.m_value.c_str(), m_bIsProcessingGroup, allowChange);
#	if LOG_CVAR_INFRACTIONS_CALLSTACK
				gEnv->pSystem->debug_LogCallStack();
#	endif // LOG_CVAR_INFRACTIONS_CALLSTACK
#endif	   // LOG_CVAR_INFRACTIONS
			}
		}

		if (allowChange)
		{
			pCVar->ForceSet(var.m_value.c_str());
			pCVar->SetFlags(pCVar->GetFlags() | var.nCVarOrFlags);
		}

		SetProcessingGroup(wasProcessingGroup);
		m_configVars.erase(it);
	}
	else
	{
		// Variable is not modified when just registered.
		pCVar->ClearFlags(VF_MODIFIED);
	}

	if (pChangeFunc)
		pCVar->AddOnChange(pChangeFunc);

	auto pair = std::make_pair(name.data(), pCVar);
	m_mapVariables.insert(pair);

	const int flags = pCVar->GetFlags();

	if (flags & VF_CHEAT_ALWAYS_CHECK)
		AddCheckedCVar(m_alwaysCheckedVariables, pair);
	else if ((flags & (VF_CHEAT | VF_CHEAT_NOCHECK)) == VF_CHEAT)
		AddCheckedCVar(m_randomCheckedVariables, pair);
}

void CXConsole::RemoveCommand(const char* sName)
{
	m_mapCommands.erase(sName);
#if 0
	UnRegisterAutoComplete(sName);
#endif
}

void CXConsole::SetInputLine(const char* szLine)
{
	CRY_ASSERT(szLine);
	m_sInputBuffer = szLine;
	m_nCursorPos = m_sInputBuffer.size();
}

void CXConsole::SetScrollMax(int value)
{
	m_nScrollMax	 = value;
	m_nTempScrollMax = m_nScrollMax;
}

void CXConsole::AddOutputPrintSink(IOutputPrintSink* inpSink)
{
}

void CXConsole::RemoveOutputPrintSink(IOutputPrintSink* inpSink)
{
}

void CXConsole::ShowConsole(bool show)
{
	if (m_pSysDeactivateConsole->GetIVal())
		show = false;

	if (show && !m_bConsoleActive)
	{
		if (gEnv->pHardwareMouse)
			gEnv->pHardwareMouse->IncrementCounter();
	}
	else if (!show && m_bConsoleActive)
	{
		if (gEnv->pHardwareMouse)
			gEnv->pHardwareMouse->DecrementCounter();
	}

	SetStatus(show);

#if 0
	if (iRequestScrollMax > 0)
		m_nTempScrollMax = iRequestScrollMax;     // temporary user request
	else
#endif
	m_nTempScrollMax = m_nScrollMax; // reset

	if (m_bConsoleActive)
		m_sdScrollDir = sdDOWN;
	else
		m_sdScrollDir = sdUP;
}

int CXConsole::Register(const char* name, void* src, float defaultvalue, int flags, int type, const char* help /* = ""*/)
{
	return 0;
}

float CXConsole::Register(const char* name, float* src, float defaultvalue, int flags /* = 0*/, const char* help /* = ""*/)
{
	return 0.0f;
}

int CXConsole::Register(const char* name, int* src, float defaultvalue, int flags /* = 0*/, const char* help /* = ""*/)
{
	return 0;
}

void CXConsole::DumpCVars(ICVarDumpSink* pCallback, unsigned int nFlagsFilter /* = 0*/)
{
	for (auto& pair : m_mapVariables)
	{
		if ((nFlagsFilter == 0) || ((nFlagsFilter != 0) && (pair.second->GetFlags() & nFlagsFilter)))
			pCallback->OnElementFound(pair.second);
	}
}

void CXConsole::CreateKeyBind(const char* sCmd, const char* sRes, bool bExecute)
{
	m_mapBinds.insert({sCmd, sRes});
}

void CXConsole::SetImage(ITexPic* pImage, bool bDeleteCurrent)
{
}

ITexPic* CXConsole::GetImage()
{
	return nullptr;
}

void CXConsole::StaticBackground(bool bStatic)
{
}

void CXConsole::SetLoadingImage(const char* szFilename)
{
}

bool CXConsole::GetLineNo(const DWORD indwLineNo, char* outszBuffer, const DWORD indwBufferSize) const
{
	return false;
}

int CXConsole::GetLineCount() const
{
	return 0;
}

ICVar* CXConsole::GetCVar(const char* sName, const bool bCaseSensitive /* = true*/)
{
	CRY_ASSERT(sName, "Calling GetCVar with a nullptr");

	if (con_debug)
	{
		CryLog("GetCVar(\"%s\") called", sName);
		#if 0
		m_system.debug_LogCallStack();
		#endif
	}

	auto it = m_mapVariables.find(sName);
	if (it != m_mapVariables.end())
		return it->second;

	return nullptr;
	return nullptr;
}

CXFont* CXConsole::GetFont()
{
	return nullptr;
}

void CXConsole::Help(const char* command /* = NULL*/)
{
}

char* CXConsole::GetVariable(const char* szVarName, const char* szFileName, const char* def_val)
{
	return nullptr;
}

float CXConsole::GetVariable(const char* szVarName, const char* szFileName, float def_val)
{
	return 0.0f;
}

void CXConsole::PrintLine(const char* s)
{
}

void CXConsole::PrintLinePlus(const char* s)
{
}

bool CXConsole::GetStatus()
{
	return m_bConsoleActive;
	return false;
}

void CXConsole::Clear()
{
}

void CXConsole::Update()
{
	// Execute the deferred commands
	ExecuteDeferredCommands();

	m_pRenderer = m_system.GetIRenderer();

	if (!m_bConsoleActive)
		m_nRepeatEvent.keyId = eKI_Unknown;

	// Process Key press repeat (backspace and cursor on PC)
	if (m_nRepeatEvent.keyId != eKI_Unknown)
	{
		const float fRepeatDelay = 1.0f / 40.0f;      // in sec (similar to Windows default but might differ from actual setting)
		const float fHitchDelay = 1.0f / 10.0f;       // in sec. Very low, but still reasonable frame-rate (debug builds)

		m_fRepeatTimer -= gEnv->pTimer->GetRealFrameTime();                     // works even when time is manipulated
		// m_fRepeatTimer -= gEnv->pTimer->GetFrameTime(ITimer::ETIMER_UI);		// can be used once ETIMER_UI works even with t_FixedTime

		if (m_fRepeatTimer <= 0.0f)
		{
			if (m_fRepeatTimer < -fHitchDelay)
			{
				// bad framerate or hitch
				m_nRepeatEvent.keyId = eKI_Unknown;
			}
			else
			{
				ProcessInput(m_nRepeatEvent);
				m_fRepeatTimer = fRepeatDelay;      // next repeat even in .. sec
			}
		}
	}

	#if 0
	CNotificationNetworkConsole::Update();
	#endif
}

void CXConsole::Draw()
{
}

void CXConsole::AddCommand(const char* sName, const char* sScriptFunc, const DWORD indwFlags /* = 0*/, const char* help /* = ""*/)
{
}

void CXConsole::ExecuteString(const char* command, bool bNeedSlash /* = false*/, bool bIgnoreDevMode /* = false*/)
{
}

void CXConsole::Exit(const char* command, ...)
{
}

bool CXConsole::IsOpened()
{
	return false;
}

int CXConsole::GetNumVars()
{
	return 0;
}

void CXConsole::GetSortedVars(const char** pszArray, size_t numItems)
{
}

const char* CXConsole::AutoComplete(const char* substr)
{
	return nullptr;
}

const char* CXConsole::AutoCompletePrev(const char* substr)
{
	return nullptr;
}

char* CXConsole::ProcessCompletion(const char* szInputBuffer)
{
	return nullptr;
}

void CXConsole::ResetAutoCompletion()
{
}

void CXConsole::DumpCommandsVars(char* prefix)
{
}

void CXConsole::GetMemoryUsage(ICrySizer* pSizer)
{
}

void CXConsole::ResetProgressBar(int nProgressRange)
{
}

void CXConsole::TickProgressBar()
{
}

void CXConsole::DumpKeyBinds(IKeyBindDumpSink* pCallback)
{
	for (auto& pair : m_mapBinds)
		pCallback->OnKeyBindFound(pair.first.c_str(), pair.second.c_str());
}

const char* CXConsole::FindKeyBind(const char* sCmd)
{
	auto it = m_mapBinds.find(CONST_TEMP_STRING(sCmd));
	if (it != m_mapBinds.end())
		return it->second.c_str();

	return nullptr;
}

void CXConsole::AddConsoleVarSink(IConsoleVarSink* pSink)
{
}

void CXConsole::RemoveConsoleVarSink(IConsoleVarSink* pSink)
{
}

const char* CXConsole::GetHistoryElement(const bool bUpOrDown)
{
	return nullptr;
}

void CXConsole::AddCommandToHistory(const char* szCommand)
{
}

//enable this for now, we need it for profiling etc
//MUST DISABLE FOR TCG BUILDS
#define PROCESS_XCONSOLE_INPUT

bool CXConsole::OnInputEvent(const SInputEvent& event)
{
#ifdef PROCESS_XCONSOLE_INPUT

	// Process input event
	if (event.state == eIS_Released && m_bConsoleActive)
		m_nRepeatEvent.keyId = eKI_Unknown;

	if (event.state != eIS_Pressed)
		return false;

	// restart cursor blinking
	m_fCursorBlinkTimer = 0.0f;
	m_bDrawCursor = true;

	// key repeat
	const float fStartRepeatDelay = 0.5f;           // in sec (similar to Windows default but might differ from actual setting)
	m_nRepeatEvent = event;
	m_fRepeatTimer = fStartRepeatDelay;

	//execute Binds
	if (!m_bConsoleActive)
	{
		const char* cmd = 0;

		if (event.modifiers == 0)
		{
			// fast
			cmd = FindKeyBind(event.keyName.c_str());
		}
		else
		{
			// slower
			char szCombinedName[40];
			int iLen = 0;

			if (event.modifiers & eMM_Ctrl)
			{ strcpy(szCombinedName, "ctrl_"); iLen += 5; }
			if (event.modifiers & eMM_Shift)
			{ strcpy(&szCombinedName[iLen], "shift_"); iLen += 6; }
			if (event.modifiers & eMM_Alt)
			{ strcpy(&szCombinedName[iLen], "alt_");  iLen += 4; }
			if (event.modifiers & eMM_Win)
			{ strcpy(&szCombinedName[iLen], "win_");  iLen += 4; }

			strcpy(&szCombinedName[iLen], event.keyName.c_str());

			cmd = FindKeyBind(szCombinedName);
		}

		if (cmd)
		{
			SetInputLine("");
			ExecuteStringInternal(cmd, true);    // keybinds are treated as they would come from console
		}
	}
	else
	{
		if (event.keyId != eKI_Tab)
			ResetAutoCompletion();

		if (event.keyId == eKI_V && (event.modifiers & eMM_Ctrl) != 0)
		{
			Paste();
			return false;
		}

		if (event.keyId == eKI_C && (event.modifiers & eMM_Ctrl) != 0)
		{
			Copy();
			return false;
		}
	}

	// keep only bare tilde key, modified one may be used by someone else - such as editor suspend
	if (event.keyId == eKI_Tilde && !(event.modifiers & (eMM_Shift | eMM_Ctrl | eMM_Alt)))
	{
		if (m_bActivationKeyEnable)
		{
			m_sInputBuffer = "";
			m_nCursorPos = 0;
			m_pInput->ClearKeyState();
			ShowConsole(!GetStatus());
			m_nRepeatEvent.keyId = eKI_Unknown;
			return true;
		}
	}
	// Normally, this will notify the editor to switch out of game mode, but in order to allow access to game functionality bound to the Escape key, we skip it if Shift is held down
	if (event.keyId == eKI_Escape && ((event.modifiers & eMM_Shift) == 0 || !gEnv->IsEditor()))
	{
		//switch process or page or other things
		m_sInputBuffer = "";
		m_nCursorPos = 0;

		ShowConsole(false);

		ISystemUserCallback* pCallback = m_system.GetUserCallback();
		if (pCallback)
			pCallback->OnProcessSwitch();

		return false;
	}

	return ProcessInput(event);

#else

	return false;

#endif
}

void CXConsole::OnElementFound(ICVar* pCVar)
{
}

bool CXConsole::ProcessInput(const SInputEvent& event)
{
#ifdef PROCESS_XCONSOLE_INPUT

	if (!m_bConsoleActive || m_readOnly)
		return false;

	// this is not so super-nice as the XKEY's ... but a small price to pay
	// if speed is a problem (which would be laughable for this) the CCryName
	// can be cached in a static var
	if (event.keyId == eKI_Enter || event.keyId == eKI_NP_Enter)
	{
		ExecuteInputBuffer();
		m_nScrollLine = 0;
		return true;
	}
	else if (event.keyId == eKI_Backspace)
	{
		RemoveInputChar(true);
		return true;
	}
	else if (event.keyId == eKI_Left)
	{
		if (m_nCursorPos)
		{
			const char* pCursor = m_sInputBuffer.c_str() + m_nCursorPos;
			#if 0
			Unicode::CIterator<const char*, false> pUnicode(pCursor);
			--pUnicode; // Note: This moves back one UCS code-point, but doesn't necessarily match one displayed character (ie, combining diacritics)
			pCursor = pUnicode.GetPosition();
			m_nCursorPos = pCursor - m_sInputBuffer.c_str();
			#else
			m_nCursorPos = pCursor - m_sInputBuffer.c_str() - 1;
			#endif
		}
		return true;
	}
	else if (event.keyId == eKI_Right)
	{
		if (m_nCursorPos < (int)(m_sInputBuffer.length()))
		{
			const char* pCursor = m_sInputBuffer.c_str() + m_nCursorPos;
			#if 0
			Unicode::CIterator<const char*, false> pUnicode(pCursor);
			++pUnicode; // Note: This moves forward one UCS code-point, but doesn't necessarily match one displayed character (ie, combining diacritics)
			pCursor = pUnicode.GetPosition();
			m_nCursorPos = pCursor - m_sInputBuffer.c_str();
			#else
			m_nCursorPos = pCursor - m_sInputBuffer.c_str() - 1;
			#endif
		}
		return true;
	}
	else if (event.keyId == eKI_Up)
	{
		const char* szHistoryLine = GetHistoryElement(true);    // true=UP

		if (szHistoryLine)
		{
			m_sInputBuffer = szHistoryLine;
			m_nCursorPos = (int)m_sInputBuffer.size();
		}
		return true;
	}
	else if (event.keyId == eKI_Down)
	{
		const char* szHistoryLine = GetHistoryElement(false);   // false=DOWN

		if (szHistoryLine)
		{
			m_sInputBuffer = szHistoryLine;
			m_nCursorPos = (int)m_sInputBuffer.size();
		}
		return true;
	}
	else if (event.keyId == eKI_Tab)
	{
		if (!(event.modifiers & eMM_Alt))
		{
			m_sInputBuffer = ProcessCompletion(m_sInputBuffer.c_str());
			m_nCursorPos = m_sInputBuffer.size();
		}
		return true;
	}
	else if (event.keyId == eKI_PgUp || event.keyId == eKI_MouseWheelUp)
	{
		if (event.modifiers & eMM_Ctrl)
			m_nScrollLine = std::min((int)(m_dqConsoleBuffer.size() - 1), m_nScrollLine + 21);
		else
			m_nScrollLine = std::min((int)(m_dqConsoleBuffer.size() - 1), m_nScrollLine + 1);

		return true;
	}
	else if (event.keyId == eKI_PgDn || event.keyId == eKI_MouseWheelDown)
	{
		if (event.modifiers & eMM_Ctrl)
			m_nScrollLine = std::max(0, m_nScrollLine - 21);
		else
			m_nScrollLine = std::max(0, m_nScrollLine - 1);

		return true;
	}
	else if (event.keyId == eKI_Home)
	{
		if (event.modifiers & eMM_Ctrl)
			m_nScrollLine = m_dqConsoleBuffer.size() - 1;
		else
			m_nCursorPos = 0;

		return true;
	}
	else if (event.keyId == eKI_End)
	{
		if (event.modifiers & eMM_Ctrl)
			m_nScrollLine = 0;
		else
			m_nCursorPos = (int)m_sInputBuffer.length();

		return true;
	}
	else if (event.keyId == eKI_Delete)
	{
		RemoveInputChar(false);
		return true;
	}
	else
	{
		// Below is a hack due to pc having character input event being caught when in the editor and also due to inconsistencies in keyboard devices (Some fire OnInputEvent and OnInputEventUI and some only fire OnInputEvent)
		// i.e. OnInputEventUI will never be fired
		// The below isn't true unicode, it's converted from ascii
		// TODO: Rework windows processing of input (WM_CHAR) into CKeyboard (Both cases when in editor and not) and make all keyboard devices consistent and can remove the below code
		if (gEnv->IsEditor())
		{
			const uint32 inputChar = m_pInput->GetInputCharUnicode(event);

			if (inputChar)
			{
				AddInputChar(inputChar);
				return true;
			}
		}
	}

#endif

	return false;
}

void CXConsole::LoadConfigVar(const char* sVariable, const char* sValue)
{
}

void CXConsole::LoadConfigCommand(const char* szCommand, const char* szArguments)
{
}

void CXConsole::ExecuteDeferredCommands()
{
	if (m_waitFrames)
	{
		--m_waitFrames;
		return;
	}

	#if 0
	if (m_waitSeconds.GetValue())
	{
		if (m_waitSeconds > gEnv->pTimer->GetFrameStartTime())
			return;

		m_waitSeconds.SetValue(0); 	// Help to avoid overflow problems
	}
	#endif

	const int blockCounter = m_blockCounter;

	#if 0
	while (m_waitFrames == 0 && m_waitSeconds.GetValue() == 0 && !m_deferredCommands.empty())
	{
		auto& deferredCommand = m_deferredCommands.front();
		ExecuteStringInternal(deferredCommand.command.c_str(), false, deferredCommand.silentMode);
		m_deferredCommands.pop_front();

		// A blocker command was executed
		if (m_blockCounter != blockCounter)
			break;
	}
	#endif
}

void CXConsole::ExecuteInputBuffer()
{
	string sTemp = m_sInputBuffer;
	if (m_sInputBuffer.empty())
		return;
	m_sInputBuffer = "";

	AddCommandToHistory(sTemp.c_str());

	#if 0
	if (gEnv->pSystem->IsCVarWhitelisted(sTemp.c_str(), false))
	#endif
		ExecuteStringInternal(sTemp.c_str(), true);   // from console

	m_nCursorPos = 0;
}
