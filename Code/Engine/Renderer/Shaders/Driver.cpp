#pragma warning(push, 0)
#include "pch.hpp"
#include <BlackBox/Core/Platform/platform_impl.inl>
#include <BlackBox/Core/Path.hpp>
#include "Driver.hpp"
#include "Scanner.hpp"

#include "Effect.hpp"

#define PARSERDRIVER_EXPORTS

#pragma optimize("", off)
Driver::Driver()
    : trace_parsing(false)
    , trace_scanning(false)
    , scanner(new Scanner(*this))
    , parser(*scanner, *this, m_Effect)
{
}

Driver::~Driver()
{
	delete scanner;
}

FxEffect* Driver::parse(const char* f)
{
	m_Effect.m_name = PathUtil::GetFileName(f);

	CommonCode.clear();
	ScanBegin(f);
	parser.set_debug_level(trace_parsing);
	int res = parser.parse();
	if (res == 0)
	{
		//Env::Log()->Log("$3[FX] File %s passed", file.c_str());
		m_Effect.m_Code = std::move(scanner->shader);
	}

	ScanEnd();
	return new FxEffect(m_Effect);
}

bool Driver::LoadEffectFromFile(IEffect* pEffect, const char* filename)
{
	return false;
}

bool Driver::LoadEffect(IEffect* pEffect, const char* str)
{
	return false;
}

void Driver::ScanBegin(const char* _file)
{
	file = _file;
	location.initialize(&file);
	scanner->set_debug(trace_scanning);
	if (file.empty() || file == "-")
	{
	}
	else
	{
		stream.open(file);
		if (!stream.is_open())
		{
			Env::Log()->LogError("[FX] File %s not found", file.c_str());
		}
		else
		{
			Env::Log()->Log("$3[FX] File %s opened", file.c_str());
		}
		//std::cout << file << std::endl;
		Env::Log()->Log(file.c_str());
		scanner->yyrestart(&stream);
	}
}

void Driver::ScanEnd()
{
	stream.close();
}

void Driver::Release()
{
	delete this;
}

extern "C" DLL_EXPORT IDriver* CreateParserDriver()
{
	return new Driver();
}
#pragma warning(pop)
