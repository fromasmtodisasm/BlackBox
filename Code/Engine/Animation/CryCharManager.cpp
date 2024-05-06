#include "pch.hpp"
#undef min

#include "CryCharManager.h"
#include "CryCharInstance.h"
#include <BlackBox/System/ISystem.hpp>
#undef min
#include "BlackBox/EntitySystem/IEntitySystem.hpp"
#include "BlackBox/EntitySystem/EntityDesc.hpp"

CryCharManager::CryCharManager(ISystem* pSystem)
  : m_pSystem(pSystem)
{
}

void CryCharManager::GetStatistics(Statistics& rStats)
{
}

void CryCharManager::GetMemoryUsage(ICrySizer* pSizer) const
{
}

ICryCharInstance* CryCharManager::MakeCharacter(const char* szFilename, unsigned nFlags)
{
		auto object = Env::I3DEngine()->MakeObject(szFilename);
    if (!object)
      return nullptr;
		CEntityDesc desc(56, 1);
		desc.name  = "AnimObject";
		auto* Jack = Env::EntitySystem()->SpawnEntity(desc);

		Jack->SetIStatObj(object);
		Jack->SetPos({-0, 40, -5});
		Jack->SetScale(glm::vec3(0.01f));
		Jack->SetAngles({-90, 0, 0});
		//Jack->Physicalize();
		//Env::I3DEngine()->RegisterEntity(Jack);

    auto pCryCharInstance = new CryCharInstance(Jack);

    return pCryCharInstance;
}

ICryCharModel* CryCharManager::LoadModel(const char* szFileName, unsigned nFlags)
{
    return nullptr;
}

void CryCharManager::RemoveCharacter(ICryCharInstance* pCryCharInstance, unsigned nFlags)
{
}

void CryCharManager::ClearDecals()
{
}

void CryCharManager::ClearResources(void)
{
}

void CryCharManager::Update()
{
}

void CryCharManager::UnloadAnimation(const char* szFileName)
{
}

void CryCharManager::StartLoadAnimation(const char* szFileName, float fWhenRequired)
{
}

void CryCharManager::Release()
{
}

void CryCharManager::LockResources()
{
}

void CryCharManager::UnlockResources()
{
}
