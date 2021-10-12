#include "SoundSystem.hpp"
#include "Sound.hpp"

#include <BlackBox\System\ConsoleRegistration.h>

CSoundSystem::~CSoundSystem()
{
	DeleteThis();
}

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
	auto Sound = CSound::Load(szFile, nFlags); 
	if (Sound)
	{
		CryLog("[SoundSystem] %s loaded", szFile);
		m_SoundList.push_back(Sound);
	}
	return Sound;
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
	// -1 all channels
	Mix_Pause(-1);
}

void CSoundSystem::Mute(bool bMute)
{
	Mix_Volume(-1, 0);
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
	m_bAudioInited = true;

	if (Mix_OpenAudio(m_AudioRate, m_AudioFormat, m_AudioChannels, m_AudioBuffers) != 0)
	{
		CryError("Mix_OpenAudio: %s\n", Mix_GetError());
	}
	m_bMixerInited = true;

	REGISTER_CVAR(s_MusicEnable, s_MusicEnable, 0, "Enable/Disable music [1/0]");
	REGISTER_CVAR(s_MusicVolume, s_MusicVolume, 0, "Music volume [0..1]");

	return true;
}

void CSoundSystem::DeleteThis()
{
	if (m_bAudioInited)
	{
		SDL_QuitSubSystem(SDL_INIT_AUDIO);
		if (m_bMixerInited)
		{
			Mix_CloseAudio();
		}
	}
}

ISoundSystem* CreateSoundSystem(struct ISystem*, void* pInitData)
{
	auto AudioSystem = new CSoundSystem;
	if (!AudioSystem->Init())
	{
		AudioSystem->Release();
		AudioSystem = nullptr;
	}
	#if 0
	auto sound = _smart_ptr<ISound>(AudioSystem->LoadSound("res/audio/file_example_WAV_1MG.wav", 0));
	#else
	//auto sound = AudioSystem->LoadSound("Sounds/MenuMusic_shortversion.wav", 0);
	//auto sound = AudioSystem->LoadSound("Music/MENU/MenuMusic_shortversion_old.ogg", 0);
	#endif

	//sound->Play();
	return AudioSystem;
}

