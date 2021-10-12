#include "SoundSystem.hpp"

void CSoundSystem::Release()
{
	delete this;
}

void CSoundSystem::Update()
{
}

IMusicSystem* CSoundSystem::CreateMusicSystem()
{
	return nullptr;
}

ISound* CSoundSystem::LoadSound(const char* szFile, int nFlags)
{
	return nullptr;
}

void CSoundSystem::SetMasterVolume(unsigned char nVol)
{
}

void CSoundSystem::SetMasterVolumeScale(float fScale, bool bForceRecalc)
{
}

ISound* CSoundSystem::GetSound(int nSoundID)
{
	return nullptr;
}

void CSoundSystem::PlaySound(int nSoundID)
{
}

void CSoundSystem::SetListener(const CCamera& cCam, const Legacy::Vec3& vVel)
{
}

void CSoundSystem::RecomputeSoundOcclusion(bool bRecomputeListener, bool bForceRecompute, bool bReset)
{
}

bool CSoundSystem::IsEAX(int version)
{
	return false;
}

bool CSoundSystem::SetEaxListenerEnvironment(int nPreset, CS_REVERB_PROPERTIES* pProps, int nFlags)
{
	return false;
}

bool CSoundSystem::GetCurrentEaxEnvironment(int& nPreset, CS_REVERB_PROPERTIES& Props)
{
	return false;
}

bool CSoundSystem::SetGroupScale(int nGroup, float fScale)
{
	return false;
}

void CSoundSystem::Silence()
{
}

void CSoundSystem::Pause(bool bPause, bool bResetVolume)
{
}

void CSoundSystem::Mute(bool bMute)
{
}

void CSoundSystem::GetSoundMemoryUsageInfo(size_t& nCurrentMemory, size_t& nMaxMemory)
{
}

int CSoundSystem::GetUsedVoices()
{
	return 0;
}

float CSoundSystem::GetCPUUsage()
{
	return 0.0f;
}

float CSoundSystem::GetMusicVolume()
{
	return 0.0f;
}

void CSoundSystem::CalcDirectionalAttenuation(Legacy::Vec3& Pos, Legacy::Vec3& Dir, float fConeInRadians)
{
}

float CSoundSystem::GetDirectionalAttenuationMaxScale()
{
	return 0.0f;
}

bool CSoundSystem::UsingDirectionalAttenuation()
{
	return false;
}

void CSoundSystem::GetMemoryUsage(ICrySizer* pSizer)
{
}

IVisArea* CSoundSystem::GetListenerArea()
{
	return nullptr;
}

Legacy::Vec3 CSoundSystem::GetListenerPos()
{
	return Legacy::Vec3();
}

bool CSoundSystem::DebuggingSound()
{
	return false;
}

int CSoundSystem::SetMinSoundPriority(int nPriority)
{
	return 0;
}

void CSoundSystem::LockResources()
{
}

void CSoundSystem::UnlockResources()
{
}

bool CSoundSystem::Init()
{
	if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0)
	{
		CryError("Failed to initialize audio subsystem: %s", SDL_GetError());
		return false;
	}
	return true;
}

ISoundSystem* CreateSoundSystem(struct ISystem*, void* pInitData)
{
	auto AudioSystem = new CSoundSystem;
	if (!AudioSystem->Init())
	{
		AudioSystem->Release();
		AudioSystem = nullptr;
	}
	return AudioSystem;
}

