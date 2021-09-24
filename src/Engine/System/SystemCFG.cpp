#include "pch.hpp"
#include <BlackBox/System/System.hpp>
#include "SystemCFG.hpp"


namespace detail
{
	string Trim(string& str)
	{
		int begin = 0;
		int end	  = 0;
		for (int i = 0; i < str.size() && isspace(str[i]); i++)
		{
			begin++;
		}
		for (int i = str.size() - 1; i >= 0 && isspace(str[i]); i--)
		{
			end++;
		}
		str.resize(str.size() - end);
		str.erase(0, begin);
		return str;
	}
}

CSystemConfiguration::CSystemConfiguration(const string& strSysConfigFilePath, CSystem* pSystem, ILoadConfigurationEntrySink* pSink)
	: 
	m_strSysConfigFilePath(strSysConfigFilePath), 
	m_bError(false), m_pSink(pSink)
{
	CRY_ASSERT(pSink);
	m_pSystem = pSystem;
	m_bError = !ParseSystemConfig();
}

bool CSystemConfiguration::ParseSystemConfig()
{
	string filename = m_strSysConfigFilePath;
	std::ifstream t(filename);
	std::string szFullText((std::istreambuf_iterator<char>(t)),
									 std::istreambuf_iterator<char>());

	auto strLast = szFullText.c_str() + szFullText.size();
	char* str = (char*)szFullText.c_str();
	while (str < strLast)
	{
		char* szLineStart = str;
		// find the first line break
		while (str < strLast && *str != '\n' && *str != '\r')
			str++;
		// clear it to handle the line as one string
		*str = '\0';
		str++;
		// and skip any remaining line break chars
		while (str < strLast && (*str == '\n' || *str == '\r'))
			str++;

		//trim all whitespace characters at the beginning and the end of the current line
		string strLine = szLineStart;
		strLine = detail::Trim(strLine);

		//skip empty lines
		if (strLine.empty())
			continue;

		// detect groups e.g. "[General]" should set strGroup="General"
		#if 0
		const size_t strLineSize = strLine.size();
		if (strLineSize >= 3)
		{
			if (strLine[0] == '[' && strLine[strLineSize - 1] == ']') // currently no comments are allowed to be behind groups
			{
				strGroup = &strLine[1]; // removes '['
				strGroup.pop_back();    // removes ']'
				continue;               // next line
			}
		}
		#endif

		//skip comments, comments start with ";" or "--" (any preceding whitespaces have already been trimmed)
		if (strLine[0] == ';' || strLine.find("--") == 0)
			continue;
		
		//if line contains a '=' try to read and assign console variable
		const string::size_type posEq(strLine.find("=", 0));
		if (string::npos != posEq)
		{
			string strKey(strLine, 0, posEq);
			strKey = detail::Trim(strKey);

			// extract value and remove surrounding quotes, if present
			string strValue(strLine, posEq + 1, strLine.size() - (posEq + 1));
			strValue = detail::Trim(strValue);
			if (strValue.front() == '"' && strValue.back() == '"')
			{
				strValue.pop_back();
				strValue.erase(0, 1);
			}
			
			//strValue.replace()
			//strValue.replace("\\\\", "\\");
			//strValue.replace("\\\"", "\"");

			if (strKey == "r_DisplayIndex")
			{
				CryLog("here r_DisplayIndex");
			}
			m_pSink->OnLoadConfigurationEntry(strKey.c_str(), strValue.c_str(), nullptr);
		}
		else
		{
			gEnv->pLog->LogWarning("%s -> invalid configuration line: %s", filename.c_str(), strLine.c_str());
		}
	}

	m_pSink->OnLoadConfigurationEntry_End();
	return true;
	return false;
}

//////////////////////////////////////////////////////////////////////////
void CSystem::OnLoadConfigurationEntry(const char* szKey, const char* szValue, const char* szGroup)
{
	if (!gEnv->pConsole)
		return;

	if (*szKey != 0)
		gEnv->pConsole->LoadConfigVar(szKey, szValue);
}

void CSystem::Relaunch(bool bRelaunch)
{
}

bool CSystem::IsQuitting()
{
	return false;
}

void CSystem::Error(const char* sFormat, ...)
{
	va_list	ArgList;
	char szBuffer[MAX_WARNING_LENGTH];
	va_start(ArgList, sFormat);
	vsprintf(szBuffer, sFormat, ArgList);
	va_end(ArgList);
	gEnv->pLog->LogError(szBuffer);
}

void CSystem::Warning(EValidatorModule module, EValidatorSeverity severity, int flags, const char* file, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	WarningV(module, severity, flags, file, format, args);
	va_end(args);
}

inline const char* ValidatorModuleToString(EValidatorModule module)
{
	switch (module)
	{
	case VALIDATOR_MODULE_RENDERER:
		return "Renderer";
	case VALIDATOR_MODULE_3DENGINE:
		return "3DEngine";
	case VALIDATOR_MODULE_ASSETS:
		return "Assets";
	case VALIDATOR_MODULE_AI:
		return "AI";
	case VALIDATOR_MODULE_ANIMATION:
		return "Animation";
	case VALIDATOR_MODULE_ENTITYSYSTEM:
		return "EntitySystem";
	case VALIDATOR_MODULE_SCRIPTSYSTEM:
		return "Script";
	case VALIDATOR_MODULE_SYSTEM:
		return "System";
	case VALIDATOR_MODULE_AUDIO:
		return "Audio";
	case VALIDATOR_MODULE_GAME:
		return "Game";
	case VALIDATOR_MODULE_MOVIE:
		return "Movie";
	case VALIDATOR_MODULE_EDITOR:
		return "Editor";
	case VALIDATOR_MODULE_NETWORK:
		return "Network";
	case VALIDATOR_MODULE_PHYSICS:
		return "Physics";
	case VALIDATOR_MODULE_FLOWGRAPH:
		return "FlowGraph";
	case VALIDATOR_MODULE_ONLINE:
		return "Online";
	case VALIDATOR_MODULE_DRS:
		return "DynamicResponseSystem";
	default:
		break;
	}
	return "";
}

void CSystem::WarningV(EValidatorModule module, EValidatorSeverity severity, int flags, const char* file, const char* format, va_list args)
{
	// Fran: No logging in a testing environment
	if (m_env.pLog == 0)
	{
		return;
	}

	#if 0
	const char* sModuleFilter = m_env.pLog->GetModuleFilter();
	if (sModuleFilter && *sModuleFilter != 0)
	{
		const char* sModule = ValidatorModuleToString(module);
		if (strlen(sModule) > 1 || CryStringUtils::stristr(sModule, sModuleFilter) == 0)
		{
			// Filter out warnings from other modules.
			return;
		}
	}
	#endif

	bool bDbgBreak = false;
	if (severity == VALIDATOR_ERROR_DBGBRK)
	{
		bDbgBreak = true;
		severity = VALIDATOR_ERROR; // change it to a standard VALIDATOR_ERROR for simplicity in the rest of the system
	}

	IMiniLog::ELogType ltype = ILog::eComment;
	switch (severity)
	{
	case VALIDATOR_ERROR:
		ltype = ILog::eError;
		break;
	case VALIDATOR_WARNING:
		ltype = ILog::eWarning;
		break;
	case VALIDATOR_COMMENT:
		ltype = ILog::eComment;
		break;
	case VALIDATOR_ASSERT:
		ltype = ILog::eAssert;
		break;
	default:
		break;
	}
	char szBuffer[MAX_WARNING_LENGTH];
	vsprintf(szBuffer, format, args);

	if (file && *file)
	{
		string fmt = szBuffer;
		fmt += " [File=";
		fmt += file;
		fmt += "]";

		m_env.pLog->LogWithType(ltype, flags | VALIDATOR_FLAG_SKIP_VALIDATOR, "%s", fmt.c_str());
	}
	else
	{
		m_env.pLog->LogWithType(ltype, flags | VALIDATOR_FLAG_SKIP_VALIDATOR, "%s", szBuffer);
	}

	//if(file)
	//m_env.pLog->LogWithType( ltype, "  ... caused by file '%s'",file);

	#if 0
	if (m_pValidator && (flags & VALIDATOR_FLAG_SKIP_VALIDATOR) == 0)
	{
		SValidatorRecord record;
		record.file = file;
		record.text = szBuffer;
		record.module = module;
		record.severity = severity;
		record.flags = flags;
		record.assetScope = m_env.pLog->GetAssetScopeString();
		m_pValidator->Report(record);
	}
	#endif

#if !defined(_RELEASE)
	if (bDbgBreak && g_cvars.sys_error_debugbreak)
		__debugbreak();
#endif

}

bool CSystem::CheckLogVerbosity(int verbosity)
{
	//FIXME: fix
	//return false;
	return true;
}

void CSystem::LoadConfiguration(const char* sFilename, ILoadConfigurationEntrySink* pSink, ELoadConfigurationType configType, ELoadConfigurationFlags flags)
{
	if (sFilename && strlen(sFilename) > 0)
	{
		CSystemConfiguration tempConfig(sFilename, this, this);
	}
	else
	{
		CSystemConfiguration tempConfig("system.cfg", this, this);
	}
}

