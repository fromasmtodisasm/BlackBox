#include "pch.h"
#include "ScriptObjectEntity.hpp"
#include "System.hpp"

#include <BlackBox/EntitySystem/IEntitySystem.hpp>


_DECLARE_SCRIPTABLEEX(CScriptObjectEntity)

CScriptObjectEntity::CScriptObjectEntity()
{
	//m_nCurrSoundId = -1;
}

CScriptObjectEntity::~CScriptObjectEntity()
{
}

bool CScriptObjectEntity::Create(IScriptSystem* pScriptSystem, ISystem* pSystem)
{
	/*m_pEntitySystem = (IEntitySystem*)pSystem->GetIEntitySystem();
	m_pISystem = pSystem;
	m_pSoundSystem = m_pISystem->GetISoundSystem();
	m_nCurrSoundId = -1;*/
	Init(pScriptSystem, this);
	assert(m_pScriptThis);
	m_pScriptThis->RegisterParent(this);

	assert(m_pScriptThis);
	return true;
}


void CScriptObjectEntity::SetEntity(IEntity* pEntity)
{
	assert(m_pScriptThis);
	m_pEntity = pEntity;
	assert(m_pScriptThis);
	SmartScriptObject pObj(m_pScriptSystem, true);
	assert(m_pScriptThis);

	string sClassname = m_pEntity->GetEntityClassName();
	assert(m_pScriptThis);
	if (!m_pScriptSystem->GetGlobalValue(sClassname.c_str(), *pObj))
	{
		m_pISystem->GetILog()->LogToFile("[FATAL ERROR] Script table %s not found. Probably script was not loaded because of an error.", sClassname.c_str());

		//return; // vlad: to be able to load level created for xbox

		CryError("[FATAL ERROR] Script table %s not found. Probably script was not loaded because of an error.", sClassname.c_str());

		//m_pISystem->GetIConsole()->Exit("[FATAL ERROR] Script table %s not found. Probably script was not loaded because of an error.",sClassname.c_str());

		//m_pISystem->Quit();
	}
	assert(m_pScriptThis);
	m_pScriptThis->Clone(*pObj);
	assert(m_pScriptThis);
	if (m_pEntity != NULL)
	{
		assert(m_pScriptThis);
		m_pScriptThis->SetValue("id", ((int)m_pEntity->GetId()));
		assert(m_pScriptThis);
		m_pScriptThis->SetValue("classid", ((int)m_pEntity->GetClassId()));
		assert(m_pScriptThis);
		m_pScriptThis->SetValue("classname", m_pEntity->GetEntityClassName());
		assert(m_pScriptThis);
	}
	else
	{
		assert(m_pScriptThis);
		m_pScriptThis->SetToNull("id");
		assert(m_pScriptThis);
		m_pScriptThis->SetToNull("classid");
		assert(m_pScriptThis);
		m_pScriptThis->SetToNull("classname");
		assert(m_pScriptThis);
	}
	assert(m_pScriptThis);
}

void CScriptObjectEntity::SetContainer(IScriptObject *pContainer)
{
	/*if(pContainer!=NULL)
	{
		EnablePropertiesMapping();
		RegisterProperty("cnt",pContainer);
	}*/
	if(pContainer!=NULL)
		m_pScriptThis->SetValue("cnt",pContainer);
	else
		m_pScriptThis->SetToNull("cnt");
//	_SmartScriptObject pCntTest(m_pScriptSystem,true);
//	if(!m_pScriptThis->GetValue("cnt",pCntTest))

}

void CSystem::CreateEntityScriptBinding(IEntity* ent)
{
	CScriptObjectEntity* pSEntity = new CScriptObjectEntity();
	pSEntity->Create(m_env.pScriptSystem, this);
	assert(pSEntity->GetScriptObject());
	if (ent->GetContainer()) {
		IScriptObject* pObj = ent->GetContainer()->GetScriptObject();
		if (pObj)
			pSEntity->SetContainer(pObj);
	}
	assert(pSEntity->GetScriptObject());
	pSEntity->SetEntity(ent);
	assert(pSEntity->GetScriptObject());
	ent->SetScriptObject(pSEntity->GetScriptObject());
	assert(pSEntity->GetScriptObject());
	assert(ent->GetScriptObject());
}

