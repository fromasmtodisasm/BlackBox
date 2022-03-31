#pragma once

#include <BlackBox/EntitySystem/IEntitySystem.hpp>

class CEntityCharacter : public IEntityCharacter
{
public:
	CEntityCharacter();
	~CEntityCharacter();

	// Inherited via IEntityCharacter
	virtual bool              LoadCharacter(int pos, const char* fileName) override;
	virtual bool              PhysicalizeCharacter(int pos, float mass, int surface_idx, float stiffness_scale = 1.0f, bool bInstant = false) override;
	virtual void              KillCharacter(int pos) override;
	virtual void              SetCharacter(int pos, ICryCharInstance* character) override;
	virtual ICryCharInstance* GetCharacter(int pos) override;
	virtual void              DrawCharacter(int pos, int mode) override;
	virtual void              ResetAnimations(int pos) override;
	virtual ILipSync*         GetLipSyncInterface() override;
	virtual void              ReleaseLipSyncInterface() override;
};
