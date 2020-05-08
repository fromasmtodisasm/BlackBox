#pragma once
struct ITechnique;

struct ITechniqueManager
{
  virtual ITechnique* get(std::string name) = 0;
  virtual ITechnique* add(std::string name, ITechnique* tech) = 0;
};