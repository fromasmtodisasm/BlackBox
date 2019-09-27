#pragma once

#define SYSTEM_API

struct IGame;
struct IShaderManager;
struct IRender;
struct ILog;
struct IConsole;
struct IFont;
struct IWindow;
struct IInputHandler;
struct IScriptSystem;

struct ISystem
{
  virtual bool Init() = 0;
  virtual void Start() = 0;
  virtual void Update() = 0;
  virtual void BeginFrame() = 0;
  virtual void EndFrame() = 0;
  virtual void Release() = 0;
  virtual IGame *CreateGame(IGame* game) = 0;

  virtual IShaderManager *getShaderManager() = 0;
  virtual IRender *getIRender() = 0;
  virtual ILog *getILog() = 0;
  virtual IConsole *getIConsole() = 0;
  virtual IGame *getIGame() = 0;
  virtual IFont *getIFont() = 0;
  virtual IWindow *getIWindow() = 0;
  virtual IInputHandler *getIInputHandler() = 0;
	virtual IScriptSystem* getIIScriptSystem() = 0;
};

// Get the system interface (must be defined locally in each module)
extern ISystem *GetISystem();

// interface of the DLL
extern "C"
{
  SYSTEM_API ISystem* CreateISystem(void *);
}
