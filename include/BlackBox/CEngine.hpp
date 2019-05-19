#pragma once

#include <BlackBox/IEngine.hpp>

class CEngine : public IEngine
{
private:
  ILog *m_pLog;
  IConsole *m_pConsole;
public:
  // Унаследовано через ISystem
  virtual void Init() override;
  virtual void Start() override;
  virtual void Release() override;
  virtual IShaderManager * getShaderManager() override;
  virtual IRender * getIRender() override;

  // Унаследовано через IEngine
  virtual ILog* getILog() override;

  // Унаследовано через IEngine
  virtual IConsole* getIConsole() override;
};
