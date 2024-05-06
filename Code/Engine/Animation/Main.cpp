#include "pch.hpp"

#include "CryCharManager.h"

//////////////////////////////////////////////////////////////////////////
// The main symbol exported from CryAnimation: creator of the main (root) interface/object
ICryCharManager* CreateCharManager(ISystem* pSystem, const char* szInterfaceVersion)
{
	// check the interface timestamp
#if 0
#if !defined(LINUX)
	if (strcmp(szInterfaceVersion, gAnimInterfaceVersion))
		pSystem->GetILog()->LogError("CreateCharManager(): CryAnimation interface version error");
#endif
#endif
	CryCharManager* pCryCharManager = new CryCharManager(pSystem);

	return static_cast<ICryCharManager*>(pCryCharManager);
}

