#include "pch.hpp"
#include "EntityCharacter.hpp"
#include "EntitySystem.hpp"
#include "IStatObj.h"

CEntityCharacter::CEntityCharacter()
{
}

CEntityCharacter::~CEntityCharacter()
{
}

bool CEntityCharacter::LoadCharacter(int pos, const char* fileName)
{
	return false;
}

bool CEntityCharacter::PhysicalizeCharacter(int pos, float mass, int surface_idx, float stiffness_scale/* = 1.0f*/, bool bInstant/* = false*/)
{
	return false;
}

void CEntityCharacter::KillCharacter(int pos)
{
}

void CEntityCharacter::SetCharacter(int pos, ICryCharInstance* character)
{
}

ICryCharInstance* CEntityCharacter::GetCharacter(int pos)
{
	return nullptr;
}

void CEntityCharacter::DrawCharacter(int pos, int mode)
{
}

void CEntityCharacter::ResetAnimations(int pos)
{
}

ILipSync* CEntityCharacter::GetLipSyncInterface()
{
	return nullptr;
}

void CEntityCharacter::ReleaseLipSyncInterface()
{
}
