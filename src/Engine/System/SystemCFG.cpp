#include "pch.hpp"
#include <BlackBox/System/System.hpp>
#include <BlackBox/System/ILog.hpp>
#include "SystemCFG.hpp"

#include <string>
#include <fstream>
#include <streambuf>

string Trim(string& str)
{
	int begin = 0;
	int end = 0;
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

CSystemConfiguration::CSystemConfiguration(const string& strSysConfigFilePath, CSystem* pSystem, ILoadConfigurationEntrySink* pSink)
	: 
	m_strSysConfigFilePath(strSysConfigFilePath), 
	m_bError(false), m_pSink(pSink)
{
	ASSERT(pSink);
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
		strLine = Trim(strLine);

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
			strKey = Trim(strKey);

			// extract value and remove surrounding quotes, if present
			string strValue(strLine, posEq + 1, strLine.size() - (posEq + 1));
			strValue = Trim(strValue);
			if (strValue.front() == '"' && strValue.back() == '"')
			{
				strValue.pop_back();
				strValue.erase(0, 1);
			}
			
			//strValue.replace()
			//strValue.replace("\\\\", "\\");
			//strValue.replace("\\\"", "\"");

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
}

void CSystem::Warning(EValidatorModule module, EValidatorSeverity severity, int flags, const char* file, const char* format, ...)
{
}

bool CSystem::CheckLogVerbosity(int verbosity)
{
	return false;
}

void CSystem::LoadConfiguration(const string& sFilename)
{
	if (sFilename.empty() && sFilename.size() > 0)
	{
		CSystemConfiguration tempConfig(sFilename, this, this);
	}
	else
	{
		CSystemConfiguration tempConfig("system.cfg", this, this);
	}
}

