#include "RenderThread.h"
#include "BaseRenderer.hpp"

void CRenderThread::ThreadEntry()
{
	threadID renderThreadId			= GetCurrentThreadId();
	gRenDev->m_RenderThread->m_nRenderThread = renderThreadId;
	//CNameTableR::m_nRenderThread	= renderThreadId;
	//gEnv->pCryPak->SetRenderThreadId(renderThreadId);
	//m_started.notify_all();
	gRenDev->m_RenderThread->Process();
}

void SRenderThread::Start()
{
	gEnv->pThreadManager->SpawnThread(m_pThread, "RenderThread");

	auto lock = std::unique_lock<std::mutex>(m_StartedMutex);
	//m_pThread->m_started.wait(lock);
}

void SRenderThread::Quit()
{
	m_bQuit.store(true);
	gEnv->pThreadManager->JoinThread(m_pThread, EJoinMode::eJM_Join);
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
