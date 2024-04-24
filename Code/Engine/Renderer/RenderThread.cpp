#include "pch.hpp"
#include "RenderThread.h"
#include "BaseRenderer.hpp"

void CRenderThread::ThreadEntry()
{
#ifdef MULTITHREADED_RENDER
	threadID renderThreadId                  = GetCurrentThreadId();
	gRenDev->m_RenderThread->m_nRenderThread = renderThreadId;
	//CNameTableR::m_nRenderThread	= renderThreadId;
	//Env::CryPak()->SetRenderThreadId(renderThreadId);
	//m_started.notify_all();
	gRenDev->m_RenderThread->Process();
#endif
}

void SRenderThread::Start()
{
#ifdef MULTITHREADED_RENDER
	#if 0
	gEnv->pThreadManager->SpawnThread(m_pThread, "RenderThread");
	#else
	m_pThread.rt  = new CRenderThread;
	m_pThread.std = std::thread(&CRenderThread::ThreadEntry, m_pThread.rt);
	#endif

	auto lock = std::unique_lock<std::mutex>(m_StartedMutex);
	//m_pThread->m_started.wait(lock);
#endif
}

void SRenderThread::Quit()
{
#ifdef MULTITHREADED_RENDER
	m_bQuit.store(true);
	//gEnv->pThreadManager->JoinThread(m_pThread, EJoinMode::eJM_Join);
	m_pThread.std.join();
#endif
}

void SRenderThread::Process()
{
	do
	{
		ProcessCommands();
	} while (!m_bQuit.load());
}

void SRenderThread::ProcessCommands()
{
	if (Commands.empty())
	{
		Sleep(1);
	}
	else
	{
		auto cmd = Commands.front();
		cmd();
		Commands.pop_front();
	}
}
