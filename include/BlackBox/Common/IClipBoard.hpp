#pragma once

#include <string>

namespace bb {
	class ClipBoard
	{
	public:
		static std::wstring GetString();
		static void SetString(std::wstring);
	};
}
