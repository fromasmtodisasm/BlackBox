#pragma once

#include <map>
#include <string>
#include <fstream>
#define YYDEBUG 1
#define YYFPRINTF(stderr, format, ...) CryError(format, __VA_ARGS__)
#pragma warning(push, 0)
#include "Parser.hpp"
#pragma warning(pop)
//#include "Scanner.hpp"

#ifdef PARSERDRIVER_EXPORTS
#	define PARSERDRIVER_API DLL_EXPORT
#else
#	define PARSERDRIVER_API DLL_IMPORT
#endif

class CEffect;
struct IEffect;


struct IDriver
{
	virtual ~IDriver()
	{
	}
	//virtual bool parse(const std::string& f) = 0;
	virtual IEffect* parse(const char* f) = 0;
	virtual void Release()					 = 0;
};

class Driver : public IDriver {
 public:
    Driver();
   ~Driver();
    //bool parse(const std::string& f) override;
    IEffect* parse(const char* f) override;

    bool        LoadEffectFromFile(IEffect* pEffect, const char * filename);
    bool        LoadEffect(IEffect* pEffect, const char * str);

    void ScanBegin(const char* file);
    void ScanEnd();

    bool trace_scanning;
    yy::location location;
    std::string file;
    bool trace_parsing;

    friend class Scanner;

    Scanner* scanner;
    yy::parser parser;
	CEffect* currentEffect;
    std::vector<std::string_view> CommonCode;
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
