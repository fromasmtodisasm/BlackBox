#pragma once

#include <BlackBox/IEngine.hpp>
#include <BlackBox/CWindow.hpp>
class CConsole;
class CRender;

class CEngine : public IEngine
{
private:
  ILog *m_pLog;
  CConsole *m_pConsole;
  IGame *m_pGame;
	IFont* m_pFont;
	IWindow* m_pWindow;
	IInputHandler* m_InputHandler;
	IRender* m_Render;
public:
  // Унаследовано через ISystem
  virtual bool Init() override;
  virtual void Start() override;
  virtual void Release() override;
  virtual IShaderManager * getShaderManager() override;
  virtual IRender * getIRender() override;

  // Унаследовано через IEngine
  virtual ILog* getILog() override;

  // Унаследовано через IEngine
  virtual IConsole* getIConsole() override;

  // Унаследовано через IEngine
  virtual IGame* getIGame() override;

  // Унаследовано через IEngine
  virtual IGame* CreateGame(IGame* game) override;

	// Унаследовано через IEngine
	virtual IFont* getIFont() override;

	// Унаследовано через IEngine
	virtual IWindow* getIWindow() override;

	// Унаследовано через IEngine
	virtual IInputHandler* getIInputHandler() override;
};
