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
void CSystem::OnRenderer_BeforeEndFrame()
{
	RenderStats();
}

void CSystem::RenderStats()
{
	if (m_pFont)
	{
		float px = 20;
		float py = 20;
		auto  dy = 30;

		PrintRightAlignedText(py, "CamPos = 0 0 0 Angl = 0 0 0");
		py += dy;
		PrintRightAlignedText(py, "Ver = 1.0.0.0");
		py += dy;
		PrintRightAlignedText(py, "Polygons 0,000");
		py += dy;
		PrintRightAlignedText(py, "...");
		py += dy;
		PrintRightAlignedText(py, "FPS 60 ( 60.. 50) / 60");
		py += dy;
		PrintRightAlignedText(py, "ViewDist = 1024/0.0");
		py += dy;
		PrintRightAlignedText(py, "Render path = ...");
		py += dy;

		dy = 15;
		if (sys_dump_memstats)
		{

			auto PrintMemoryUsage = [&,this](const char* name, typename auto fn, auto This) {
				py += dy;
				PrintMemoryUsageForName(name, fn, This, px, py);
			};

			PrintMemoryUsage("Game", &IGame::GetMemoryUsage, m_pGame);
			PrintMemoryUsage("System", &CSystem::GetMemoryStatistics, this);
			PrintMemoryUsage("Renderer", &IRenderer::GetMemoryUsage, m_env.pRenderer);
			PrintMemoryUsage("ScriptSsystem", &IScriptSystem::GetMemoryUsage, m_env.pScriptSystem);
			#if 0
			static char stats[256];
			{
				auto		len = sprintf(stats, "Frame rate: %f\n                        Frame time: %f", m_env.pTimer->GetFrameRate(), m_env.pTimer->GetRealFrameTime());
				stats[len]		= 0;
				m_env.pRenderer->Draw2dText(100, (float)py, stats, dti);
			}
			#endif
		}

#if ENABLE_DEBUG_GUI
		if (m_GuiManager)
			m_GuiManager->Render();
#endif
		//if (m_bIsActive)
		//m_pWindow->swap();
	}

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
		m_env.pRenderer->Update();	
	}
}

