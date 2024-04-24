#pragma once

#include <BlackBox/Core/Platform/IGamePlatform.hpp>

class GamePlatformSystem : public IGamePlatform
{
public:
	bool Init() override
	{
	}

	void Update() override
	{
	}

	void Shutdown() override
	{
	}

	IAchievements* GetAchievements() override
	{
		return nullptr;
	}
};

#include <steam/steam_api.h>

class CSteamAchievements : public IAchievements
{
private:
	int64          m_iAppID;           // Our current AppID
	Achievement_t* m_pAchievements;    // Achievements data
	int            m_iNumAchievements; // The number of Achievements
	bool           m_bInitialized;     // Have we called Request stats and received the callback?

public:
	CSteamAchievements(Achievement_t* Achievements, int NumAchievements);
	~CSteamAchievements();

	bool RequestStats() override;
	bool SetAchievement(const char* ID) override;

	STEAM_CALLBACK(CSteamAchievements, OnUserStatsReceived, UserStatsReceived_t,
		m_CallbackUserStatsReceived);
	STEAM_CALLBACK(CSteamAchievements, OnUserStatsStored, UserStatsStored_t,
		m_CallbackUserStatsStored);
	STEAM_CALLBACK(CSteamAchievements, OnAchievementStored,
		UserAchievementStored_t, m_CallbackAchievementStored);

};

class CSteamPlatform : public IGamePlatform
{
	CSteamAchievements* m_pAchievements;
public:
	bool Init() override;

	void Update() override
	{
		SteamAPI_RunCallbacks();
	}

	void Shutdown() override
	{
		SteamAPI_Shutdown();

		delete m_pAchievements;
		m_pAchievements = nullptr;

	}

	IAchievements* GetAchievements() override
	{
		return m_pAchievements;
	}

	void OpenOverlayToWebPage(const char* URL) override
	{
		SteamFriends()->ActivateGameOverlayToWebPage(URL);
	}

	void OpenGameOverlay( const char *pchDialog ) override
	{
		SteamFriends()->ActivateGameOverlay(pchDialog);
	}
};