#pragma once

#include <BlackBox/System/ConsoleRegistration.h>
#include <BlackBox/Threading/IThreadManager.h>
#include <BlackBox/Utils/smartptr.hpp>

#include <thread>
#include <mutex>

class CThreadManager;

//struct CryCriticalSection

using CryCriticalSection = CryMutex;
using CryConditionVariable = std::condition_variable;

//////////////////////////////////////////////////////////////////////////
struct SThreadMetaData : public _reference_target_MT
{
	SThreadMetaData()
		: m_pThreadTask(0)
		, m_pThreadMngr(nullptr)
		//, m_threadHandle(0)
		, m_threadId(0)
		, m_threadName("Cry_UnnamedThread")
		, m_isRunning(false)
	{
	}

	IThread*                                m_pThreadTask; // Pointer to thread task to be executed
	CThreadManager*                         m_pThreadMngr; // Pointer to thread manager

	//CryThreadUtil::TThreadHandle            m_threadHandle; // Thread handle
	std::thread								m_threadHandle;
	threadID                                m_threadId;     // The active threadId, 0 = Invalid Id

	CryMutex                                m_threadExitMutex;     // Mutex used to safeguard thread exit condition signaling
	CryConditionVariable                    m_threadExitCondition; // Signaled when the thread is about to exit

	string									m_threadName; // Thread name
	volatile bool                           m_isRunning;  // Indicates the thread is not ready to exit yet

	void Dump()
	{
		CryLogAlways("$3Name: %s    Id: %d", m_threadName.data(), &m_threadId);
	}
};

void DumpThreads(IConsoleCmdArgs* args);

class CThreadManager : public IThreadManager
{
  public:
	CThreadManager()
	{
		REGISTER_COMMAND("dump_threads", DumpThreads, VF_NULL, "dump threads");
	}
	// Inherited via IThreadManager
	virtual IThreadConfigManager* GetThreadConfigManager() override;
	virtual bool SpawnThread(IThread* pThread, const char* sThreadName, ...) override;

	bool     UnregisterThread(IThread* pThreadTask);
	bool     SpawnThreadImpl(IThread* pThread, const char* sThreadName);

	virtual bool JoinThread(IThread* pThreadTask, EJoinMode joinStatus) override;
	virtual bool RegisterThirdPartyThread(void* pThreadHandle, const char* sThreadName, ...) override;
	virtual bool UnRegisterThirdPartyThread(const char* sThreadName, ...) override;
	virtual const char* GetThreadName(threadID nThreadId) override;
	virtual threadID GetThreadId(const char* sThreadName, ...) override;
	virtual void ForEachOtherThread(IThreadManager::ThreadModifFunction fpThreadModiFunction, void* pFuncData = 0) override;
	virtual void EnableFloatExceptions(EFPE_Severity eFPESeverity, threadID nThreadId = 0) override;
	virtual void EnableFloatExceptionsForEachOtherThread(EFPE_Severity eFPESeverity) override;
	virtual uint GetFloatingPointExceptionMask() override;
	virtual void SetFloatingPointExceptionMask(uint nMask) override;

	void Dump()
	{
		for (auto &thread : m_spawnedThreads)
		{
			thread.second->Dump();
		}
	}

private:
	// Note: Guard SThreadMetaData with a _smart_ptr and lock to ensure that a thread waiting to be signaled by another still
	// has access to valid SThreadMetaData even though the other thread terminated and as a result unregistered itself from the CThreadManager.
	// An example would be the join method. Where one thread waits on a signal from an other thread to terminate and release its SThreadMetaData,
	// sharing the same SThreadMetaData condition variable.
	typedef std::map<IThread*, _smart_ptr<SThreadMetaData>>                                                SpawnedThreadMap;
	typedef std::map<IThread*, _smart_ptr<SThreadMetaData>>::iterator                                      SpawnedThreadMapIter;
	typedef std::map<IThread*, _smart_ptr<SThreadMetaData>>::const_iterator                                SpawnedThreadMapConstIter;
	typedef std::pair<IThread*, _smart_ptr<SThreadMetaData>>                                               ThreadMapPair;

	CryCriticalSection         m_spawnedThreadsLock; // Use lock for the rare occasion a thread is created/destroyed
	SpawnedThreadMap           m_spawnedThreads;     // Holds information of all spawned threads (through this system)

	CryCriticalSection         m_spawnedThirdPartyThreadsLock; // Use lock for the rare occasion a thread is created/destroyed
	//SpawnedThirdPartyThreadMap m_spawnedThirdPartyThread;      // Holds information of all registered 3rd party threads (through this system)

	//CThreadConfigManager       m_threadConfigManager;
};

#define AUTO_LOCK_T(Type, lock) std::lock_guard<Type> __AutoLock(lock)
#define AUTO_LOCK(lock) AUTO_LOCK_T(std::mutex, lock.m_mutex)