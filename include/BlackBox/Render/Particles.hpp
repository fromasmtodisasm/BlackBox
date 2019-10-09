#pragma once
#include <BlackBox/Render/BaseShader.hpp>

#include <memory>

class Particles
{
public:
	Particles(int cnt, float radius, float pos, float minV, float maxV, BaseShadeProgramrRef shader);
	~Particles();

	bool Create();
	void Draw(float dt);
	void initPos();
	void initVel();


private:
	BaseShadeProgramrRef shader;
	GLuint positions;
	GLuint velocities;
	int posCnt;
	float radius;
	float pos;
	float minV, maxV;

	/*
	class Impl;
	std::unique_ptr<Impl> Impl;
	*/
};