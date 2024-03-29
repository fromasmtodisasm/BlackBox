
//////////////////////////////////////////////////////////////////////
//
//	Crytek Source code
//	Copyright (c) Crytek 2001-2004
//
//  File: XArea.h
//  Description: 2D area class. Area is in XY plane. Area has entities are attached to it.
//	Area has fade width (m_Proximity) - distance from border to inside at which fade coefficient
//	changes linearly from 0(on the border) to 1(inside distance to border more than m_Proximity).
//	A 2D areas manager, checks player for entering/leaving areas. Updates fade
//	coefficient for areas player is in
//
//  History:
//  - Feb 24, 2002: Created by Kirill Bulatsev
//	- February 2005: Modified by Marco Corbetta for SDK release
//
//////////////////////////////////////////////////////////////////////

#if !defined(_XAREA2D_H__INCLUDED_)
	#define _XAREA2D_H__INCLUDED_

	#pragma once

	#include <BlackBox\Core\IMarkers.hpp>
    //#include <Cry_Math.h>
	#include <BlackBox/Math/MathHelper.hpp>

class CXGame;
//////////////////////////////////////////////////////////////////////
class CXAreaUser
{
	friend class CXArea;
	friend class CXAreaMgr;

	CXGame*          m_pGame;
	std::vector<int> m_HostedAreasIdx;
	Legacy::Vec3     m_vPos;
	IEntity*         m_pEntity;

public:
	void SetGame(CXGame* pGame) { m_pGame = pGame; }
	void SetEntity(IEntity* pEntity) { m_pEntity = pEntity; }
};

//////////////////////////////////////////////////////////////////////
class CXArea : public IXArea
{
public:
	typedef enum
	{
		ATP_SHAPE = 0,
		ATP_SPHERE,
		ATP_SECTOR,
		ATP_BOX,
	} tAreaType;
	struct a2DPoint
	{
		float x, y;
		a2DPoint(void)
		    : x(0.0f)
		    , y(0.0f)
		{
		}
		a2DPoint(const Legacy::Vec3& pos3D)
		{
			x = pos3D.x;
			y = pos3D.y;
		}
		float DistSqr(const struct a2DPoint& point) const
		{
			float xx = x - point.x;
			float yy = y - point.y;
			return (xx * xx + yy * yy);
		}
		float DistSqr(const float px, const float py) const
		{
			float xx = x - px;
			float yy = y - py;
			return (xx * xx + yy * yy);
		}
	};
	struct a2DBBox
	{
		a2DPoint min; // 2D BBox min
		a2DPoint max; // 2D BBox max
		bool     PointOutBBox2D(const a2DPoint& point) const
		{
			return (point.x < min.x || point.x > max.x || point.y < min.y || point.y > max.y);
		}
		bool PointOutBBox2DVertical(const a2DPoint& point) const
		{
			return (point.y <= min.y || point.y > max.y || point.x > max.x);
		}
		bool BBoxOutBBox2D(const a2DBBox& box) const
		{
			return (box.max.x < min.x || box.min.x > max.x || box.max.y < min.y || box.min.y > max.y);
		}
	};
	struct a2DSegment
	{
		bool    isHorizontal; //horizontal flag
		float   k, b;         //line parameters y=kx+b
		a2DBBox bbox;         // segment's BBox
		bool    IntersectsXPos(const a2DPoint& point) const
		{
			return (point.x < (point.y - b) / k);
		}

		float GetIntersectX(const a2DPoint& point) const
		{
			return (point.y - b) / k;
		}

		bool IntersectsXPosVertical(const a2DPoint& point) const
		{
			if (k == 0.0f)
				return (point.x < b);
			return false;
		}
	};

	CXArea(void);
	~CXArea(void);

	void      SetPoints(const Legacy::Vec3* const vPoints, const int count);
	void      SetID(const int id) { m_AreaID = id; }
	int       GetID() const { return m_AreaID; }

	void      SetGroup(const int id) { m_AreaGroupID = id; }
	int       GetGroup() const { return m_AreaGroupID; }

	void      SetAreaType(const tAreaType type) { m_AreaType = type; }
	tAreaType GetAreaType() const { return m_AreaType; }

	void      SetCenter(const Legacy::Vec3& center) { m_Center = center; }
	void      SetRadius(const float rad)
	{
		m_Radius  = rad;
		m_Radius2 = m_Radius * m_Radius;
	}

	void     SetMin(const Legacy::Vec3& min) { m_Min = min; }
	void     SetMax(const Legacy::Vec3& max) { m_Max = max; }
	void     SetTM(const Legacy::Matrix44& TM);

	void     SetVOrigin(float org) { m_VOrigin = org; }
	void     SetVSize(float sz = 0.0f) { m_VSize = sz; }

	void     AddEntity(const char* const clsName);
	void     AddEntites(const std::vector<string>& names);
	void     AddEntity(const EntityId entId);
	void     AddEntites(const std::vector<EntityId>& entIDs);

	void     ClearEntities();

	void     SetProximity(float prx) { m_Proximity = prx; }
	float    GetProximity() { return m_Proximity; }

	float    IsPointWithinDist(const a2DPoint& point) const;
	bool     IsPointWithin(const Legacy::Vec3& point3d) const;
	float    CalcDistToPoint(const a2DPoint& point) const;

	void     UpdateIDs(ISystem* pSystem);
	void     EnterArea(CXAreaUser& user);
	void     EnterArea(IEntity* const pEntity, ISystem* pSystem);
	void     LeaveArea(CXAreaUser& user);
	void     UpdateArea(CXAreaUser& user);
	float    CalculateFade(const Legacy::Vec3& pos3D);
	void     ProceedFade(CXAreaUser& user, const float fadeValue);

	void     Draw(const ISystem* const pSystem, const int idx);

	int      m_stepID;
	bool     m_bIsActive;

	unsigned MemStat();

private:
	void                     AddSegment(const a2DPoint& p0, const a2DPoint& p1);
	void                     CalcBBox();
	void                     ClearPoints();

	float                    m_Proximity;

	//	attached entities names list
	std::vector<string>      m_vsEntityName;
	//	attached entities IDs list
	std::vector<EntityId>    m_vEntityID;

	float                    m_PrevFade;

	int                      m_AreaID;
	int                      m_AreaGroupID;

	Legacy::Matrix44         m_InvMatrix;

	tAreaType                m_AreaType;

	// for shape areas
	// area's bbox
	a2DBBox                  m_BBox;
	// the area segments
	std::vector<a2DSegment*> m_vpSegments;

	// for box areas
	Legacy::Vec3             m_Min;
	Legacy::Vec3             m_Max;

	// for sphere areas
	Legacy::Vec3             m_Center;
	float                    m_Radius;
	float                    m_Radius2;

	//	area vertical origin - the lowest point of area
	float                    m_VOrigin;
	//	area height (vertical size). If (m_VSize<=0) - not used, only 2D check is done. Otherwise
	//	additional check for Z to be in [m_VOrigin, m_VOrigin + m_VSize] range is done
	float                    m_VSize;
};

//////////////////////////////////////////////////////////////////////
struct IVisArea;
struct IXAreaMgr;

//////////////////////////////////////////////////////////////////////
//Areas manager
class CXAreaMgr : public IXAreaMgr
{
public:
	CXAreaMgr(void);
	virtual ~CXAreaMgr(void);

	void                     Init(ISystem* pSystem);
	// adds a shape area
	CXArea*                  AddArea(const Legacy::Vec3* const vPoints, const int count, const std::vector<string>& names, const int id, const int groupId = -1, const float width = 0.0f);
	// adds a box area
	CXArea*                  AddArea(const Legacy::Vec3& min, const Legacy::Vec3& max, const Legacy::Matrix44& TM, const std::vector<string>& names, const int id, const int groupId = -1, const float width = 0.0f);
	// adds a sphere area
	CXArea*                  AddArea(const Legacy::Vec3& center, const float radius, const std::vector<string>& names, const int id, const int groupId = -1, const float width = 0.0f);

	void                     ReTriggerArea(IEntity* pEntity, const Legacy::Vec3& vPos, bool bIndoor);

	void                     UpdatePlayer(CXAreaUser& user);
	bool                     ProceedExclusiveEnter(CXAreaUser& user, unsigned int curIdx);
	bool                     ProceedExclusiveLeave(CXAreaUser& user, unsigned int curIdx);
	void                     ProceedExclusiveUpdate(CXAreaUser& user, unsigned int curIdx);

	typedef std::vector<int> intVector;
	int                      FindHighestHostedArea(intVector& hostedAreas, unsigned int curIdx);
	void                     ExitAllAreas(CXAreaUser& user);
	CXArea*                  GetArea(const Legacy::Vec3& point);
	void                     DeleteArea(const IXArea* aPtr);
	void                     Clear();

	void                     DrawAreas(const ISystem* const pSystem);
	unsigned                 MemStat();

	void                     RetriggerAreas();

	virtual IXArea*          CreateArea(const Legacy::Vec3* vPoints, const int count, const std::vector<std::string>& names,
	                                    const int type = 0, const int groupId = -1, const float width = 0.0f, const float height = 0.0f) override;
	virtual IXArea*          CreateArea(const Legacy::Vec3& min, const Legacy::Vec3& max, const Legacy::Matrix44& TM, const std::vector<std::string>& names,
	                                    const int type = 0, const int groupId = -1, const float width = 0.0f) override;
	virtual IXArea*          CreateArea(const Legacy::Vec3& center, const float radius, const std::vector<std::string>& names,
	                                    const int type = 0, const int groupId = -1, const float width = 0.0f) override;

	IVisArea *               m_pPrevArea, *m_pCurrArea;

private:
	std::vector<CXArea*> m_vpAreas;
	int                  m_sCurStep;
	ISystem*             m_pSystem;
	Legacy::Vec3         m_lastUpdatePos;
};

#endif //!defined(_XAREA2D_H__INCLUDED_)
