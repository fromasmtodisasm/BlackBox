#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/Core/IGame.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/Camera.hpp>
#include <BlackBox/System/ILog.hpp>

#include "MainWindow.hpp"

#include <imgui.h>


MainWindow::MainWindow()
{
	//gEnv->pRenderer->SetRenderCallback(this);
	m_ViewRenderTarget = gEnv->pRenderer->CreateRenderTarget();
	gEnv->pRenderer->SetRenderTarget(m_ViewRenderTarget);
}

bool MainWindow::Update()
{
	auto& vp = m_NextFrameViewPortSize;
	//auto c	 = gEnv->pSystem->GetViewCamera().Ratio = vp.x / vp.y;
	//gEnv->pRenderer->SetViewport(0, 0, static_cast<int>(vp.x), static_cast<int>(vp.y));
	return true;
}

void MainWindow::Draw()
{
	//auto size = ImGui::GetContentRegionAvail();
	//ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
	static bool p_open = true;
	ImGui::Begin("MainView", &p_open, ImGuiWindowFlags_NoBackground
#if 0
			,ImGuiWindowFlags_NoNav
			| ImGuiWindowFlags_NoInputs
#endif
	);
#pragma warning(push)
#pragma warning(disable : 4312)
	auto pos = ImGui::GetWindowPos();
	pos.x += ImGui::GetCursorPosX();
	pos.y -= ImGui::GetCursorPosY();
	auto size = ImGui::GetContentRegionAvail();
	gEnv->pRenderer->SetViewport((int)m_NextFrameViewPortPos.x, (int)m_NextFrameViewPortPos.y, (int)size.x, (int)size.y);
	auto r	  = gEnv->pRenderer;
	auto x	  = (float)r->GetWidth() / m_ViewRenderTarget;
	auto y	  = (float)r->GetHeight();
	//ImGui::Image(reinterpret_cast<ImTextureID>(m_ViewRenderTarget), size, ImVec2(0, 1), ImVec2(1, 0));
	if (ImGui::IsItemClicked())
	{
		gEnv->pLog->Log("View clicked");
	}
	//auto pos				= ImGui::GetCursorPos();
	m_NextFrameViewPortSize = Legacy::Vec2(size.x, size.y);
	m_NextFrameViewPortPos = Legacy::Vec2(pos.x, pos.y);
#pragma warning(pop)
	//ImGui::EndTabItem();
	ImGui::End();
}
