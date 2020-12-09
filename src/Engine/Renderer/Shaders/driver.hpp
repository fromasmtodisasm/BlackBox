#pragma once

#include <map>
#include <string>
#include <fstream>
#pragma warning(push, 0)
#include "Parser.hpp"
#pragma warning(pop)
//#include "Scanner.hpp"

#ifdef PARSERDRIVER_EXPORTS
#	define PARSERDRIVER_API DLL_EXPORT
#else
#	define PARSERDRIVER_API DLL_IMPORT
#endif

struct IDriver
{
	virtual ~IDriver()
	{
	}
	//virtual bool parse(const std::string& f) = 0;
	virtual bool parse(const char* f) = 0;
	virtual void Release()					 = 0;
};

class Driver : public IDriver {
 public:
    Driver();
    //bool parse(const std::string& f) override;
    bool parse(const char* f) override;
    std::string file;
    bool trace_parsing;

    void scan_begin();
    void scan_end();

    bool trace_scanning;
    yy::location location;

    friend class Scanner;
    Scanner* scanner;
    yy::parser parser;
 private:
    std::ifstream stream;

   // Унаследовано через IDriver
   virtual void Release() override;
};

typedef IDriver* (*PFNCREATEDRIVERINTERFACE)();
extern "C"
{
	PARSERDRIVER_API IDriver* CreateParserDriver();
}
