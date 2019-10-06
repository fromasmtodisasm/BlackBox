#include <BlackBox/Game/CGame.hpp>
#include <BlackBox/TagPoint.hpp>

//////////////////////////////////////////////////////////////////////
// Inserts a tag-point in the list.
ITagPoint* CGame::CreateTagPoint(const string& name, const Vec3& pos, const Vec3& angles)
{
  // create new one
  CTagPoint* pNewPoint = new CTagPoint(this);
  pNewPoint->OverrideName(name.c_str());
  pNewPoint->SetPos(pos);
  pNewPoint->SetAngles(angles);

  // insert it into the map
  m_mapTagPoints.insert(TagPointMap::iterator::value_type(name, pNewPoint));
  return (ITagPoint*)pNewPoint;
}

//////////////////////////////////////////////////////////////////////
// Remove tag-point.
void CGame::RemoveTagPoint(ITagPoint* pPoint)
{
  TagPointMap::iterator ti;
  // find and delete tag-point
  for (ti = m_mapTagPoints.begin(); ti != m_mapTagPoints.end(); ti++)
  {
    if (ti->second == pPoint)
    {
      m_mapTagPoints.erase(ti);
      pPoint->Release();
      return;
    }
  }
}

//////////////////////////////////////////////////////////////////////
// Retrieve a tag-point by its name
ITagPoint* CGame::GetTagPoint(const string& name)
{
  TagPointMap::iterator ti;
  // find and return tag-point
  if ((ti = m_mapTagPoints.find(name)) == m_mapTagPoints.end())
    return 0;
  else
    return (ITagPoint*)ti->second;
}

//////////////////////////////////////////////////////////////////////
bool CGame::RenameTagPoint(const string& oldname, const string& newname)
{
  TagPointMap::iterator ti;
  // find tag-point
  if ((ti = m_mapTagPoints.find(oldname)) != m_mapTagPoints.end())
  {
    // does the new name already exist ?
    if (m_mapTagPoints.find(newname) == m_mapTagPoints.end())
    {
      // change name
      CTagPoint* pPoint = ti->second;
      pPoint->OverrideName(newname);

      m_mapTagPoints.erase(oldname);
      m_mapTagPoints.insert(TagPointMap::iterator::value_type(newname, pPoint));

      return true;
    }
    else
      return false;
  }

  return false;
}

bool CGame::InitScripts()
{
	m_ScriptObjectConsole = new CScriptObjectConsole();
	CScriptObjectConsole::InitializeTemplate(m_pScriptSystem);
	m_ScriptObjectGame = new CScriptObjectGame();
	m_ScriptObjectGame->InitializeTemplate(m_pScriptSystem);

	m_ScriptObjectConsole->Init(m_pSystem->getIIScriptSystem(), m_Console);
	m_ScriptObjectGame->Init(m_pSystem->getIIScriptSystem(), this);

	m_pScriptSystem->ExecuteFile("scripts/common.lua", true, false);

	fps = 35.f;
	m_pScriptSystem->ExecuteFile("scripts/game.lua");

	m_playerObject = m_pScriptSystem->CreateEmptyObject();
	if (!m_pScriptSystem->GetGlobalValue("player", m_playerObject))
	{
		delete m_playerObject;
		m_pSystem->Log("\002 ERROR: can't find player table ");
		return false;
	}
	const char *name;
	int age;
	m_playerObject->GetValue("name", name);
	m_playerObject->GetValue("age", age);
	m_Console->PrintLine("Player name: %s", name);
	m_Console->PrintLine("Player age: %d", age);

	HSCRIPTFUNCTION psina;
	m_playerObject->GetValue("TestChanges", psina);

	m_playerObject->SetValue("name", "Psina");
	//m_pScriptSystem->BeginCall(m_playerObject, "TestChanges");
	m_pScriptSystem->BeginCall(psina);

	IScriptObject* console=nullptr;
	m_pScriptSystem->EndCall(console);

	HSCRIPTFUNCTION PrintLine = 0;
	/*console->GetValue("PrintLine", PrintLine);
	m_pScriptSystem->BeginCall(PrintLine);
	m_pScriptSystem->PushFuncParam("Test HSCRIPTFUNCTION call!!!");
	m_pScriptSystem->EndCall();
*/
	int n;
	m_pScriptSystem->BeginCall(m_ScriptObjectConsole->GetScriptObject(), "PrintLine");
	m_pScriptSystem->PushFuncParam("Test HSCRIPTFUNCTION call!!!");
	m_pScriptSystem->EndCall();

	m_playerObject->GetValue("name", name);
	m_Console->PrintLine("Player name: %s", name);

}
