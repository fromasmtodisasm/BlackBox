#pragma once

#include <BlackBox/IEngine.hpp>
#include <BlackBox/CWindow.hpp>
#include <BlackBox/IConsole.hpp>

#define SAFE_RELEASE(o) if (o) o->Release();

class CConsole;
class CRender;

class CEngine : public ISystem, public IInputEventListener, public IConsoleVarSink
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
	CEngine();
	~CEngine();
  
	// Inherited via ISystem
  virtual bool Init() override;
  virtual void Start() override;
	virtual void Update() override;
	virtual void BeginFrame() override;
	virtual void EndFrame() override;
  virtual void Release() override;

  virtual IShaderManager * getShaderManager() override;
  virtual IRender * getIRender() override;
  virtual ILog* getILog() override;
  virtual IConsole* getIConsole() override;
  virtual IGame* getIGame() override;
  virtual IGame* CreateGame(IGame* game) override;
	virtual IFont* getIFont() override;
	virtual IWindow* getIWindow() override;
	virtual IInputHandler* getIInputHandler() override;

	virtual bool OnInputEvent(sf::Event& event) override;
	// Inherited via IConsoleVarSink
	virtual bool OnBeforeVarChange(ICVar* pVar, const char* sNewValue) override;

	bool ConfigLoad(const char* file);
private:
	ICVar* r_window_width;
	ICVar* r_window_height;
	ICVar* r_bpp;
	ICVar* r_zbpp;
	ICVar* r_sbpp;





};
