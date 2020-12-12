#include <BlackBox/Core/Platform/CryLibrary.h>

#include "FxParser.h"
#include "driver.hpp"

#include <memory>

FxParser::FxParser()
{
}

bool FxParser::Parse(const std::string& f, PEffect* pEffect)
{

#define DLL_MODULE_INIT_ISYSTEM "ModuleInitISystem"
	//auto driver = std::unique_ptr<IDriver>(CreateParserDriver());
	//CCryLibrary ;
	HMODULE FxLibrary;
	if (FxLibrary = CryLoadLibrary("FxParser"))
	{
		PFNCREATEDRIVERINTERFACE CreateParserDriver = (PFNCREATEDRIVERINTERFACE)CryGetProcAddress(FxLibrary,"CreateParserDriver");
		auto driver									= CreateParserDriver();

		typedef void* (*PtrFunc_ModuleInitISystem)(ISystem * pSystem, const char* moduleName);
		PtrFunc_ModuleInitISystem pfnModuleInitISystem = (PtrFunc_ModuleInitISystem)CryGetProcAddress(FxLibrary, DLL_MODULE_INIT_ISYSTEM);
		if (pfnModuleInitISystem)
		{
			pfnModuleInitISystem(gEnv->pSystem, "FxParser");
		}
		*pEffect = driver->parse(f.data());
		return *pEffect != nullptr;
	}
	return false;
}
