#pragma once

#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/Core/Platform/Windows.hpp>
#include <BlackBox/Core/Stream.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Input/IInput.hpp>
#include <BlackBox/ScriptSystem/IScriptSystem.hpp>
#include <BlackBox/EntitySystem/IEntitySystem.hpp>
#include <BlackBox/System/CmdLine.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/Timer.hpp>
#include <BlackBox/System/Window.hpp>
#include <BlackBox/GUI/ImGuiManager.hpp>
#include <BlackBox/System/IWindowMessageHandler.h>

#include <BlackBox/ScriptSystem/ScriptObjectConsole.hpp>
#include <BlackBox/ScriptSystem/ScriptObjectRenderer.hpp>
#include <BlackBox/ScriptSystem/ScriptObjectScript.hpp>

#define DEFAULT_APP_NAME "BlackBox"

struct IWindow;

class CScriptObjectConsole;
class CScriptObjectScript;
class CScriptObjectRenderer;

class CSystem :
  public ISystem,
  public IInputEventListener,
  public IConsoleVarSink,
  public ISystemEventListener,
  public IWindowMessageHandler,
  public ILoadConfigurationEntrySink
{
public:
  CSystem(SSystemInitParams& initParams);
  ~CSystem();
  void PreprocessCommandLine();
  void ProcessCommandLine();

  // Inherited via ISystem
  virtual bool Init() override;
  bool CreateLog();
  virtual void Start() override;
  virtual bool Update(int updateFlags = 0, int nPauseMode = 0) override;
  virtual void RenderBegin() override;
  virtual void Render() override;
  virtual void RenderEnd() override;
  virtual void Release() override;
	virtual SSystemGlobalEnvironment* GetGlobalEnvironment() override { return &m_env; }

  virtual IRenderer* GetIRenderer() override;
  virtual ILog* GetILog() override;
  virtual ICmdLine* GetICmdLine() override { return m_pCmdLine; };
  virtual ITimer* GetITimer() override;
  virtual IConsole* GetIConsole() override;
  virtual IInput* GetIInput() override;
  virtual IGame* GetIGame() override;
  virtual IFont* GetIFont() override;
  virtual IWindow* GetIWindow() override;
  virtual IScriptSystem* GetIScriptSystem() override;
  virtual ISystemEventDispatcher* GetISystemEventDispatcher() override { return m_pSystemEventDispatcher; }
  virtual INetwork* GetINetwork() override;
  virtual ICryPak* GetIPak() override;
  virtual IHardwareMouse* GetIHardwareMouse() override { return m_env.pHardwareMouse; };
  virtual IEntitySystem* GetIEntitySystem() override;

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

  virtual float GetDeltaTime() override;


  virtual const SFileVersion& GetFileVersion() override;
  virtual const SFileVersion& GetProductVersion() override;


// Унаследовано через ISystem
  
	void      SetViewCamera(CCamera& Camera) override { m_ViewCamera = Camera; }
	CCamera&  GetViewCamera() override                { return m_ViewCamera; }
private:
  bool CreateConsole();
  bool InitConsole();
  bool InitRender();
  bool InitInput();
  bool InitScriptSystem();
  bool InitEntitySystem();
  bool InitNetwork();
  bool InitGUI();
  bool Init3DEngine();
  bool InitSubSystem();
  bool OpenRenderLibrary(std::string_view render);

  void ParseCMD();
  void LoadScreen();
  bool InitScripts();
  bool InitFileSystem(/*const IGameStartup* pGameStartup*/);
  void SetWorkingDirectory(const std::string& path) const;
  void LogCommandLine() const;
  void Tests();
  void PollEvents();


	//////////////////////////////////////////////////////////////////////////
	// Helper functions.
	//////////////////////////////////////////////////////////////////////////
	void        CreateRendererVars(const SSystemInitParams& startupParams);
	void        CreateSystemVars();
protected:
	CCmdLine*                                 m_pCmdLine;

private:
	// System environment.
#if defined(SYS_ENV_AS_STRUCT)
	//since gEnv is a global var, this should just be a reference for code consistency
	SSystemGlobalEnvironment & m_env;
#else
	SSystemGlobalEnvironment m_env;
#endif
  CTimer									m_Time;                  //!<
	CCamera									m_ViewCamera;            //!<

  ILog* m_pLog;
  IConsole*								m_pConsole;
  //IInput* m_pInput;
  ICryPak*								m_pCryPak;
  IGame*									m_pGame;
  IFont*									m_pFont;
  IWindow*								m_pWindow;
  IRenderer*							m_Render;
  IScriptSystem*					m_pScriptSystem;
  IEntitySystem*					m_pEntitySystem;
  INetwork*								m_pNetwork;
  //! system event dispatcher
  ISystemEventDispatcher* m_pSystemEventDispatcher;

  CScriptObjectConsole*		m_ScriptObjectConsole;
  CScriptObjectScript*		m_ScriptObjectScript;
  CScriptObjectRenderer*	m_ScriptObjectRenderer;

private:
  ICVar* cvGameName;
	//////////////////////////////////////////////////////////////////////////
	//! User define callback for system events.
	ISystemUserCallback* m_pUserCallback;


  SSystemInitParams& m_startupParams;
  SFileVersion m_FileVersion;
  SFileVersion m_ProductVersion;

  uint64_t NOW;
  uint64_t LAST;
  double m_DeltaTime = 0.0;

#if ENABLE_DEBUG_GUI
  IImGuiManager* m_GuiManager = nullptr;
#endif

	//! to hold the values stored in system.cfg
	//! because editor uses it's own values,
	//! and then saves them to file, overwriting the user's resolution.
	int m_iHeight = 0;
	int m_iWidth = 0;
	int m_iColorBits = 0;

	// System console variables.
	//////////////////////////////////////////////////////////////////////////
	float m_rIntialWindowSizeRatio;
	int m_rWidth;
	int m_rHeight;
	int m_rColorBits;
	int m_rDepthBits;
	int m_rStencilBits;
	int m_rFullscreen;
	//ICVar* m_rFullsceenNativeRes;
	//ICVar* m_rWindowState;
	//ICVar* m_rDriver;
	int m_rDisplayInfo;
  int m_rDebug;
  int m_rTonemap;

  bool m_bIsActive = true;



// Inherited via ISystem
  virtual void EnableGui(bool enable) override;

  // Inherited via ISystem
  virtual void SaveConfiguration() override;
  virtual void LoadConfiguration(const string& sFilename) override;

  // Inherited via ILoadConfigurationEntrySink
  virtual void OnLoadConfigurationEntry(const char* szKey, const char* szValue, const char* szGroup) override;

  // Inherited via ISystem
  virtual void Relaunch(bool bRelaunch) override;
  virtual bool IsQuitting() override;
  virtual void Error(const char* sFormat, ...) override;
  virtual void Warning(EValidatorModule module, EValidatorSeverity severity, int flags, const char* file, const char* format, ...) override;
  virtual bool CheckLogVerbosity(int verbosity) override;

  // Inherited via ISystem
  virtual bool WriteCompressedFile(char* filename, void* data, unsigned int bitlen) override;
  virtual unsigned int ReadCompressedFile(char* filename, void* data, unsigned int maxbitlen) override;
  virtual unsigned int GetCompressedFileSize(char* filename) override;

  // Inherited via IConsoleVarSink
  virtual void OnAfterVarChange(ICVar* pVar) override;
  virtual void OnVarUnregister(ICVar* pVar) override;
};

void AddInternalCommands(ISystem* pSystem);
