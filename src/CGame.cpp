#include "CGame.hpp"


bool CGame::readConfig()
{
	/*switch (it.second.t) {
	case TSTR:

	case TIVAL:
	case TDVAL:
	case TLIVAL:
	case TLDVAL:
	}
	}*/


	for (auto it : props) { 
		if (it.first == "WinTitle")
		{
			switch (it.second.t) {
			case TSTR:
				this->title = it.second.sval;
			/*case TIVAL: case TDVAL: case TLIVAL: case TLDVAL:
				break;*/
			default:
				break;
			}
		}
	}
	return true;
}

bool CGame::setConfig(std::map<string, UniType> props)
{

	return false;
}

bool CGame::init() {
	readConfig();
  win = CreateIWindow(); 
  if (win != nullptr) {
    win->init();
    return win->create(); 
  }
  return false;
}

bool CGame::update() {
	while (win->update()) {
	
	}
  return true;
}

bool CGame::run() {
  update();
  return true;
}


IGame *CreateIGame() {
  CGame *game = new CGame();
  return (game);
}
