#pragma once

struct IStatObj;

class CTerrain
{
public:
	CTerrain();
	~CTerrain();

	void Render(CCamera& Camera);
	void Update();

private:

	std::vector<_smart_ptr<IStatObj>> m_Areas;
};

