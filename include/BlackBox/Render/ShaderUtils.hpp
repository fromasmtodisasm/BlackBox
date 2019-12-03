#pragma once

#include <map>
#include <string>

struct ShaderDesc
{
  ShaderDesc() {}
  ShaderDesc(std::string name) : name(name) {}
  ShaderDesc(std::string name, std::string type) : type(type), name(name) {}

  using Macro = std::map<std::string, std::string>;
  std::string type;
  std::string name;
  Macro macro;

  static std::string root;
};
struct ProgramDesc
{
  std::string name;
  ShaderDesc vs;
  ShaderDesc fs;
  ShaderDesc gs;
  ShaderDesc cs;
};