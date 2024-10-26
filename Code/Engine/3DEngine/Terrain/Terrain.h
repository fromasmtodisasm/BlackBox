#pragma once

struct IStatObj;

struct SSector
{
	int x, y;
};

class CTerrain
{
public:
	CTerrain();
	~CTerrain();

	void Render(CCamera& Camera);
	void Update();

	void DrawAxises();

	void Nick(Legacy::Vec3 dir, Legacy::Vec3 normal, float from, float to, float step = 1.f);

private:
	std::vector<_smart_ptr<IStatObj>> m_Areas;

	int m_SectorSize = 100;
	
	std::vector<SSector> m_Sectors;
};
