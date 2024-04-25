#include "stdafx.h"
#include <TagPoint.hpp>

//////////////////////////////////////////////////////////////////////////
unsigned CTagPoint::MemStats() const
{
	unsigned memSize = sizeof *this;
	memSize += m_sName.capacity();
	return memSize;
}

//////////////////////////////////////////////////////////////////////////
unsigned CEntityClassRegistry::MemStats() const
{
	unsigned size = sizeof *this;

	//for(EntityClassMapItor itr=m_vEntityClasses.begin(); itr!=m_vEntityClasses.end(); itr++)
	for (const auto& itr : m_vEntityClasses)
	{
		size += sizeof(EntityClass) + (itr.second).strFullScriptFile.capacity() + (itr.second).strScriptFile.capacity() + (itr.second).strClassName.capacity();
	}

	return size;
}

//////////////////////////////////////////////////////////////////////////
unsigned CXServerSlot::MemStats()
{
	unsigned size = sizeof *this;
	size += m_strPlayerName.capacity();
	size += m_strPlayerModel.capacity();
	return size;
}
