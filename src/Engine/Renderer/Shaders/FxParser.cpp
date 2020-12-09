#include <BlackBox/Core/Platform/CryLibrary.h>

#include "FxParser.h"
#include "driver.hpp"

#include <memory>

FxParser::FxParser()
{
}

bool FxParser::Parse(const std::string& f)
{

#define DLL_MODULE_INIT_ISYSTEM "ModuleInitISystem"
	//auto driver = std::unique_ptr<IDriver>(CreateParserDriver());
	CCryLibrary FxLibrary("FxParser");
	if (FxLibrary.IsLoaded())
	{
		PFNCREATEDRIVERINTERFACE CreateParserDriver = FxLibrary.GetProcedureAddress<PFNCREATEDRIVERINTERFACE>("CreateParserDriver");
		auto driver									= CreateParserDriver();

		typedef void* (*PtrFunc_ModuleInitISystem)(ISystem * pSystem, const char* moduleName);
		PtrFunc_ModuleInitISystem pfnModuleInitISystem = (PtrFunc_ModuleInitISystem)FxLibrary.GetProcedureAddress<PtrFunc_ModuleInitISystem>(DLL_MODULE_INIT_ISYSTEM);
		if (pfnModuleInitISystem)
		{
			pfnModuleInitISystem(gEnv->pSystem, "FxParser");
		}
		return driver->parse(f.data());
	}
	return false;
}
