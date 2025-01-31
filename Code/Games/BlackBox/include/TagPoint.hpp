#pragma once

#include <BlackBox/Math/MathHelper.hpp>
#include <BlackBox/Core/IMarkers.hpp>
#include <Game.hpp>
#include <string>

class CGame;

//////////////////////////////////////////////////////////////////////
class CTagPoint : public ITagPoint
{
	Vec3	m_vPosition;
	Vec3	m_vAngles;
	string	m_sName;
	CXGame* m_pGame;

  public:
	CTagPoint(CXGame* pGame) { m_pGame = pGame; }
	virtual ~CTagPoint(){};

	virtual void SetPos(const Vec3& pos) { m_vPosition = pos; }
	virtual void GetPos(Vec3& pos) { pos = m_vPosition; }

	virtual void SetAngles(const Vec3& angles) { m_vAngles = angles; }
	virtual void GetAngles(Vec3& angles) { angles = m_vAngles; }

	virtual bool  SetName(const char* pName) { return m_pGame->RenameTagPoint(m_sName, pName); }
	virtual void  OverrideName(const string& name) { m_sName = name; }
	virtual char* GetName() { return (char*)m_sName.c_str(); }

	void Release() { delete this; }

	unsigned MemStats() const;
};
