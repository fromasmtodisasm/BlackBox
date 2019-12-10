#pragma once

struct ICommand
{
  virtual void execute() = 0;
};