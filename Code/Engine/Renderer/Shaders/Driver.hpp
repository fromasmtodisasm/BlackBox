#pragma once

#include <map>
#include <string>
#include <fstream>
#define YYDEBUG 1
#define YYFPRINTF(stderr, format, ...) CryError(format, __VA_ARGS__)
#pragma warning(push, 0)
#include "Parser.hpp"
#include "Effect.hpp"
#pragma warning(pop)
//#include "Scanner.hpp"

#ifdef PARSERDRIVER_EXPORTS
#	define PARSERDRIVER_API DLL_EXPORT
#else
#	define PARSERDRIVER_API DLL_IMPORT
#endif

class FxEffect;
struct IEffect;


struct IDriver
{
	virtual ~IDriver()
	{
	}
	//virtual bool parse(const std::string& f) = 0;
	virtual FxEffect* parse(const char* f) = 0;
	virtual void Release()					 = 0;
};

class Driver : public IDriver {
 public:
    Driver();
   ~Driver();
    //bool parse(const std::string& f) override;
    FxEffect* parse(const char* f) override;

    bool        LoadEffectFromFile(IEffect* pEffect, const char * filename);
    bool        LoadEffect(IEffect* pEffect, const char * str);

    void ScanBegin(const char* file);
    void ScanEnd();

    bool trace_scanning;
    yy::location location;
    std::string file;
    bool trace_parsing;

    friend class Scanner;

private:
   FxEffect m_Effect;
 private:
    std::ifstream stream;
 private:
    Scanner* scanner;
    yy::parser parser;
    std::vector<std::string_view> CommonCode;

   // ������������ ����� IDriver
   virtual void Release() override;
};

typedef IDriver* (*PFNCREATEDRIVERINTERFACE)();
extern "C"
{
	PARSERDRIVER_API IDriver* CreateParserDriver();
}

extern "C"
{
  IDriver* CreateParserDriverStatic();
}
