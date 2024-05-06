#pragma once

#include "ICryAnimation.h"


class CryCharManager : public ICryCharManager
{

public:

	CryCharManager(ISystem* pSystem);

	// Inherited via ICryCharManager
	void GetStatistics(Statistics& rStats) override;
	void GetMemoryUsage(ICrySizer* pSizer) const override;
	ICryCharInstance* MakeCharacter(const char* szFilename, unsigned nFlags) override;
	ICryCharModel* LoadModel(const char* szFileName, unsigned nFlags) override;
	void RemoveCharacter(ICryCharInstance* pCryCharInstance, unsigned nFlags) override;
	void ClearDecals() override;
	void ClearResources(void) override;
	void Update() override;
	void UnloadAnimation(const char* szFileName) override;
	void StartLoadAnimation(const char* szFileName, float fWhenRequired) override;
	void Release() override;
	void LockResources() override;
	void UnlockResources() override;

private:
	ISystem* m_pSystem;
};