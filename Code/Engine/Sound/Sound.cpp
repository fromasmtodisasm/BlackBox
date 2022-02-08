
#include "Sound.hpp"

CSound::~CSound()
{
	DeleteThis();
}

CSound::CSound(SampleType pChunk) : Data(pChunk)
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

void CSound::Play(float fVolumeScale/* = 1.0f*/, bool bForceActiveState/* = true*/, bool bSetRatio/* = true*/)
{
	#ifdef SOUND_SAMPLE
	auto channel = Mix_PlayChannel(-1, m_Sample, 0);
	#else
	auto channel = Mix_PlayMusic(Data.Music, 0);
	#endif
	CryLog("[Sound] Playing at channel: %d", channel);
}

void CSound::PlayFadeUnderwater(float fVolumeScale/* = 1.0f*/, bool bForceActiveState/* = true*/, bool bSetRatio/* = true*/)
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

unsigned int CSound::GetCurrentSamplePos(bool bMilliSeconds/* = false*/)
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
}

int CSound::GetVolume()
{
	return 0;
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
	if(nFlags & FLAG_SOUND_16BITS)
		Mix_FreeChunk(Data.Sample);
	else
	{
		Mix_FreeMusic(Data.Music);
	}
}

CSound* CSound::Load(const char* path, int nFlags)
{
	SampleType Data{};
	CSound*	   pSound{};
	char buf[256];
	sprintf(buf, "Data/%s", path);
	if(nFlags & FLAG_SOUND_16BITS)
		Data.Sample = Mix_LoadWAV(buf);
	else
		Data.Music = Mix_LoadMUS(buf);
	if (!Data.Sample)
	{
		CryError("Mix_LoadWA: %s", Mix_GetError());
	}
	else
	{
		pSound = new CSound(Data);
	}
	return pSound;
}
