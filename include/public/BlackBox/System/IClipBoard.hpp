#pragma once

#include <string>

namespace bb {
  class ClipBoard
  {
  public:
    static std::wstring GetString()
    {
      return std::wstring();
    }
    static void SetString(std::wstring)
    {
    }
  };
}