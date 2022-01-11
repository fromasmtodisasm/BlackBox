#pragma once
#include <BlackBox/Core/MathHelper.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <IEditor.h>

//#include <QMainWindow>

class MainWindow : public IEditorNotifyListener
{
  public:
	MainWindow();
	bool Update();

	int			 m_ViewRenderTarget		 = -1;
	Legacy::Vec2 m_NextFrameViewPortSize = Legacy::Vec2(32);
	Legacy::Vec2 m_NextFrameViewPortPos	 = Legacy::Vec2(0);

	// Inherited via IGuiElement
  private:
	virtual void Draw();

	// Inherited via IEditorNotifyListener
	virtual void OnEditorNotifyEvent(EEditorNotifyEvent event) override;
};