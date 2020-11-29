#pragma once

#include <map>
#include <string>
#include <fstream>
#pragma warning(push, 0)
#include "Parser.hpp"
#pragma warning(pop)
#include "Scanner.hpp"

class Driver {
 public:
    Driver();
    int parse(const std::string& f);
    std::string file;
    bool trace_parsing;

    void scan_begin();
    void scan_end();

    bool trace_scanning;
    yy::location location;

    friend class Scanner;
    Scanner scanner;
    yy::parser parser;
 private:
    std::ifstream stream;
};

