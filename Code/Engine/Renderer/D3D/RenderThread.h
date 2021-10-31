#pragma once

#include <BlackBox\Threading\IThreadManager.h>

class CRenderThread : public IThread
{
public:
	std::condition_variable m_started;
	virtual void ThreadEntry();
};


struct SRenderThread
{
	std::deque<std::function<void()>> Commands;
	CRenderThread*					  m_pThread;
	std::mutex						  m_QueueMutex;

	SRenderThread() {
		m_pThread = new CRenderThread();
	}

	void Start();
	void Quit();
	void Process();
	void ProcessCommands();

	//! Adds a callback to be executed on the RenderThread
	//! If Render thread doesn't exist it will be executed on the Main thread
	//! @see ERenderCommandFlags
	template<typename RenderThreadCallback>
	void ExecuteRenderThreadCommand(RenderThreadCallback&& callback)
	{
		#define MULTITHREADED_RENDER
		#ifdef MULTITHREADED_RENDER
		std::lock_guard<std::mutex> lock(m_QueueMutex);
		Commands.push_back(callback);
		#else
		callback();
		#endif
	}

	std::atomic<bool> m_bQuit = false;
	threadID		  m_nRenderThread;


	std::mutex m_StartedMutex;

};