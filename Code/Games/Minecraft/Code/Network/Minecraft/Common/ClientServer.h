#pragma once
#include <chrono>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <sstream>
#include <string_view>
#include <thread>
#include <vector>
#include <type_traits>

using msg_type = uint8;
namespace network
{
	namespace msg
	{
		enum class Server : msg_type
		{
			MESSAGE,
			NAME,
			JOIN,
			CONNECT,
			CONNECTED,
			DISCONNECT,

			UPDATE_SNAKE,

			NEW_FOOD,
			EAT,

			NEW_DIR
		};
		enum class Client : msg_type
		{
			AUTH,
			LOOSE,
			MESSAGE,
			CHANGE_DIR
		};

	} // namespace msg
} // namespace network

struct User
{
	std::string name;
	std::string mail;
	std::string password;
};
