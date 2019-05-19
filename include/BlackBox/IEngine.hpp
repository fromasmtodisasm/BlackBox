#pragma once

#define SYSTEM_API

struct IGame;
struct IShaderManager;
struct IRender;
struct ILog;

struct IEngine
{
  virtual void Init() = 0;
  virtual void Start() = 0;
  virtual void Release() = 0;

  virtual IShaderManager *getShaderManager() = 0;
  virtual IRender *getIRender() = 0;
  virtual ILog *getILog() = 0;
};

// Get the system interface (must be defined locally in each module)
extern IEngine *GetIEngine();

// interface of the DLL
extern "C"
{
  SYSTEM_API IEngine* CreateIEngine(void *);
}
