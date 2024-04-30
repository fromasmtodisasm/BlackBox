#pragma once


#include <BlackBox/ScriptSystem/IScriptSystem.hpp>
#include <BlackBox/ScriptSystem/_ScriptableEx.hpp>

struct IEntity;
struct ISystem;
class CScriptObjectVector;


class CScriptObjectEntity :
	public _ScriptableEx<CScriptObjectEntity>,
	public IScriptObjectSink
{
public:
	CScriptObjectEntity();
	virtual ~CScriptObjectEntity();
	bool Create(IScriptSystem* pScriptSystem, ISystem* pSystem);
	void SetEntity(IEntity* pEntity);
	void SetContainer(IScriptObject* pContainer);
	//IScriptObjectSink
	void OnRelease()
	{
		m_pScriptThis->Clear();
		m_pScriptThis = NULL;
		delete this;
	}
	static void InitializeTemplate(IScriptSystem* pSS);
	static void ReleaseTemplate();
public:


	IEntity *m_pEntity;
	IEntitySystem *m_pEntitySystem;
	ISystem *m_pISystem;
	ISoundSystem *m_pSoundSystem;
	int m_nCurrSoundId;
};
