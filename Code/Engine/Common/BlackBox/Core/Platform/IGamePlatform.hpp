#pragma once

#ifdef IGAMEPLATFORM_EXPORTS
#define IGAMEPLATFORM_API DLL_EXPORT
#elif defined(IS_MONOLITHIC_BUILD) && defined(CRY_IS_APPLICATION)
#define IGAMEPLATFORM_API
#else
#define IGAMEPLATFORM_API DLL_IMPORT
#endif

struct ISystem;

namespace Overlays
{
	constexpr static inline auto Friends = "Friends";
	constexpr static inline auto Community = "Community";
	constexpr static inline auto Players = "Players";
	constexpr static inline auto Settings = "Settings";
	constexpr static inline auto OfficialGameGroup = "OfficialGameGroup";
	constexpr static inline auto Stats = "Stats";
	constexpr static inline auto Achievements = "Achievements";
};


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

	virtual void OpenOverlayToWebPage(const char* URL) = 0;
	virtual void OpenGameOverlay( const char *pchDialog ) = 0;
};

// interface of the DLL
extern "C"
{
	IGAMEPLATFORM_API IGamePlatform* CreateGamePlatformInterface(ISystem* pSystem);
}
