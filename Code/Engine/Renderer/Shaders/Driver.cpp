#pragma warning(push, 0)
#include <BlackBox/Core/Platform/platform_impl.inl>
#include <BlackBox/Core/Path.hpp>
#include "Driver.hpp"
#include "Scanner.hpp"

#include "Effect.hpp"

#define PARSERDRIVER_EXPORTS

Driver::Driver()
    : trace_parsing(false)
    , trace_scanning(false)
    , scanner(new Scanner(*this))
    , parser(*scanner, *this)
{
}

Driver::~Driver()
{
	delete scanner;
}

IEffect* Driver::parse(const char* f)
{
	std::unique_ptr<CEffect> pEffect = std::make_unique<CEffect>(CEffect(PathUtil::GetFileName(f)));
	currentEffect                    = pEffect.get();
	pEffect->m_Techniques.clear();
	CommonCode.clear();
	ScanBegin(f);
	parser.set_debug_level(trace_parsing);
	int res = parser.parse();
	if (res == 0)
	{
		Env::Log()->Log("$3[FX] File %s passed", file.c_str());
		pEffect->m_Code = std::move(scanner->shader);
	}
	else
	{
		pEffect.reset();
	}

	ScanEnd();
	return pEffect.release();
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
