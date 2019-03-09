#include "Utils.hpp"

std::string getBasePath(std::string fullpath) {
  size_t pos = fullpath.find_last_of("\\");
  return fullpath.substr(0, pos);
}
