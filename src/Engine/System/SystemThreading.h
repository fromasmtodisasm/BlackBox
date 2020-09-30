#pragma once

#include <BlackBox/Threading/IThreadManager.h>

#include <thread>
#include <mutex>

class CThreadManager;

//////////////////////////////////////////////////////////////////////////
struct SThreadMetaData //: public CMultiThreadRefCount
{
	using CryMutex = std::mutex;
	using CryConditionVariable = std::condition_variable;
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
	threadID                                m_threadId;     // The active threadId, 0 = Invalid Id

	CryMutex                                m_threadExitMutex;     // Mutex used to safeguard thread exit condition signaling
	CryConditionVariable                    m_threadExitCondition; // Signaled when the thread is about to exit

	string									m_threadName; // Thread name
	volatile bool                           m_isRunning;  // Indicates the thread is not ready to exit yet
};

class CThreadManager : public IThreadManager
{
  public:
	// Inherited via IThreadManager
	virtual IThreadConfigManager* GetThreadConfigManager() override;
	virtual bool SpawnThread(IThread* pThread, const char* sThreadName, ...) override;
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

private:
	// Note: Guard SThreadMetaData with a _smart_ptr and lock to ensure that a thread waiting to be signaled by another still
	// has access to valid SThreadMetaData even though the other thread terminated and as a result unregistered itself from the CThreadManager.
	// An example would be the join method. Where one thread waits on a signal from an other thread to terminate and release its SThreadMetaData,
	// sharing the same SThreadMetaData condition variable.
	typedef std::map<IThread*, _smart_ptr<SThreadMetaData>>                                                SpawnedThreadMap;
	typedef std::map<IThread*, _smart_ptr<SThreadMetaData>>::iterator                                      SpawnedThreadMapIter;
	typedef std::map<IThread*, _smart_ptr<SThreadMetaData>>::const_iterator                                SpawnedThreadMapConstIter;
	typedef std::pair<IThread*, _smart_ptr<SThreadMetaData>>                                               ThreadMapPair;
};