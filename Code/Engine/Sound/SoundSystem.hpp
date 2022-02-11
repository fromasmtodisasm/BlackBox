
class CSound;

struct SSoundCvars
{
	int   s_MusicEnable = 1;
	int   s_SoundEnable = 1;
	float s_MusicVolume = 1.f;
	float s_SFXVolume   = 1.f;
};

class CSoundSystem :
    public SSoundCvars,
    public ISoundSystem

{
public:
	~CSoundSystem();
	// Inherited via ISoundSystem
	virtual void                    Release() override;
	virtual void                    Update() override;
	virtual IMusicSystem*           CreateMusicSystem() override;
	virtual ISound*                 LoadSound(const char* szFile, int nFlags) override;
	virtual void                    SetMasterVolume(unsigned char nVol) override;
	virtual void                    SetMasterVolumeScale(float fScale, bool bForceRecalc = false) override;
	virtual ISound*                 GetSound(int nSoundID) override;
	virtual void                    PlaySound(int nSoundID) override;
	virtual void                    SetListener(const CCamera& cCam, const Legacy::Vec3& vVel) override;
	virtual void                    RecomputeSoundOcclusion(bool bRecomputeListener, bool bForceRecompute, bool bReset = false) override;
	virtual bool                    IsEAX(int version) override;
	virtual bool                    SetEaxListenerEnvironment(int nPreset, CS_REVERB_PROPERTIES* pProps = NULL, int nFlags = 0) override;
	virtual bool                    GetCurrentEaxEnvironment(int& nPreset, CS_REVERB_PROPERTIES& Props) override;
	virtual bool                    SetGroupScale(int nGroup, float fScale) override;
	virtual void                    Silence() override;
	virtual void                    Pause(bool bPause, bool bResetVolume = false) override;
	virtual void                    Mute(bool bMute) override;
	virtual void                    GetSoundMemoryUsageInfo(size_t& nCurrentMemory, size_t& nMaxMemory) override;
	virtual int                     GetUsedVoices() override;
	virtual float                   GetCPUUsage() override;
	virtual float                   GetMusicVolume() override;
	virtual void                    CalcDirectionalAttenuation(Legacy::Vec3& Pos, Legacy::Vec3& Dir, float fConeInRadians) override;
	virtual float                   GetDirectionalAttenuationMaxScale() override;
	virtual bool                    UsingDirectionalAttenuation() override;
	virtual void                    GetMemoryUsage(ICrySizer* pSizer) override;
	virtual IVisArea*               GetListenerArea() override;
	virtual Legacy::Vec3            GetListenerPos() override;
	virtual bool                    DebuggingSound() override;
	virtual int                     SetMinSoundPriority(int nPriority) override;
	virtual void                    LockResources() override;
	virtual void                    UnlockResources() override;

	bool                            Init();
	void                            DeleteThis();

	//////////////////////////////////////////////////////

	bool                            m_bAudioInited  = false;
	bool                            m_bMixerInited  = false;

	int                             m_AudioRate     = 22050;
	Uint16                          m_AudioFormat   = MIX_DEFAULT_FORMAT;
	int                             m_AudioChannels = 2;
	int                             m_AudioBuffers  = 4096;

	std::vector<_smart_ptr<CSound>> m_SoundList;
};