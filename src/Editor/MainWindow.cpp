#include <BlackBox/Core/Platform/platform_impl.inl>
#include <BlackBox/Core/IGame.hpp>

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
	return m_pGame->Update();
}

void MainWindow::CallBack(Type type)
{
	switch (type)
	{
	case IRenderCallback::eBeforeSwapBuffers:
	{
		//auto size = ImGui::GetContentRegionAvail();
		//ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
		ImGui::Begin("View",(bool*)true);
#pragma warning(push)
#pragma warning(disable: 4312)
			ImGui::Image(reinterpret_cast<ImTextureID>(m_ViewRenderTarget), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));
			auto pos = ImGui::GetCursorPos();
			auto size = ImGui::GetContentRegionAvail();
			gEnv->pRenderer->SetViewport(
				0, 0, gEnv->pRenderer->GetWidth(), gEnv->pRenderer->GetHeight()
			);
#pragma warning(pop)
			//ImGui::EndTabItem();
		ImGui::End();
		break;
	}
	default:
		break;
	}
}
