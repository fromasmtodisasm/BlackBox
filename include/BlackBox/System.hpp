#pragma once

#include <BlackBox/ISystem.hpp>
#include <BlackBox/Window.hpp>
#include <BlackBox/IConsole.hpp>
#include <BlackBox/IScriptSystem.hpp>

class CConsole;
class CRender;

class CSystem : public ISystem, public IInputEventListener, public IConsoleVarSink
{
public:
	CSystem();
	~CSystem();
  
	// Inherited via ISystem
  virtual bool Init() override;
  virtual void Start() override;
	virtual void Update() override;
	virtual void BeginFrame() override;
	virtual void EndFrame() override;
  virtual void Release() override;

  virtual IShaderManager*		GetShaderManager() override;
  virtual IRender*					GetIRender() override;
  virtual ILog*							GetILog() override;
  virtual IConsole*					GetIConsole() override;
  virtual IGame*						GetIGame() override;
	virtual IFont*						GetIFont() override;
	virtual IWindow*					GetIWindow() override;
	virtual IInputHandler*		GetIInputHandler() override;
	virtual IScriptSystem*		GetIScriptSystem() override;

  virtual IGame*						CreateGame(IGame* game) override;

	virtual void ShowMessage(const char* message, const char* caption, MessageType messageType) override;
	virtual void Log(const char* message) override;

	virtual bool OnInputEvent(sf::Event& event) override;
	// Inherited via IConsoleVarSink
	virtual bool OnBeforeVarChange(ICVar* pVar, const char* sNewValue) override;

	bool ConfigLoad(const char* file);
private:
  ILog *m_pLog;
  CConsole *m_pConsole;
  IGame *m_pGame;
	IFont* m_pFont;
	IWindow* m_pWindow;
	IInputHandler* m_InputHandler;
	IRender* m_Render;
	IScriptSystem* m_pScriptSystem;

private:
	ICVar* r_window_width;
	ICVar* r_window_height;
	ICVar* r_bpp;
	ICVar* r_zbpp;
	ICVar* r_sbpp;
	ICVar* r_fullscreen;

	// Inherited via ISystem
	virtual bool IsDevMode() override;

	// Inherited via ISystem
	virtual void Error(const char* message) override;
};
