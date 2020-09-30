#include "SystemThreading.h"
#include <BlackBox/System/System.hpp>

IThreadConfigManager* CThreadManager::GetThreadConfigManager()
{
	return nullptr;
}

bool CThreadManager::SpawnThread(IThread* pThread, const char* sThreadName, ...)
{
	return false;
}

bool CThreadManager::JoinThread(IThread* pThreadTask, EJoinMode joinStatus)
{
	return false;
}

bool CThreadManager::RegisterThirdPartyThread(void* pThreadHandle, const char* sThreadName, ...)
{
	return false;
}

bool CThreadManager::UnRegisterThirdPartyThread(const char* sThreadName, ...)
{
	return false;
}

const char* CThreadManager::GetThreadName(threadID nThreadId)
{
	return nullptr;
}

threadID CThreadManager::GetThreadId(const char* sThreadName, ...)
{
	return threadID();
}

void CThreadManager::ForEachOtherThread(IThreadManager::ThreadModifFunction fpThreadModiFunction, void* pFuncData/* = 0*/)
{
}

void CThreadManager::EnableFloatExceptions(EFPE_Severity eFPESeverity, threadID nThreadId/* = 0*/)
{
}

void CThreadManager::EnableFloatExceptionsForEachOtherThread(EFPE_Severity eFPESeverity)
{
}

uint CThreadManager::GetFloatingPointExceptionMask()
{
	return uint();
}

void CThreadManager::SetFloatingPointExceptionMask(uint nMask)
{
}

//////////////////////////////////////////////////////////////////////////
void CSystem::InitThreadSystem()
{
	m_env.pThreadManager = CreateThreadManager();
}

//////////////////////////////////////////////////////////////////////////
void CSystem::ShutDownThreadSystem()
{
	SAFE_DELETE(m_env.pThreadManager);
}

//////////////////////////////////////////////////////////////////////////
IThreadManager* CreateThreadManager()
{
	return new CThreadManager();
}


