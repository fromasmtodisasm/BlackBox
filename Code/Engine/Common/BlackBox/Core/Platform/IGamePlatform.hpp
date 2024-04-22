#pragma once

#ifdef IGAMEPLATFORM_EXPORTS
	#define IGAMEPLATFORM_API DLL_EXPORT
#elif defined(IS_MONOLITHIC_BUILD) && defined(CRY_IS_APPLICATION)
	#define IGAMEPLATFORM_API
#else
	#define IGAMEPLATFORM_API DLL_IMPORT
#endif

struct ISystem;

struct IAchievement
{
    virtual void Unlock() = 0;
};

struct Achievement_t
{
	int         m_eAchievementID;
	const char* m_pchAchievementID;
	char        m_rgchName[128];
	char        m_rgchDescription[256];
	bool        m_bAchieved;
	int         m_iIconImage;
};

class IAchievements
{
private:

public:
	virtual bool RequestStats() = 0;
	virtual bool SetAchievement(const char* ID) = 0;
};
struct IGamePlatform
{
    virtual bool Init() = 0;
    virtual void Update() = 0;
    virtual void Shutdown() = 0;

    virtual IAchievements* GetAchievements() = 0;
};

// interface of the DLL
extern "C"
{
	IGAMEPLATFORM_API IGamePlatform* CreateGamePlatformInterface(ISystem* pSystem);
}
