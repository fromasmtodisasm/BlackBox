#pragma once
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/Core/MathHelper.hpp>

class BaseCommand : public IConsoleCommand
{
protected:
  //CGame* game;
  glm::vec3 unpack_vector(std::vector<std::wstring>::iterator it, int size = 3)
  {
    glm::vec3 pos;
    pos[0] = static_cast<float>(_wtof(it->c_str()));
    it++;
    pos[1] = static_cast<float>(_wtof(it->c_str()));
    if (size == 3)
    {
      it++;
      pos[2] = static_cast<float>(_wtof(it->c_str()));
    }
    return pos;
  };
public:
  BaseCommand(){}
  ~BaseCommand() = default;

private:
};

