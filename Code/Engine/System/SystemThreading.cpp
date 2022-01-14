#include "SystemThreading.h"
#include "System.hpp"

void DumpThreads(IConsoleCmdArgs* args)
{
	static_cast<CThreadManager*>(gEnv->pThreadManager)->Dump();
}

IThreadConfigManager* CThreadManager::GetThreadConfigManager()
{
	return nullptr;
}

bool CThreadManager::SpawnThread(IThread* pThreadTask, const char* sThreadName, ...)
{
	va_list args;
	va_start(args, sThreadName);

	// Format thread name
	char strThreadName[THREAD_NAME_LENGTH_MAX];
	if (!vsprintf(strThreadName, sThreadName, args))
	{
		CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "<ThreadInfo>: ThreadName \"%s\" has been truncated to \"%s\". Max characters allowed: %i.", sThreadName, strThreadName, (int)sizeof(strThreadName) - 1);
	}

	// Spawn thread
	bool ret = SpawnThreadImpl(pThreadTask, strThreadName);

	if (!ret)
	{
		CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "<ThreadInfo>: CSystem::SpawnThread error spawning thread: \"%s\"", strThreadName);
	}
	else
	{
		CryLog("<ThreadInfo>: CSystem::SpawnThread thread spawned: \"%s\"", strThreadName);
	}

	va_end(args);
	return ret;
	return false;
}

bool CThreadManager::UnregisterThread(IThread* pThreadTask)
{
	AUTO_LOCK(m_spawnedThreadsLock);

	SpawnedThreadMapIter res = m_spawnedThreads.find(pThreadTask);
	if (res == m_spawnedThreads.end())
	{
		// Duplicate thread deletion
		CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "<ThreadInfo>: UnregisterThread: Unable to unregister thread. Thread name could not be found. Double deletion? IThread pointer: %p", pThreadTask);
		return false;
	}

	m_spawnedThreads.erase(res);

	CryLog("<ThreadInfo>: UnregisterThread: Unregistered thread <%s>", res->second->m_threadName.data());
	return true;
}

bool CThreadManager::SpawnThreadImpl(IThread* pThreadTask, const char* sThreadName)
{
	
	if (pThreadTask == NULL)
	{
		CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_ERROR, "<ThreadInfo>: SpawnThread '%s' ThreadTask is NULL : ignoring", sThreadName);
		return false;
	}

	// Init thread meta data
	SThreadMetaData* pThreadMetaData = new SThreadMetaData();
	pThreadMetaData->m_pThreadTask = pThreadTask;
	pThreadMetaData->m_pThreadMngr = this;
	pThreadMetaData->m_threadName = sThreadName;

	// Add thread to map
	{
		AUTO_LOCK(m_spawnedThreadsLock);
		SpawnedThreadMapIter res = m_spawnedThreads.find(pThreadTask);
		if (res != m_spawnedThreads.end())
		{
			// Thread with same name already spawned
			CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "<ThreadInfo>: SpawnThread: Thread \"%s\" already exists.", sThreadName);
			delete pThreadMetaData;
			return false;
		}

		// Insert thread data
		m_spawnedThreads.insert(ThreadMapPair(pThreadTask, pThreadMetaData));
	}

	#if 0
	// Load config if we can and if no config has been defined to be loaded
	const SThreadConfig* pThreadConfig = gEnv->pThreadManager->GetThreadConfigManager()->GetThreadConfig(sThreadName);

	// Create thread description
	CryThreadUtil::SThreadCreationDesc desc = { sThreadName, RunThread, pThreadMetaData, pThreadConfig->paramActivityFlag & SThreadConfig::eThreadParamFlag_StackSize ? pThreadConfig->stackSizeBytes : 0 };

	// Spawn new thread
	pThreadMetaData->m_isRunning = true;
	std::
	CryThreadUtil::CryCreateThread(&(pThreadMetaData->m_threadHandle), desc);
	#else
	pThreadMetaData->m_threadHandle = std::thread([pThreadTask,pThreadMetaData] { 
		pThreadTask->ThreadEntry(); 
		// Signal imminent thread end
		pThreadMetaData->m_threadExitMutex.Lock();
		pThreadMetaData->m_isRunning = false;
		pThreadMetaData->m_threadExitCondition.notify_all();
		pThreadMetaData->m_threadExitMutex.Unlock();

		// Unregister thread
		// Note: Unregister after m_threadExitCondition.Notify() to ensure pThreadData is still valid
		pThreadMetaData->m_pThreadMngr->UnregisterThread(pThreadMetaData->m_pThreadTask);
	
	});
	pThreadMetaData->m_isRunning = true;
	#endif


	// Validate thread creation
	if (!pThreadMetaData->m_isRunning)
	{
		CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "<ThreadInfo>: SpawnThread: Could not spawn thread \"%s\".", sThreadName);

		// Remove thread from map (also releases SThreadMetaData _smart_ptr)
		m_spawnedThreads.erase(m_spawnedThreads.find(pThreadTask));
		return false;
	}

	return true;
}

bool CThreadManager::JoinThread(IThread* pThreadTask, EJoinMode eJoinMode)
{
	
	// Get thread object
	_smart_ptr<SThreadMetaData> pThreadImpl = 0;
	{
		AUTO_LOCK(m_spawnedThreadsLock);

		SpawnedThreadMapIter res = m_spawnedThreads.find(pThreadTask);
		if (res == m_spawnedThreads.end())
		{
			// Thread has already finished and unregistered itself.
			// As it is complete we cannot wait for it.
			// Hence return true.
			return true;
		}

		pThreadImpl = res->second; // Keep object alive
	}

	// On try join, exit if the thread is not in a state to exit
	if (eJoinMode == eJM_TryJoin && pThreadImpl->m_isRunning)
	{
		return false;
	}

	// Wait for completion of the target thread exit condition
	AUTO_LOCK_T(decltype(pThreadImpl->m_threadExitMutex.m_mutex), pThreadImpl->m_threadExitMutex.m_mutex);
	#if 1
	while (pThreadImpl->m_isRunning)
	{
#if !BB_PLATFORM_ORBIS
		// Ensure thread is still alive.
		// Handle special case where engine shutdown is using exit(1) e.g. CrashHandler.
		// Exit(1) force terminates all threads so they don't reach the cleanup code at the end of the RunThread() function.		
		// 1) Thread must be running as we hold the m_threadExitMutex and pThreadImpl->m_isRunning == true. 
		// 2) If pThreadImpl->m_isRunning == false we would not be in this loop. Hence there is no double call of UnregisterThread()
		if (!CryThreadUtil::CryIsThreadAlive(pThreadImpl->m_threadHandle))
		{
			pThreadImpl->m_pThreadMngr->UnregisterThread(pThreadImpl->m_pThreadTask);
			break;
		}
#endif
		std::unique_lock<std::mutex> lk(pThreadImpl->m_threadExitMutex.m_mutex);
		pThreadImpl->m_threadExitCondition.wait(lk);
	}
	#endif

	return true;
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


