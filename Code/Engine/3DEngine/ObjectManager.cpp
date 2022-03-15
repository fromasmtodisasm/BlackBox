#include "StatObject.hpp"

#include "ObjectManager.hpp"
#include <BlackBox/System/File/ICryPak.hpp>
#include <BlackBox/Core/Path.hpp>

IStatObj* CObjectManager::LoadStatObj(const char* szFileName, const char* szGeomName)
{
	CStatObj* obj{};
	{
		const string AdjastedFileName = PathUtil::Make(PathUtil::Make(PathUtil::GetEnginePath(), string("Engine/")).c_str(), szFileName);
		obj                           = stl::find_in_map(m_nameToObjectMap, AdjastedFileName, nullptr);

		if (obj)
		{
			CryLog("[INFO] Object [%s] already cached\n", AdjastedFileName.c_str());
		}
		else
		{
			CIndexedMesh im;
			if (im.LoadCGF(szFileName, szGeomName))
			{
				obj                                 = new CStatObj(im);
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
