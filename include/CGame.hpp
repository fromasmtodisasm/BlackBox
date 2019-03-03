#pragma once
#include "IGame.hpp"
#include "IWindow.hpp"

#include <common.h>
#include <map>
#include <string>
#include <vector>

using string = std::string;

class CGame : public IGame {
private:
	 IWindow *win;
	 std::map<string, UniType> props;
	 string title;
public:
	bool setConfig(std::map<string,UniType>);
	bool readConfig();
	bool init();
	bool update();
	bool run();
};
