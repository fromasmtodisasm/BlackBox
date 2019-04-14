#pragma once

struct ISystem;
struct IInputHandler;


struct IGame {
	virtual bool init(ISystem *pSystem) = 0;
	virtual bool update() = 0;
	virtual bool run() = 0;

  virtual IInputHandler *getInputHandler() = 0;
};

extern IGame *p_gIGame;
extern "C" {
  IGame* CreateIGame(const char *title);
}
