#pragma once

struct ISystem;

struct IGame {
	virtual bool init(ISystem *pSystem) = 0;
	virtual bool update() = 0;
	virtual bool run() = 0;
};

extern "C" {
  IGame* CreateIGame(const char *title);
}
