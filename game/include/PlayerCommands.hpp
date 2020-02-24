#pragma once
#include <BlackBox/ICommand.hpp>

class CPlayer;
class CCamera;

class MoveForward : public ICommand
{
  CPlayer* player;
  CCamera* camera;
  float deltatime;
public:
  MoveForward(CPlayer* player);
  // Унаследовано через ICommand
  virtual void execute() override;
};

/*
class MoveForward : public ICommand
{
  CPlayer* player;
  CCamera* camera;
  float deltatime;
public:
  MoveForward(CPlayer* player);
  // Унаследовано через ICommand
  virtual void execute() override;
};
*/