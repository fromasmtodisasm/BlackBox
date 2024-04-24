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
	auto       redColor   = Legacy::Vec3(0, 0, 1);
	auto       greenColor = Legacy::Vec3(0, 1, 0);
	auto       blueColor  = Legacy::Vec3(1, 0, 0);
	float      axisLength = 100.f;

	const auto oX         = Legacy::Vec3{1, 0, 0};
	const auto oY         = Legacy::Vec3{0, 1, 0};
	const auto oZ         = Legacy::Vec3{0, 0, 1};

	const auto A          = Legacy::Vec3{0, 1, 0};
	const auto B          = Legacy::Vec3{0, 0, 1};
	const auto C          = Legacy::Vec3{0, 1, 0};

	auto dir = Legacy::Vec3{1, 0, 0};
	Nick(oX, A, -axisLength, axisLength, 1.f);
	//Nick(oY, B, -axisLength, axisLength, 1.f);
	Nick(oZ, C, -axisLength, axisLength, 1.f);

	Env::AuxGeomRenderer()->DrawLine({0, -axisLength, 0}, {blueColor}, {0, axisLength, 0}, {blueColor});
	Env::AuxGeomRenderer()->DrawLine({-axisLength, 0, 0}, {redColor}, {axisLength, 0, 0}, {redColor});
	Env::AuxGeomRenderer()->DrawLine({0, 0, -axisLength}, {greenColor}, {0, 0, axisLength}, {greenColor});

}
void CTerrain::Nick(Legacy::Vec3 dir, Legacy::Vec3 normal, float from, float to, float step)
{
	auto  color  = UCol(Legacy::Vec3(1, 1, 1) * 0.15f);
	auto  v      = glm::normalize(glm::cross(dir, normal));
	auto  length = glm::length(to - from);
	float width  = 20;

	for (auto origin = from; origin < length; origin += step)
	{
		auto p = dir + dir * origin;

		Env::AuxGeomRenderer()->DrawLine(p + width * v, color, p - width * v, color);
	}
}
