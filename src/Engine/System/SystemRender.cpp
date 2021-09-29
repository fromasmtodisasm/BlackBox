#include <BlackBox/3DEngine/3DEngine.hpp>
#include <BlackBox/Profiler/Profiler.h>
#include <BlackBox/System/System.hpp>

#include "CrySizerImpl.h"

void CSystem::Render()
{
	if (m_env.pRenderer)
	{
		PROFILER_PUSH_CPU_MARKER("CPU RENDER", Utils::COLOR_YELLOW);
		{
			m_env.pRenderer->SetState(IRenderer::State::DEPTH_TEST, true);
			m_env.p3DEngine->SetCamera(GetViewCamera());
			m_env.p3DEngine->Draw();
		}
		PROFILER_POP_CPU_MARKER();
	}
}

void CSystem::RenderBegin()
{
	if (!m_env.pRenderer)
		return;
	PROFILER_SYNC_FRAME();
	PROFILER_PUSH_CPU_MARKER("Full frame", COLOR_GRAY);
	m_env.pRenderer->SetState(IRenderer::State::DEPTH_TEST, true);
	m_env.pRenderer->BeginFrame();
#if ENABLE_DEBUG_GUI
	if (m_GuiManager)
	{
		m_GuiManager->NewFrame();
		m_GuiManager->ShowDemoWindow();
	}
	//m_GuiManager.ShowNodeEditor();
#endif
}

void CSystem::RenderEnd()
{
	if (!m_env.pRenderer)
		return;
	PROFILER_POP_CPU_MARKER();
	{
		//DEBUG_GROUP("DRAW_PROFILE");
		PROFILER_DRAW();
	}
	if (m_env.pRenderer)
	{
		if (sys_dump_memstats)
		{
			auto py = 100;
			const auto dy = 15;

			SDrawTextInfo dti;
			dti.font = m_pFont;
			auto& color = dti.color;
			color[0]	= 1.0; //green
			color[1]	= 1.0;
			color[2]	= 1.0; //alpha
			color[3]	= 0.0; //red
			static char stats[256];
			{
				CrySizerImpl sizer;
				SIZER_COMPONENT_NAME(&sizer, "Game");
				m_pGame->GetMemoryStatistics(&sizer);
				auto		len = sprintf(stats, "Game memory usage: %d", sizer.GetTotalSize());
				stats[len]		= 0;
				m_env.pRenderer->Draw2dText(100, (float)py, stats, dti);
			}
			py += dy;
			{
				CrySizerImpl sizer;
				SIZER_COMPONENT_NAME(&sizer, "System");
				sizer.AddObject(this, sizeof(*this));
				//sizer.

				auto		len = sprintf(stats, "System memory usage: %d", sizer.GetTotalSize());
				stats[len]		= 0;
				m_env.pRenderer->Draw2dText(100, (float)py, stats, dti);
			}
		}

		m_env.pRenderer->Update();

#if ENABLE_DEBUG_GUI
		if (m_GuiManager)
			m_GuiManager->Render();
#endif
		//if (m_bIsActive)
		//m_pWindow->swap();
	}
}
