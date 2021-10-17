#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/Core/IGame.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/Camera.hpp>
#include <BlackBox/System/ILog.hpp>

#include "MainWindow.hpp"
#include "imgui.h"


MainWindow::MainWindow()
{
	m_pGame = gEnv->pSystem->GetIGame();
	gEnv->pRenderer->SetRenderCallback(this);
	m_ViewRenderTarget = gEnv->pRenderer->CreateRenderTarget();
	gEnv->pRenderer->SetRenderTarget(m_ViewRenderTarget);
}

void MainWindow::Draw()
{

}
bool MainWindow::Update()
{
	auto& vp = m_NextFrameViewPortSize;
	//auto c	 = gEnv->pSystem->GetViewCamera().Ratio = vp.x / vp.y;
	//gEnv->pRenderer->SetViewport(0, 0, static_cast<int>(vp.x), static_cast<int>(vp.y));

	return m_pGame->Update();
}

void MainWindow::CallBack(Type type)
{
	switch (type)
	{
	case IRenderCallback::eOnRender:
	{
		auto s = m_NextFrameViewPortSize;
		gEnv->pRenderer->SetViewport(0, 0, static_cast<int>(s.x), static_cast<int>(s.y));
		break;
	}
	case IRenderCallback::eBeforeSwapBuffers:
	{
		//auto size = ImGui::GetContentRegionAvail();
		//ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
		static bool p_open = true;
		ImGui::Begin("View",&p_open
			#if 0
			,ImGuiWindowFlags_NoNav
			| ImGuiWindowFlags_NoInputs
			#endif
			);
#pragma warning(push)
#pragma warning(disable: 4312)
			auto size = ImGui::GetContentRegionAvail();
			auto r	   = gEnv->pRenderer;
			auto x	  = (float)r->GetWidth() / m_ViewRenderTarget;
			auto y	   = (float)r->GetHeight();
			ImGui::Image(reinterpret_cast<ImTextureID>(m_ViewRenderTarget), size, ImVec2(0, 1), ImVec2(1, 0));
			if (ImGui::IsItemClicked())
			{
				gEnv->pLog->Log("View clicked");
			}
			auto pos = ImGui::GetCursorPos();
			m_NextFrameViewPortSize = Vec2(size.x, size.y);
#pragma warning(pop)
			//ImGui::EndTabItem();
		ImGui::End();
		break;
	}
	default:
		break;
	}
}

extern "C" int AppMain(int c, char** v)
{
    MainWindow mainWindow;
    while (mainWindow.Update())
			;
	return 0;
}
