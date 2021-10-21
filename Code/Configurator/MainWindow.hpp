#pragma once
#include <BlackBox/Renderer/IRender.hpp>

class MainWindow
{
  public:
	MainWindow();
	void Draw();
	bool Update();
	void LeftCollumn();
	void RightColumn();


	int m_ViewRenderTarget		 = -1;
	IGame* m_pGame				 = nullptr;
	Vec2 m_NextFrameViewPortSize = Vec2(1);


	bool is_done = false;
};
