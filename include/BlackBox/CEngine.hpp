#pragma once

#include <BlackBox/IEngine.hpp>

class CEngine : public IEngine
{
  // Унаследовано через ISystem
  virtual void Init() override;
  virtual void Start() override;
  virtual void Release() override;
  virtual IShaderManager * getShaderManager() override;
  virtual IRender * getIRender() override;
};
