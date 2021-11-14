#pragma once

class CObjectManager
{
  public:
	IStatObj* LoadStatObj(const char* szFileName, const char* szGeomName = 0);

  private:
	typedef std::map<string, CStatObj*, stl::less_stricmp<string>> ObjectsMap;
	ObjectsMap													   m_nameToObjectMap;

	typedef std::set<CStatObj*> LoadedObjects;
	LoadedObjects				m_lstLoadedObjects;
};