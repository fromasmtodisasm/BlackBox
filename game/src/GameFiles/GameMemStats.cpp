#include <TagPoint.hpp>

//////////////////////////////////////////////////////////////////////////
unsigned	CTagPoint::MemStats()
{
	unsigned memSize = sizeof *this;
	memSize += m_sName.capacity();
	return memSize;
}

//////////////////////////////////////////////////////////////////////////
unsigned CEntityClassRegistry::MemStats()
{
unsigned size = sizeof *this;

	for(EntityClassMapItor itr=m_vEntityClasses.begin(); itr!=m_vEntityClasses.end(); itr++)
	{
	  size += sizeof(EntityClass) + (itr->second).strFullScriptFile.capacity()
						+ (itr->second).strScriptFile.capacity()
						+ (itr->second).strClassName.capacity();
	}
	
	return size;
}
