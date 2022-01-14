#include "StatObject.hpp"

#include "ObjectManager.hpp"

IStatObj* CObjectManager::LoadStatObj(const char* szFileName, const char* szGeomName)
{
	string prefix = "Data/";

	CStatObj* obj{};
	{
		const string AdjastedFileName = prefix + szFileName;
		obj							  = stl::find_in_map(m_nameToObjectMap, AdjastedFileName, nullptr);

		if (obj)
		{
			CryLog("[INFO] Object [%s] already cached\n", AdjastedFileName.c_str());
		}
		else
		{
			CIndexedMesh im;
			if (im.LoadCGF(szFileName, szGeomName))
			{
				obj									= new CStatObj(im);
				m_nameToObjectMap[AdjastedFileName] = obj;
				obj->SetBBoxMin(obj->m_IndexedMesh.m_vBoxMin);
				obj->SetBBoxMax(obj->m_IndexedMesh.m_vBoxMax);
				CryLog("Object [%s] loaded\n", AdjastedFileName.c_str());
			}
		}
		if (obj == nullptr)
		{
			CryError("[ERROR] Error or load object: %s\n", AdjastedFileName.c_str());
		}
	}
	return obj;
}
