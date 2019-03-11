#pragma once

#define EXPORT _cdecl

struct IGame {
	virtual bool init(bool debug) = 0;
	virtual bool update() = 0;
	virtual bool run() = 0;
};

IGame* EXPORT CreateIGame(char *title);
