#pragma once

#include <BlackBox/ISystem.hpp>
#include <BlackBox/Window.hpp>
#include <BlackBox/IConsole.hpp>
#include <BlackBox/IInput.hpp>
#include <BlackBox/IScriptSystem.hpp>
#include <BlackBox/Common/CmdLine.hpp>
#include <BlackBox/Timer.hpp>

class CConsole;
struct IWindow;

class CScriptObjectConsole;
class CScriptObjectScript;
class CScriptObjectRenderer;

class CSystem :
  public ISystem,
  public IInputEventListener,
  public IConsoleVarSink,
  public ISystemEventListener
{
public:
  CSystem(SSystemInitParams& initParams);
  ~CSystem();

  // Inherited via ISystem
  virtual bool Init() override;
  virtual void Start() override;
  virtual bool Update(int updateFlags = 0, int nPauseMode = 0) override;
  virtual void RenderBegin() override;
  virtual void Render() override;
  virtual void RenderEnd() override;
  virtual void Release() override;

  virtual IShaderManager* GetShaderManager() override;
  virtual IRenderer* GetIRender() override;
  virtual ILog* GetILog() override;
  virtual ITimer* GetITimer() override;
  virtual IConsole* GetIConsole() override;
  virtual IInput* GetIInput() override;
  virtual IGame* GetIGame() override;
  virtual IFont* GetIFont() override;
  virtual IWindow* GetIWindow() override;
  virtual IWorld* GetIWorld() override;
  virtual IScriptSystem* GetIScriptSystem() override;
  virtual ISystemEventDispatcher* GetISystemEventDispatcher() override { return m_pSystemEventDispatcher; }

  virtual IGame* CreateGame(IGame* game) override;

  virtual void Quit() override;

  virtual void ShowMessage(const char* message, const char* caption, MessageType messageType) override;
  virtual void Log(const char* message) override;

  virtual bool OnInputEvent(const SInputEvent& event) override;

  bool ConfigLoad(const char* file);

  virtual bool IsDevMode() override;
  virtual void Error(const char* message) override;
  virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override;

  // Inherited via IConsoleVarSink
  virtual bool OnBeforeVarChange(ICVar* pVar, const char* sNewValue) override;

private:
  bool InitResourceManagers();
  void ParseCMD();
  void LoadScreen();
  bool InitScripts();
private:
  CTimer									m_Time;                  //!<

  ILog* m_pLog;
  CConsole* m_pConsole;
  IInput* m_pInput;
  IGame* m_pGame;
  IFont* m_pFont;
  IWindow* m_pWindow;
  IWorld* m_pWorld;
  IRenderer* m_Render;
  IScriptSystem* m_pScriptSystem;
  INetwork* m_pNetwork;
  //! system event dispatcher
  ISystemEventDispatcher* m_pSystemEventDispatcher;

  CScriptObjectConsole* m_ScriptObjectConsole;
  CScriptObjectScript* m_ScriptObjectScript;
  CScriptObjectRenderer* m_ScriptObjectRenderer;

private:
  ICVar* r_window_width;
  ICVar* r_window_height;
  ICVar* r_bpp;
  ICVar* r_zbpp;
  ICVar* r_sbpp;
  ICVar* r_fullscreen;

  SSystemInitParams& m_startupParams;
  CCmdLine* m_pCmdLine;

  uint64_t NOW;
  uint64_t LAST;
  double m_DeltaTime = 0.0;

  // Inherited via ISystem
  virtual float GetDeltaTime() override;

  // ������������ ����� ISystem
  virtual INetwork* GetINetwork() override;
};