// Copyright 2001-2019 Crytek GmbH / Crytek Group. All rights reserved.

#include "pch.hpp"
#include "XConsole.h"
#include "XConsoleVariable.h"

#if 0
void CXConsoleVariableCVarGroup::OnLoadConfigurationEntry(const char* szKey, const char* szValue, const char* szGroup)
{
	CRY_ASSERT(szGroup != nullptr);
	CRY_ASSERT(szKey != nullptr);
	CRY_ASSERT(szValue != nullptr);

	bool checkIfInDefault = false;
	SCVarGroup* pGroup = nullptr;
	if (stricmp(szGroup, "default") == 0) // needs to be before the other groups
	{
		pGroup = &m_CVarGroupDefault;

		if (*szKey == 0)
		{
			m_defaultValue = szValue;
			int groupValue = atoi(szValue);

			// if default state is not part of the mentioned states generate this state, so GetIRealVal() can return this state as well
			if (m_CVarGroupStates.find(groupValue) == m_CVarGroupStates.end())
				m_CVarGroupStates[groupValue] = new SCVarGroup;

			return;
		}
	}
	else
	{
		int group;
		if (sscanf(szGroup, "%d", &group) == 1)
		{
			if (m_CVarGroupStates.find(group) == m_CVarGroupStates.end())
				m_CVarGroupStates[group] = new SCVarGroup;

			pGroup = m_CVarGroupStates[group];
			checkIfInDefault = true;
		}
		else
		{
			Env::Log()->LogError("[CVARS]: [MISSING] [%s] is not a registered console variable group", szGroup);
	#if LOG_CVAR_INFRACTIONS_CALLSTACK
			Env::System()->debug_LogCallStack();
	#endif // LOG_CVAR_INFRACTIONS_CALLSTACK
			return;
		}

		if (*szKey == 0)
		{
			CRY_ASSERT(false);      // =%d only expected in default section
			return;
		}
	}

	if (pGroup)
	{
		if (pGroup->m_KeyValuePair.find(szKey) != pGroup->m_KeyValuePair.end())
			Env::Log()->LogError("[CVARS]: [DUPLICATE] [%s] specified multiple times in console variable group [%s] = [%s]", szKey, GetName(), szGroup);

		pGroup->m_KeyValuePair[szKey] = szValue;

		if (checkIfInDefault)
		{
			if (m_CVarGroupDefault.m_KeyValuePair.find(szKey) == m_CVarGroupDefault.m_KeyValuePair.end())
				Env::Log()->LogError("[CVARS]: [MISSING] [%s] specified in console variable group [%s] = [%s], but missing from default group", szKey, GetName(), szGroup);
		}
	}
}

void CXConsoleVariableCVarGroup::OnLoadConfigurationEntry_End()
{
	if (!m_defaultValue.empty())
	{
		Env::Console()->LoadConfigVar(GetName(), m_defaultValue.data());
		m_defaultValue.clear();
	}
}

CXConsoleVariableCVarGroup::CXConsoleVariableCVarGroup(IConsole* pConsole, const string& name, const char* szFileName, int flags)
	: CXConsoleVariableInt(pConsole, name, 0, flags, nullptr, true)
{
	#if 0
	Env::System()->LoadConfiguration(szFileName, this, eLoadConfigSystemSpec);
	#endif
}

string CXConsoleVariableCVarGroup::GetDetailedInfo() const
{
	string info = GetName();
	info += " [";

	for (auto it = m_CVarGroupStates.cbegin(); it != m_CVarGroupStates.cend(); ++it)
	{
		if (it != m_CVarGroupStates.cbegin())
			info += "/";

		char szNum[10];
		sprintf(szNum, "%d", it->first);
		info += szNum;
	}

	info += "/default] [current]:\n";

	for (auto it = m_CVarGroupDefault.m_KeyValuePair.cbegin(); it != m_CVarGroupDefault.m_KeyValuePair.cend(); ++it)
	{
		const string& key = it->first;

		info += " ... ";
		info += key;
		info += " = ";

		for (auto it2 = m_CVarGroupStates.cbegin(); it2 != m_CVarGroupStates.cend(); ++it2)
		{
			info += GetValueSpec(key, &(it2->first));
			info += "/";
		}
		info += GetValueSpec(key);
		ICVar* pCVar = Env::Console()->GetCVar(key.data());
		if (pCVar != nullptr)
		{
			info += " [";
			info += pCVar->GetString();
			info += "]";
		}

		info += "\n";
	}

	return info;
}

const char* CXConsoleVariableCVarGroup::GetHelp() const
{
	return const_cast<CXConsoleVariableCVarGroup*>(this)->GetHelpInternal();
}

const char* CXConsoleVariableCVarGroup::GetHelpInternal()
{
	// Code was moved to a seperate function as GetHelp() is const due to the interface and GCC/Clang don't allow modifying m_psHelp there

	m_helpMessage = "Console variable group to apply settings to multiple variables\n\n" + GetDetailedInfo();
	return m_helpMessage.data();
}
#endif

#if 0
void CXConsoleVariableCVarGroup::DebugLog(const int expectedValue, const ICVar::EConsoleLogMode mode) const
{
	SCVarGroup* pCurrentGroup = nullptr;
	TCVarGroupStateMap::const_iterator itCurrentGrp = m_CVarGroupStates.find(expectedValue);
	if (itCurrentGrp != m_CVarGroupStates.cend())
		pCurrentGroup = itCurrentGrp->second;

	// try the current state
	TestCVars(pCurrentGroup, mode);
}
#endif

#if 0
int CXConsoleVariableCVarGroup::GetRealIVal() const
{
	int value = GetIVal();
	SCVarGroup* pCurrentGroup = nullptr;
	{
		TCVarGroupStateMap::const_iterator itCurrentGrp = m_CVarGroupStates.find(value);
		if (itCurrentGrp != m_CVarGroupStates.cend())
			pCurrentGroup = itCurrentGrp->second;
	}

	// first try the current state
	if (TestCVars(pCurrentGroup))
		return value;

	// then all other
	for (auto it = m_CVarGroupStates.cbegin(); it != m_CVarGroupStates.cend(); ++it)
	{
		SCVarGroup* pLocalGroup = it->second;
		if (pLocalGroup == pCurrentGroup)
			continue;

		int localState = it->first;
		if (TestCVars(pLocalGroup))
			return localState;
	}

	return -1; // no state found that represent the current one
}
#endif
#if 0
void CXConsoleVariableCVarGroup::Set(const int i)
{
	if (i == m_value)
	{
		SCVarGroup* pCurrentGroup = nullptr;
		TCVarGroupStateMap::const_iterator itCurrentGroup = m_CVarGroupStates.find(m_value);
		if (itCurrentGroup != m_CVarGroupStates.cend())
			pCurrentGroup = itCurrentGroup->second;

	#if 0
		if (TestCVars(pCurrentGroup))
		{
			// All cvars in this group match the current state - no further action is necessary
			return;
		}
	#endif
	}

	char sTemp[128];
	sprintf(sTemp, "%d", i);

	CXConsole* const pConsole = static_cast<CXConsole*>(m_pConsole);
	bool wasProcessingGroup = pConsole->GetIsProcessingGroup();
	pConsole->SetProcessingGroup(true);
	if (pConsole->OnBeforeVarChange(this, sTemp))
	{
		m_flags |= VF_MODIFIED;
		m_value = i;

		CallOnChangeFunctions();
		pConsole->OnAfterVarChange(this);
	}
	pConsole->SetProcessingGroup(wasProcessingGroup);

	// Useful for debugging cvar groups
	//CryLogAlways("[CVARS]: CXConsoleVariableCVarGroup::Set() Group %s in state %d (wanted %d)", GetName(), m_value, i);
}

CXConsoleVariableCVarGroup::~CXConsoleVariableCVarGroup()
{
	for (auto& it : m_CVarGroupStates)
	{
		SCVarGroup* pGroup = it.second;
		SAFE_DELETE(pGroup);
	}
}

void CXConsoleVariableCVarGroup::OnCVarChangeFunc(ICVar* pVar)
{
	CXConsoleVariableCVarGroup* pThis = (CXConsoleVariableCVarGroup*)pVar;

	int value = pThis->GetIVal();

	// only the global sys_spec should be clamped by the max available spec and not the individual sys_spec_*
	if (stricmp(pThis->GetName(), "sys_spec") == 0)
	{
		int maxSpec = Env::System()->GetMaxConfigSpec();
		if (value > maxSpec)
		{
			value = maxSpec;
			pThis->m_value = value;
		}
	}


	SCVarGroup* pGroup = nullptr;
	TCVarGroupStateMap::const_iterator itGroup = pThis->m_CVarGroupStates.find(value);
	if (itGroup != pThis->m_CVarGroupStates.cend())
		pGroup = itGroup->second;

	if (pGroup)
	{
		pThis->ApplyCVars(*pGroup);
	}
	else
	{
		CryWarning(EValidatorModule::VALIDATOR_MODULE_SYSTEM, EValidatorSeverity::VALIDATOR_COMMENT
				, "Trying to set CVarGroup '%s' to unknown value %d; will fall back to default.", pThis->GetName(), value);
	}

	pThis->ApplyCVars(pThis->m_CVarGroupDefault, pGroup);
}

bool CXConsoleVariableCVarGroup::TestCVars(const SCVarGroup* pGroup, const ICVar::EConsoleLogMode mode) const
{
	if (pGroup)
		if (!_TestCVars(*pGroup, mode))
			return false;

	if (!_TestCVars(m_CVarGroupDefault, mode, pGroup))
		return false;

	return true;
}

bool CXConsoleVariableCVarGroup::_TestCVars(const SCVarGroup& group, const ICVar::EConsoleLogMode mode, const SCVarGroup* pExclude) const
{
	bool returnValue = true;
	for (const auto& it : group.m_KeyValuePair)
	{
		const string& key = it.first;
		const string& value = it.second;

		if (pExclude)
		{
			if (pExclude->m_KeyValuePair.find(key) != pExclude->m_KeyValuePair.end())
				continue;
		}

		
		if (ICVar* pVar = Env::Console()->GetCVar(key.c_str()))
		{
			if (pVar->GetFlags() & VF_CVARGRP_IGNOREINREALVAL) // Ignore the cvars which change often and shouldn't be used to determine state
				continue;

			bool isOk = true;

			// compare exact type,
			// simple string comparison would fail on some comparisons e.g. 2.0 == 2
			// and GetString() for int and float return pointer to shared array so this
			// can cause problems
			switch (pVar->GetType())
			{
			case ECVarType::Int:
			case ECVarType::Int64:
				{
					int iVal;
					if (sscanf(value.c_str(), "%d", &iVal) == 1)
					{
						if (pVar->GetIVal() != atoi(value.c_str()))
						{
							isOk = false;
							break;
						}
					}

					if (pVar->GetIVal() != pVar->GetRealIVal())
					{
						isOk = false;
						break;
					}
				}
				break;

			case ECVarType::Float:
				{
					float fVal;
					if (sscanf(value.c_str(), "%f", &fVal) == 1)
					{
						if (pVar->GetFVal() != fVal)
						{
							isOk = false;
							break;
						}
					}
				}
				break;

			case ECVarType::String:
				if (value != pVar->GetString())
				{
					isOk = false;
					break;
				}
				break;

			default:
				CRY_ASSERT(false);
			}

			if (!isOk)
			{
				if (mode == ICVar::eCLM_Off)
					return false; // exit as early as possible

				returnValue = false; // exit with same return code but log all differences

				if (strcmp(pVar->GetString(), value.c_str()) != 0)
				{
					switch (mode)
					{
					case ICVar::eCLM_ConsoleAndFile:
						CryLog("[CVARS]: $3[FAIL] [%s] = $6[%s] $4(expected [%s] in group [%s] = [%s])", key.c_str(), pVar->GetString(), value.c_str(), GetName(), GetString());
						break;

					case ICVar::eCLM_FileOnly:
					case ICVar::eCLM_FullInfo:
						Env::Log()->LogToFile("[CVARS]: [FAIL] [%s] = [%s] (expected [%s] in group [%s] = [%s])", key.c_str(), pVar->GetString(), value.c_str(), GetName(), GetString());
						break;

					default:
						CRY_ASSERT(false);
					}
				}
				else if (mode == ICVar::eCLM_FullInfo)
				{
					Env::Log()->LogToFile("[CVARS]: [FAIL] [%s] = [%s] (expected [%s] in group [%s] = [%s])", key.c_str(), pVar->GetString(), value.c_str(), GetName(), GetString());
				}

				pVar->DebugLog(pVar->GetIVal(), mode); // recursion
			}

			if ((pVar->GetFlags() & (VF_CHEAT | VF_CHEAT_ALWAYS_CHECK | VF_CHEAT_NOCHECK)) != 0)
			{
				// either VF_CHEAT should be removed or the var should be not part of the CVarGroup
				Env::Log()->LogError("[CVARS]: [%s] is cheat protected; referenced in console variable group [%s] = [%s] ", key.c_str(), GetName(), GetString());
			}
		}
		else
		{
			Env::Console()->LoadConfigVar(key.c_str(), value.c_str());
		}
	}

	return returnValue;
}

string CXConsoleVariableCVarGroup::GetValueSpec(const string& key, const int* pSpec) const
{
	if (pSpec)
	{
		TCVarGroupStateMap::const_iterator itGroup = m_CVarGroupStates.find(*pSpec);
		if (itGroup != m_CVarGroupStates.cend())
		{
			const SCVarGroup* pGroup = itGroup->second;

			// check in spec
			std::map<string, string>::const_iterator it = pGroup->m_KeyValuePair.find(key);
			if (it != pGroup->m_KeyValuePair.cend())
				return it->second;
		}
	}

	// check in default
	std::map<string, string>::const_iterator it = m_CVarGroupDefault.m_KeyValuePair.find(key);
	if (it != m_CVarGroupDefault.m_KeyValuePair.cend())
		return it->second;

	CRY_ASSERT(false, "Internal error");
	return "";
}

void CXConsoleVariableCVarGroup::ApplyCVars(const SCVarGroup& group, const SCVarGroup* pExclude)
{
	if (ICVar* pCVarLogging = Env::Console()->GetCVar("sys_cvar_logging"))
	{
		if (pCVarLogging->GetIVal() >= 2)
			CryLog("Applying CVar group '%s'", GetName());
	}

	CXConsole* const pConsole = static_cast<CXConsole*>(m_pConsole);
	const bool wasProcessingGroup = pConsole->GetIsProcessingGroup();
	pConsole->SetProcessingGroup(true);
	const bool isSpecGroup = (cry_strncmp(m_name.c_str(), "sys_spec") == 0);
	const ELoadConfigurationType wasConfigType = pConsole->SetCurrentConfigType(isSpecGroup ? eLoadConfigSystemSpec : eLoadConfigDefault);

	for (const auto& it : group.m_KeyValuePair)
	{
		const string& rKey = it.first;
		if (pExclude)
		{
			if (pExclude->m_KeyValuePair.find(rKey) != pExclude->m_KeyValuePair.end())
				continue;
		}
		m_pConsole->LoadConfigVar(rKey, it.second);
	}

	pConsole->SetCurrentConfigType(wasConfigType);
	pConsole->SetProcessingGroup(wasProcessingGroup);
}
#endif
