#pragma once

#include <BlackBox/Renderer/Material.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/IShader.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/ILog.hpp>

#include <map>
#include <string>
#include <tinyxml2.h>

extern Material* defaultMaterial;
struct ISystem;

struct IMaterialShaderSink
{
  virtual void OnShaderFound(const char*& name) = 0;
};

struct IMaterialManager
{
  virtual Material* getMaterial(const char* name) = 0;
  virtual static bool init(const char* materialLib) = 0;
  virtual bool loadLib(const char* name) = 0;
}
