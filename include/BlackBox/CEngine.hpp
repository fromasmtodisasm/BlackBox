#pragma once

#include <BlackBox/IEngine.hpp>

class CEngine : public IEngine
{
private:
  ILog *m_pLog;
  IConsole *m_pConsole;
  IGame *m_pGame;
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
};
