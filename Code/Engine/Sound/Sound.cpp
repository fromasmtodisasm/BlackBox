
#include "Sound.hpp"
#include "SoundSystem.hpp"

CSound::~CSound()
{
	DeleteThis();
}

CSound::CSound(SampleType pChunk)
    : Data(pChunk)
{
}

void CSound::AddEventListener(ISoundEventListener* pListener)
{
}

void CSound::RemoveEventListener(ISoundEventListener* pListener)
{
}

bool CSound::IsPlaying()
{
	return false;
}

bool CSound::IsPlayingVirtual()
{
	return false;
}

bool CSound::IsLoading()
{
	return false;
}

bool CSound::IsLoaded()
{
	return false;
}

void CSound::Play(float fVolumeScale /* = 1.0f*/, bool bForceActiveState /* = true*/, bool bSetRatio /* = true*/)
{
	//#ifdef SOUND_SAMPLE
	if (nFlags & FLAG_SOUND_MUSIC)
	{
		m_nChannel = Mix_PlayMusic(Data.Music, -1);
		m_Volume   = s_MusicVolume * MIX_MAX_VOLUME;
	}
	else
	{
		m_nChannel = Mix_PlayChannel(-1, Data.Sample, 0);
		m_Volume   = s_SFXVolume * MIX_MAX_VOLUME;
		SetVolume((int)m_Volume);
	}
	#if 0
	CryLog("[Sound] Playing at channel: %d", m_nChannel);
	#endif
}

void CSound::PlayFadeUnderwater(float fVolumeScale /* = 1.0f*/, bool bForceActiveState /* = true*/, bool bSetRatio /* = true*/)
{
}

void CSound::Stop()
{
}

const char* CSound::GetName()
{
	return nullptr;
}

const int CSound::GetId()
{
	return 0;
}

void CSound::SetLoopMode(bool bLoop)
{
}

bool CSound::Preload()
{
	return false;
}

unsigned int CSound::GetCurrentSamplePos(bool bMilliSeconds /* = false*/)
{
	return 0;
}

void CSound::SetCurrentSamplePos(unsigned int nPos, bool bMilliSeconds)
{
}

void CSound::SetPitching(float fPitching)
{
}

void CSound::SetRatio(float fRatio)
{
}

int CSound::GetFrequency()
{
	return 0;
}

void CSound::SetPitch(int nPitch)
{
}

void CSound::SetPan(int nPan)
{
}

void CSound::SetMinMaxDistance(float fMinDist, float fMaxDist)
{
}

void CSound::SetConeAngles(float fInnerAngle, float fOuterAngle)
{
}

void CSound::AddToScaleGroup(int nGroup)
{
}

void CSound::RemoveFromScaleGroup(int nGroup)
{
}

void CSound::SetScaleGroup(unsigned int nGroupBits)
{
}

void CSound::SetVolume(int nVolume)
{
	m_Volume = (float)nVolume;
	if (nFlags & FLAG_SOUND_MUSIC)
	{
		Mix_VolumeMusic(nVolume);
	}
	else
	{
		Mix_Volume(m_nChannel, nVolume);
	}
}

int CSound::GetVolume()
{
	return (int)m_Volume;
}

void CSound::SetPosition(const Legacy::Vec3& pos)
{
}

const bool CSound::GetPosition(Legacy::Vec3& vPos)
{
	return false;
}

void CSound::SetVelocity(const Legacy::Vec3& vel)
{
}

Legacy::Vec3 CSound::GetVelocity(void)
{
	return Legacy::Vec3();
}

void CSound::SetDirection(const Legacy::Vec3& dir)
{
}

Legacy::Vec3 CSound::GetDirection()
{
	return Legacy::Vec3();
}

void CSound::SetLoopPoints(const int iLoopStart, const int iLoopEnd)
{
}

bool CSound::IsRelative() const
{
	return false;
}

int CSound::AddRef()
{
	_reference_target_t::AddRef();
	return m_nRefCounter;
}

int CSound::Release()
{
	auto refs = m_nRefCounter;
	_reference_target_t::Release();
	return refs;
}

void CSound::SetSoundProperties(float fFadingValue)
{
}

void CSound::FXEnable(int nEffectNumber)
{
}

void CSound::FXSetParamEQ(float fCenter, float fBandwidth, float fGain)
{
}

int CSound::GetLengthMs()
{
	return 0;
}

int CSound::GetLength()
{
	return 0;
}

void CSound::SetSoundPriority(unsigned char nSoundPriority)
{
}

void CSound::DeleteThis()
{
	if (nFlags & FLAG_SOUND_MUSIC)
		Mix_FreeMusic(Data.Music);
	else
		Mix_FreeChunk(Data.Sample);
}

Sint64 istream_seek(struct SDL_RWops* context, Sint64 offset, int whence)
{
	CCryFile* file = (CCryFile*)context->hidden.unknown.data1;

	file->Seek(offset, whence);

	return file->GetPosition();
}

size_t istream_read(struct SDL_RWops* context, void* ptr,
                    size_t size, size_t maxnum)
{
	if (size == 0) return -1;
	CCryFile* file = (CCryFile*)context->hidden.unknown.data1;
	return file->Read((char*)ptr, size * maxnum) / size;

	//return file->bad() ? -1 : file->gcount() / size;
}

int istream_close(SDL_RWops* context)
{
	if (context)
	{
		delete (CCryFile*)context->hidden.unknown.data1;
		SDL_FreeRW(context);
	}
	return 0;
}

SDL_RWops* SDL_RWFromPak(CCryFile* stream)
{
	SDL_RWops* rwops;
	rwops = SDL_AllocRW();

	if (rwops != NULL)
	{
		rwops->seek                 = istream_seek;
		rwops->read                 = istream_read;
		rwops->write                = NULL;
		rwops->close                = istream_close;
		rwops->hidden.unknown.data1 = stream;
	}
	return rwops;
}

CSound* CSound::Load(const char* path, int nFlags)
{
	SampleType Data{};
	CSound*    pSound{};
	CCryFile* file = new CCryFile;
	if (file->Open(path, "rb"))
	{
		if (nFlags & FLAG_SOUND_MUSIC)
			Data.Music = Mix_LoadMUS_RW(SDL_RWFromPak(file), 0);
		else
			Data.Sample = Mix_LoadWAV_RW(SDL_RWFromPak(file), 0);
		if (!Data.Sample)
		{
			CryError("Mix_LoadWA: %s", Mix_GetError());
		}
		else
		{
			pSound         = new CSound(Data);
			pSound->nFlags = nFlags;
		}
	}
	return pSound;
}
