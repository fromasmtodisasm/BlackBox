#pragma once
#include <BlackBox/Core/MathHelper.hpp>
#include <BlackBox/Renderer/IRender.hpp>

class MainWindow : public IRenderCallback, public IRendererCallbackClient
{
public:
	MainWindow();
	void Draw();
	bool Update();

	int m_ViewRenderTarget = -1;
	IGame* m_pGame = nullptr;
	Legacy::Vec2 m_NextFrameViewPortSize = Legacy::Vec2(1);

	// Inherited via IRenderCallback
	virtual void CallBack(Type type) override;
	virtual void OnRenderer_BeforeEndFrame() override;
};