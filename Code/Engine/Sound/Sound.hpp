#include <BlackBox\Core\smartptr.hpp>
#include "SoundBase.hpp"
//#define SOUND_SAMPLE

class CSound : public ISound, _reference_target_t, SSoundCvars
{
	virtual ~CSound();
  public:
#ifdef SOUND_SAMPLE
	using SampleType = Mix_Chunk;
#else
	union SampleType
	{
		Mix_Chunk* Sample;
		Mix_Music* Music;
	}Data;

	typedef SampleType SampleType;
#endif
	CSound(SampleType pChunk);
	// Inherited via ISound
	virtual void AddEventListener(ISoundEventListener* pListener) override;
	virtual void RemoveEventListener(ISoundEventListener* pListener) override;
	virtual bool IsPlaying() override;
	virtual bool IsPlayingVirtual() override;
	virtual bool IsLoading() override;
	virtual bool IsLoaded() override;
	virtual void Play(float fVolumeScale = 1.0f, bool bForceActiveState = true, bool bSetRatio = true) override;
	virtual void PlayFadeUnderwater(float fVolumeScale = 1.0f, bool bForceActiveState = true, bool bSetRatio = true) override;
	virtual void Stop() override;
	virtual const char* GetName() override;
	virtual const int	GetId() override;
	virtual void		SetLoopMode(bool bLoop) override;
	virtual bool		Preload() override;
	virtual unsigned int GetCurrentSamplePos(bool bMilliSeconds = false) override;
	virtual void		 SetCurrentSamplePos(unsigned int nPos, bool bMilliSeconds) override;
	virtual void		 SetPitching(float fPitching) override;
	virtual void		 SetRatio(float fRatio) override;
	virtual int			 GetFrequency() override;
	virtual void		 SetPitch(int nPitch) override;
	virtual void		 SetPan(int nPan) override;
	virtual void		 SetMinMaxDistance(float fMinDist, float fMaxDist) override;
	virtual void		 SetConeAngles(float fInnerAngle, float fOuterAngle) override;
	virtual void		 AddToScaleGroup(int nGroup) override;
	virtual void		 RemoveFromScaleGroup(int nGroup) override;
	virtual void		 SetScaleGroup(unsigned int nGroupBits) override;
	virtual void		 SetVolume(int nVolume) override;
	virtual int			 GetVolume() override;
	virtual void		 SetPosition(const Legacy::Vec3& pos) override;
	virtual const bool	 GetPosition(Legacy::Vec3& vPos) override;
	virtual void		 SetVelocity(const Legacy::Vec3& vel) override;
	virtual Legacy::Vec3 GetVelocity(void) override;
	virtual void		 SetDirection(const Legacy::Vec3& dir) override;
	virtual Legacy::Vec3 GetDirection() override;
	virtual void		 SetLoopPoints(const int iLoopStart, const int iLoopEnd) override;
	virtual bool		 IsRelative() const override;
	virtual int			 AddRef() override;
	virtual int			 Release() override;
	virtual void		 SetSoundProperties(float fFadingValue) override;
	virtual void		 FXEnable(int nEffectNumber) override;
	virtual void		 FXSetParamEQ(float fCenter, float fBandwidth, float fGain) override;
	virtual int			 GetLengthMs() override;
	virtual int			 GetLength() override;
	virtual void		 SetSoundPriority(unsigned char nSoundPriority) override;

	static void SetMusicVolume();

  public:
	void DeleteThis();

	static CSound* Load(const char* path, int nFlags);

	int	  nFlags;
	int	  m_nChannel;
	float m_Volume;
};
