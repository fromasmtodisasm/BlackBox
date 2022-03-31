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
	auto  redColor   = Legacy::Vec3(0, 0, 1);
	auto  greenColor = Legacy::Vec3(0, 1, 0);
	auto  blueColor  = Legacy::Vec3(1, 0, 0);
	float axisLength = 20;

	//Env::AuxGeomRenderer()->DrawLine({0, -axisLength, 0}, {redColor}, {0, axisLength, 0}, {redColor});
	//Env::AuxGeomRenderer()->DrawLine({-axisLength, 0, 0}, {greenColor}, {axisLength, 0, 0}, {greenColor});
	//Env::AuxGeomRenderer()->DrawLine({0, 0, -axisLength}, {blueColor}, {0, 0, axisLength}, {blueColor});

	for each (const auto& area in m_Areas)
	{
		SRendParams rp;
		area->Render(rp, {});
	}
}

void CTerrain::Update()
{
}
