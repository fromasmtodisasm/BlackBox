#include <winsock2.h>
#include <ws2tcpip.h>

#include <vector>
#include <thread>
#include <mutex>
#include <sstream>
#include <memory>
#include <string_view>
#include <functional>

#include "imgui.h"
#include <SDL.h>

namespace network
{
	enum class Server
	{
		MESSAGE,
		JOIN
	};
	enum class Client
	{
		MESSAGE,
		AUTH
	};
}

#include <functional>
struct AppData
{
	std::string Caption = "App";
	int Width = 400;
	int Height = 400;
	int Px = SDL_WINDOWPOS_CENTERED;
	int Py = SDL_WINDOWPOS_CENTERED;
};

struct MainLoop
{
	void Start();

	AppData AppData;
	std::function<void()> OnUpdate;
};