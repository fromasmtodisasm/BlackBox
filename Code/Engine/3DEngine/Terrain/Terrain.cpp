#include "pch.hpp"
#include "Terrain.h"

#include <BlackBox/3DEngine/IStatObj.hpp>
#include <BlackBox/Renderer/IRenderAuxGeom.hpp>

CTerrain::CTerrain()
{
}

CTerrain::~CTerrain()
{
}

void CTerrain::Render(CCamera& Camera)
{
	DrawAxises();

	for (const auto& area : m_Areas)
	{
		SRendParams rp;
		area->Render(rp, {});
	}
}

void CTerrain::Update()
{
}

void CTerrain::DrawAxises()
{

}
void CTerrain::Nick(Legacy::Vec3 dir, Legacy::Vec3 normal, float from, float to, float step)
{
}
